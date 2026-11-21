//Chip specific functions for LMX2582 device
//
//All limits and register bit patterns in this file are taken from the TI LMX2582 datasheet
//SNAS680E (December 2015, revised August 2022).
//
//Key datasheet limits used here:
//  Reference input (OSCin)          5 - 1400 MHz                      (6.5 Electrical Characteristics)
//                                   (the program accepts 4 - 1401 MHz so that a reference oscillator that is slightly
//                                   low or high in frequency can still be used - same margin as the LMX2595)
//  OSCin doubler                    input 5 - 200 MHz, output 10 - 400 MHz   (Table 7-1)
//  Pre-R divider                    input 10 - 1400 MHz, output 5 - 700 MHz  (Table 7-1)
//  Multiplier                       input 40 - 70 MHz, output 180 - 250 MHz, MULT = 3,4,5 or 6 (Table 7-1, Table 8-2)
//                                   Must not be used together with the OSCin doubler (7.3.2)
//  Post-R divider                   input 5 - 250 MHz, output 0.25 - 125 MHz (Table 7-1)
//  PFD                              5 - 200 MHz normal (dual loop) mode
//                                   0.25 - 5 MHz extended low mode (needs PLL_N_PRE = 4)   (7.3.3)
//                                   200 - 400 MHz extended high mode is integer-N only, so is NOT used here
//                                   because this program relies on fractional-N for FSK and JT modes.
//  VCO                              3550 - 7100 MHz                   (7.3.5)
//  Output frequency                 20 - 5500 MHz                     (6.5 Electrical Characteristics)
//  Channel divider                  2 to 192, with VCO limits per division (Table 7-4)
//  Fvco = Fpd x PLL_N_PRE x (PLL_N + PLL_NUM / PLL_DEN)               (7.3.4)
//
//Unlike the LMX2595 this chip has no VCO doubler, no SYSREF and no ramping, and the
//channel divider is built from three segments (SEG1, SEG2, SEG3) rather than a single CHDIV value.

#define LMX2582CEPin 3         //GPO 3 Connect to LMX2582  CE Pin
#define LMX2582MUXPin 4         //GPO 4 Connect to LMX2582  MUXout Pin
#define LMX2582CSBPin 5         //GPO 5 Connect to LMX2582  CSB Pin
#define LMX2582SCKPin 6         //GPO 6 Connect to LMX2582  SCK Pin
#define LMX2582SDIPin 7         //GPO 7 Connect to LMX2582  SDI Pin

#include "SynthChip.h"

class LMX2582Chip : public SynthChip
{
public:

LMX2582Chip() { name = "LMX2582"; }

//LMX2582 Register Parameters.

//Reg 0
bool LMX2582_LD_EN;
byte LMX2582_FCAL_HPFD_ADJ;
byte LMX2582_FCAL_LPFD_ADJ;
bool LMX2582_ACAL_EN;
bool LMX2582_FCAL_EN;
bool LMX2582_MUXOUT_SEL;
bool LMX2582_RESET;
bool LMX2582_POWERDOWN;
//Reg 1
byte LMX2582_CAL_CLK_DIV;
//Reg 4
byte LMX2582_ACAL_CMP_DLY;
//Reg 8
bool LMX2582_VCO_IDAC_OVR;
bool LMX2582_VCO_CAPCTRL_OVR;
//Reg 9
bool LMX2582_OSC_2X;
bool LMX2582_REF_EN;
//Reg 10
byte LMX2582_MULT;
//Reg 11
byte LMX2582_PLL_R;
//Reg 12
unsigned int LMX2582_PLL_R_PRE;
//Reg 13
bool LMX2582_CP_EN;
byte LMX2582_PFD_CTL;
//Reg 14
byte LMX2582_CP_IDN;
byte LMX2582_CP_IUP;
byte LMX2582_CP_ICOARSE;
//Reg 19
unsigned int LMX2582_VCO_IDAC;
//Reg 20
unsigned int LMX2582_ACAL_VCO_IDAC_STRT;
//Reg 22
byte LMX2582_VCO_CAPCTRL;
//Reg 23
bool LMX2582_FCAL_VCO_SEL_STRT;
byte LMX2582_VCO_SEL;
bool LMX2582_VCO_SEL_FORCE;
//Reg 30
bool LMX2582_MASH_DITHER;
byte LMX2582_VTUNE_ADJ;
//Reg 31
bool LMX2582_VCO_DISTB_PD;
bool LMX2582_VCO_DISTA_PD;
bool LMX2582_CHDIV_DIST_PD;
//Reg 34
bool LMX2582_CHDIV_EN;
//Reg 35
byte LMX2582_CHDIV_SEG2;
bool LMX2582_CHDIV_SEG3_EN;
bool LMX2582_CHDIV_SEG2_EN;
bool LMX2582_CHDIV_SEG1;
bool LMX2582_CHDIV_SEG1_EN;
//Reg 36
bool LMX2582_CHDIV_DISTB_EN;
bool LMX2582_CHDIV_DISTA_EN;
byte LMX2582_CHDIV_SEG_SEL;
byte LMX2582_CHDIV_SEG3;
//Reg 37
bool LMX2582_PLL_N_PRE;              //0 = divide by 2, 1 = divide by 4
//Reg 38
unsigned int LMX2582_PLL_N;          //12 bits
//Reg 39
byte LMX2582_PFD_DLY;
//Reg 40
unsigned int LMX2582_PLL_DENH;        //High 16 bits
//Reg 41
unsigned int LMX2582_PLL_DENL;        //Low 16 bits
//combined 32 bit variable for conveniance
uint32_t LMX2582_PLL_DEN;
//Reg 42
unsigned int LMX2582_MASH_SEEDH;        //High 16 bits
//Reg 43
unsigned int LMX2582_MASH_SEEDL;        //Low 16 bits
//combined 32 bit variable for conveniance
uint32_t LMX2582_MASH_SEED;
//Reg 44
unsigned int LMX2582_PLL_NUMH;        //High 16 bits
//Reg 45
unsigned int LMX2582_PLL_NUML;        //Low 16 bits
//combined 32 bit variable for conveniance
uint32_t LMX2582_PLL_NUM;
//Reg 46
byte LMX2582_OUTA_POW;
bool LMX2582_OUTB_PD;
bool LMX2582_OUTA_PD;
byte LMX2582_MASH_ORDER;
//Reg 47
byte LMX2582_OUTA_MUX;
byte LMX2582_OUTB_POW;
//Reg 48
byte LMX2582_OUTB_MUX;
//Reg 59
bool LMX2582_MUXOUT_HDRV;
//Reg 61
bool LMX2582_LD_TYPE;
//Reg 64
bool LMX2582_ACAL_FAST;
bool LMX2582_FCAL_FAST;
byte LMX2582_AJUMP_SIZE;
byte LMX2582_FJUMP_SIZE;

//Readback registers R68 - R70 not used so not included.

//Only these registers exist on the LMX2582 (datasheet Figure 7-5). All others are left unwritten.
static const uint8_t numberOfValidRegs = 43;
const uint8_t validRegs[numberOfValidRegs] = {0,1,2,4,7,8,9,10,11,12,13,14,19,20,22,23,24,25,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,59,61,62,64};

//Channel divider settings, from datasheet Table 7-4. One row per available division ratio.
//minOut is the lowest output frequency for that division. (VCO limits in Table 7-4 are all met by these ranges.)
//SEG2/SEG3 register codes are 1 = /2, 2 = /4, 4 = /6, 8 = /8.  SEG1 code 0 = /2, 1 = /3.
static const uint8_t numberOfDividers = 15;
const uint8_t  chdivValue[numberOfDividers]  = {   2,    3,   4,   6,   8,  12,  16,  24,  32,  36,  48,  64,  96, 128, 192};
const uint16_t chdivMinOut[numberOfDividers] = {1775, 1184, 888, 592, 444, 296, 222, 148, 111,  99,  74,  56,  37,  28,  20};
const uint8_t  chdivSeg1[numberOfDividers]   = {   0,    1,   0,   1,   0,   0,   0,   1,   0,   1,   1,   0,   0,   0,   1};
const uint8_t  chdivSeg2[numberOfDividers]   = {   1,    1,   1,   1,   2,   4,   8,   8,   8,   4,   8,   8,   8,   8,   8};
const uint8_t  chdivSeg3[numberOfDividers]   = {   1,    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   4,   8,   8};
const uint8_t  chdivSegSel[numberOfDividers] = {   1,    1,   2,   2,   2,   2,   2,   2,   4,   4,   4,   4,   4,   4,   4};


void setDefault(void) override
{
//Datasheet default values unless noted.
//The frequency related values give an output of 435.000 MHz with a 100 MHz reference oscillator:
//PFD = 100 MHz, VCO = 100 x 2 x (26 + 1/10) = 5220 MHz, channel divider /12 (SEG1 /2 x SEG2 /6), output = 435 MHz.
//Reg 0
LMX2582_LD_EN = 1;
LMX2582_FCAL_HPFD_ADJ = 0;
LMX2582_FCAL_LPFD_ADJ = 0;
LMX2582_ACAL_EN = 1;
LMX2582_FCAL_EN = 0;                  //FCAL_EN is set by update() after all registers are written
LMX2582_MUXOUT_SEL = 1;
LMX2582_RESET = 0;
LMX2582_POWERDOWN = 0;
//Reg 1
LMX2582_CAL_CLK_DIV = 3;
//Reg 4
LMX2582_ACAL_CMP_DLY = 25;
//Reg 8
LMX2582_VCO_IDAC_OVR = 0;
LMX2582_VCO_CAPCTRL_OVR = 0;
//Reg 9
LMX2582_OSC_2X = 0;
LMX2582_REF_EN = 1;
//Reg 10
LMX2582_MULT = 1;
//Reg 11
LMX2582_PLL_R = 1;
//Reg 12
LMX2582_PLL_R_PRE = 1;
//Reg 13
LMX2582_CP_EN = 1;
LMX2582_PFD_CTL = 0;
//Reg 14
LMX2582_CP_IDN = 3;
LMX2582_CP_IUP = 3;
LMX2582_CP_ICOARSE = 1;
//Reg 19
LMX2582_VCO_IDAC = 300;
//Reg 20
LMX2582_ACAL_VCO_IDAC_STRT = 300;
//Reg 22
LMX2582_VCO_CAPCTRL = 0;
//Reg 23
LMX2582_FCAL_VCO_SEL_STRT = 0;
LMX2582_VCO_SEL = 1;
LMX2582_VCO_SEL_FORCE = 0;
//Reg 30
LMX2582_MASH_DITHER = 0;
LMX2582_VTUNE_ADJ = 0;
//Reg 31
LMX2582_VCO_DISTB_PD = 1;
LMX2582_VCO_DISTA_PD = 1;             //VCO direct path to output A powered down as output A uses the channel divider
LMX2582_CHDIV_DIST_PD = 0;
//Reg 34
LMX2582_CHDIV_EN = 1;
//Reg 35
LMX2582_CHDIV_SEG2 = 4;               //SEG2 /6
LMX2582_CHDIV_SEG3_EN = 0;
LMX2582_CHDIV_SEG2_EN = 1;
LMX2582_CHDIV_SEG1 = 0;               //SEG1 /2
LMX2582_CHDIV_SEG1_EN = 1;
//Reg 36
LMX2582_CHDIV_DISTB_EN = 0;
LMX2582_CHDIV_DISTA_EN = 1;
LMX2582_CHDIV_SEG_SEL = 2;            //SEG1 and SEG2 in use, total /12
LMX2582_CHDIV_SEG3 = 1;
//Reg 37
LMX2582_PLL_N_PRE = 0;
//Reg 38
LMX2582_PLL_N = 26;
//Reg 39
LMX2582_PFD_DLY = 2;
//Reg 40
LMX2582_PLL_DENH = 0;        //High 16 bits
//Reg 41
LMX2582_PLL_DENL = 10;        //Low 16 bits
//combined 32 bit variable for conveniance
LMX2582_PLL_DEN = 10;
//Reg 42
LMX2582_MASH_SEEDH = 0;        //High 16 bits
//Reg 43
LMX2582_MASH_SEEDL = 0;        //Low 16 bits
//combined 32 bit variable for conveniance
LMX2582_MASH_SEED = 0;
//Reg 44
LMX2582_PLL_NUMH = 0;        //High 16 bits
//Reg 45
LMX2582_PLL_NUML = 1;        //Low 16 bits
//combined 32 bit variable for conveniance
LMX2582_PLL_NUM = 1;
//Reg 46
LMX2582_OUTA_POW = 15;
LMX2582_OUTB_PD = 1;
LMX2582_OUTA_PD = 0;
LMX2582_MASH_ORDER = 3;
//Reg 47
LMX2582_OUTA_MUX = 0;                 //channel divider to output A
LMX2582_OUTB_POW = 0;
//Reg 48
LMX2582_OUTB_MUX = 0;
//Reg 59
LMX2582_MUXOUT_HDRV = 0;
//Reg 61
LMX2582_LD_TYPE = 1;
//Reg 64
LMX2582_ACAL_FAST = 0;
LMX2582_FCAL_FAST = 0;
LMX2582_AJUMP_SIZE = 3;
LMX2582_FJUMP_SIZE = 15;

  //See the note in MAX2870.ino setDefault() - setFrequency(0) here would
  //hang any non-interactive caller waiting for serial input.

  encodeRegs();
  update();
}

//--- Validators for parameters that only accept specific values (datasheet register descriptions) ---
static bool okMult(long v)   { return (v == 1) || ((v >= 3) && (v <= 6)); }                 //1 = bypass, 3-6 = multiplier
static bool okPfdCtl(long v) { return (v == 0) || (v == 3); }                               //0 = dual PFD, 3 = single PFD
static bool okVtune(long v)  { return (v == 0) || (v == 3); }                               //0 = Fvco < 6500, 3 = Fvco >= 6500
static bool okSeg(long v)    { return (v == 0) || (v == 1) || (v == 2) || (v == 4) || (v == 8); }   //0 PD, 1 /2, 2 /4, 4 /6, 8 /8
static bool okSegSel(long v) { return (v == 0) || (v == 1) || (v == 2) || (v == 4); }
static bool okPfdDly(long v) { return (v == 1) || (v == 2) || (v == 4) || (v == 8) || (v == 16); }
static bool okPow(long v)    { return ((v >= 0) && (v <= 31)) || ((v >= 48) && (v <= 63)); }
static bool okVcoSel(long v) { return (v >= 1) && (v <= 7); }

//Same behaviour as paramByte() in Menu.ino, but only accepts values for which valid() returns true.
bool paramByteValid(String param , String name , byte* var , String value , bool (*valid)(long) , const char* allowed)
{
  bool ret = false;
  if((param == name) | (param == "*"))
    {
      if(value.length() > 0)
        {
          if(valid(value.toInt()))
            {
              *var = value.toInt();
            }
          else
            {
              Serial.print("Invalid value. Allowed values are ");
              Serial.println(allowed);
            }
        }
      Serial.print(name);
      Serial.print(" = ");
      Serial.println(*var);
      if (param != "*") ret = true;
    }
  return ret;
}

void setParameters(void) override
{
  String resp;
  String param;
  String value;
  bool done = false;
  int equals;
  while(!done)
  {
    Serial.print("\nEnter Parameter or ? for help--> ");
    resp = inputString(true);
    if (resp.length() == 0) done = true;
    if(resp.indexOf("?") != -1)
      {
        Serial.println();
        Serial.println("Enter 'Name' to view current Parameter value.");
        Serial.println("Enter 'Name = Value' to set Parameter value.");
        Serial.println("Enter '*' to view all Parameter values.");
        Serial.println("Enter blank line to exit");
        Serial.println("Valid Parameter Names (See LMX2582 Data Sheet for details) are:-");
        Serial.println("Reg 0 = LD_EN FCAL_HPFD_ADJ FCAL_LPFD_ADJ ACAL_EN FCAL_EN MUXOUT_SEL RESET POWERDOWN");
        Serial.println("Reg 1 = CAL_CLK_DIV");
        Serial.println("Reg 4 = ACAL_CMP_DLY");
        Serial.println("Reg 8 = VCO_IDAC_OVR VCO_CAPCTRL_OVR");
        Serial.println("Reg 9 = OSC_2X REF_EN");
        Serial.println("Reg 10 = MULT (1 = bypass, 3 to 6)");
        Serial.println("Reg 11 = PLL_R");
        Serial.println("Reg 12 = PLL_R_PRE");
        Serial.println("Reg 13 = CP_EN PFD_CTL");
        Serial.println("Reg 14 = CP_IDN CP_IUP CP_ICOARSE   (CP_IDN and CP_IUP are kept equal)");
        Serial.println("Reg 19 = VCO_IDAC (250 to 450)");
        Serial.println("Reg 20 = ACAL_VCO_IDAC_STRT (250 to 450)");
        Serial.println("Reg 22 = VCO_CAPCTRL");
        Serial.println("Reg 23 = FCAL_VCO_SEL_STRT VCO_SEL VCO_SEL_FORCE");
        Serial.println("Reg 30 = MASH_DITHER VTUNE_ADJ");
        Serial.println("Reg 31 = VCO_DISTB_PD VCO_DISTA_PD CHDIV_DIST_PD");
        Serial.println("Reg 34 = CHDIV_EN");
        Serial.println("Reg 35 = CHDIV_SEG2 CHDIV_SEG3_EN CHDIV_SEG2_EN CHDIV_SEG1 CHDIV_SEG1_EN");
        Serial.println("Reg 36 = CHDIV_DISTB_EN CHDIV_DISTA_EN CHDIV_SEG_SEL CHDIV_SEG3");
        Serial.println("Reg 37 = PLL_N_PRE");
        Serial.println("Reg 38 = PLL_N");
        Serial.println("Reg 39 = PFD_DLY");
        Serial.println("Reg 40/41 = PLL_DEN");
        Serial.println("Reg 42/43 = MASH_SEED");
        Serial.println("Reg 44/45 = PLL_NUM");
        Serial.println("Reg 46 = OUTA_POW OUTB_PD OUTA_PD MASH_ORDER");
        Serial.println("Reg 47 = OUTA_MUX OUTB_POW");
        Serial.println("Reg 48 = OUTB_MUX");
        Serial.println("Reg 59 = MUXOUT_HDRV");
        Serial.println("Reg 61 = LD_TYPE");
        Serial.println("Reg 64 = ACAL_FAST FCAL_FAST AJUMP_SIZE FJUMP_SIZE");
        Serial.println("Readback registers R68 - R70 are not used or supported.");

        Serial.println();
      }

    equals = resp.indexOf("=");

    if(equals > -1)                 // Trying to assigng value
      {
        param = resp.substring(0,equals);
        param.trim();
        value = resp.substring(equals + 1);
        value.trim();
      }
     else
      {
        param = resp;
        param.trim();
        value = "";
      }

// Check each possible Parameter name and process it.

//Reg 0
      if(paramBool(param , "LD_EN" , &LMX2582_LD_EN , value)) goto handled;
      if(paramByte(param , "FCAL_HPFD_ADJ" , &LMX2582_FCAL_HPFD_ADJ , value , 0 , 3)) goto handled;
      if(paramByte(param , "FCAL_LPFD_ADJ" , &LMX2582_FCAL_LPFD_ADJ , value , 0 , 3)) goto handled;
      if(paramBool(param , "ACAL_EN" , &LMX2582_ACAL_EN , value)) goto handled;
      if(paramBool(param , "FCAL_EN" , &LMX2582_FCAL_EN , value)) goto handled;
      if(paramBool(param , "MUXOUT_SEL" , &LMX2582_MUXOUT_SEL , value)) goto handled;
      if(paramBool(param , "RESET" , &LMX2582_RESET , value)) goto handled;
      if(paramBool(param , "POWERDOWN" , &LMX2582_POWERDOWN , value)) goto handled;
//Reg 1
      if(paramByte(param , "CAL_CLK_DIV" , &LMX2582_CAL_CLK_DIV , value , 0 , 7)) goto handled;
//Reg 4
      if(paramByte(param , "ACAL_CMP_DLY" , &LMX2582_ACAL_CMP_DLY , value , 0 , 255)) goto handled;
//Reg 8
      if(paramBool(param , "VCO_IDAC_OVR" , &LMX2582_VCO_IDAC_OVR , value)) goto handled;
      if(paramBool(param , "VCO_CAPCTRL_OVR" , &LMX2582_VCO_CAPCTRL_OVR , value)) goto handled;
//Reg 9
      if(paramBool(param , "OSC_2X" , &LMX2582_OSC_2X , value)) goto handled;
      if(paramBool(param , "REF_EN" , &LMX2582_REF_EN , value)) goto handled;
//Reg 10
      if(paramByteValid(param , "MULT" , &LMX2582_MULT , value , okMult , "1 (bypass), 3, 4, 5, 6")) goto handled;
//Reg 11
      if(paramByte(param , "PLL_R" , &LMX2582_PLL_R , value , 1 , 255)) goto handled;
//Reg 12
      if(paramUint(param , "PLL_R_PRE" , &LMX2582_PLL_R_PRE , value , 1 , 4095)) goto handled;
//Reg 13
      if(paramBool(param , "CP_EN" , &LMX2582_CP_EN , value)) goto handled;
      if(paramByteValid(param , "PFD_CTL" , &LMX2582_PFD_CTL , value , okPfdCtl , "0 (dual PFD), 3 (single PFD, PFD > 200 MHz integer only)")) goto handled;
//Reg 14
      if(paramByte(param , "CP_IDN" , &LMX2582_CP_IDN , value , 0 , 31)) { LMX2582_CP_IUP = LMX2582_CP_IDN; goto handled; }
      if(paramByte(param , "CP_IUP" , &LMX2582_CP_IUP , value , 0 , 31)) { LMX2582_CP_IDN = LMX2582_CP_IUP; goto handled; }
      if(paramByte(param , "CP_ICOARSE" , &LMX2582_CP_ICOARSE , value , 0 , 3)) goto handled;
//Reg 19
      if(paramUint(param , "VCO_IDAC" , &LMX2582_VCO_IDAC , value , 250 , 450)) goto handled;
//Reg 20
      if(paramUint(param , "ACAL_VCO_IDAC_STRT" , &LMX2582_ACAL_VCO_IDAC_STRT , value , 250 , 450)) goto handled;
//Reg 22
      if(paramByte(param , "VCO_CAPCTRL" , &LMX2582_VCO_CAPCTRL , value , 0 , 183)) goto handled;
//Reg 23
      if(paramBool(param , "FCAL_VCO_SEL_STRT" , &LMX2582_FCAL_VCO_SEL_STRT , value)) goto handled;
      if(paramByteValid(param , "VCO_SEL" , &LMX2582_VCO_SEL , value , okVcoSel , "1 to 7")) goto handled;
      if(paramBool(param , "VCO_SEL_FORCE" , &LMX2582_VCO_SEL_FORCE , value)) goto handled;
//Reg 30
      if(paramBool(param , "MASH_DITHER" , &LMX2582_MASH_DITHER , value)) goto handled;
      if(paramByteValid(param , "VTUNE_ADJ" , &LMX2582_VTUNE_ADJ , value , okVtune , "0 (Fvco < 6500 MHz), 3 (Fvco >= 6500 MHz)")) goto handled;
//Reg 31
      if(paramBool(param , "VCO_DISTB_PD" , &LMX2582_VCO_DISTB_PD , value)) goto handled;
      if(paramBool(param , "VCO_DISTA_PD" , &LMX2582_VCO_DISTA_PD , value)) goto handled;
      if(paramBool(param , "CHDIV_DIST_PD" , &LMX2582_CHDIV_DIST_PD , value)) goto handled;
//Reg 34
      if(paramBool(param , "CHDIV_EN" , &LMX2582_CHDIV_EN , value)) goto handled;
//Reg 35
      if(paramByteValid(param , "CHDIV_SEG2" , &LMX2582_CHDIV_SEG2 , value , okSeg , "0 (PD), 1 (/2), 2 (/4), 4 (/6), 8 (/8)")) goto handled;
      if(paramBool(param , "CHDIV_SEG3_EN" , &LMX2582_CHDIV_SEG3_EN , value)) goto handled;
      if(paramBool(param , "CHDIV_SEG2_EN" , &LMX2582_CHDIV_SEG2_EN , value)) goto handled;
      if(paramBool(param , "CHDIV_SEG1" , &LMX2582_CHDIV_SEG1 , value)) goto handled;
      if(paramBool(param , "CHDIV_SEG1_EN" , &LMX2582_CHDIV_SEG1_EN , value)) goto handled;
//Reg 36
      if(paramBool(param , "CHDIV_DISTB_EN" , &LMX2582_CHDIV_DISTB_EN , value)) goto handled;
      if(paramBool(param , "CHDIV_DISTA_EN" , &LMX2582_CHDIV_DISTA_EN , value)) goto handled;
      if(paramByteValid(param , "CHDIV_SEG_SEL" , &LMX2582_CHDIV_SEG_SEL , value , okSegSel , "0 (PD), 1 (SEG1), 2 (SEG1+2), 4 (SEG1+2+3)")) goto handled;
      if(paramByteValid(param , "CHDIV_SEG3" , &LMX2582_CHDIV_SEG3 , value , okSeg , "0 (PD), 1 (/2), 2 (/4), 4 (/6), 8 (/8)")) goto handled;
//Reg 37
      if(paramBool(param , "PLL_N_PRE" , &LMX2582_PLL_N_PRE , value)) goto handled;
//Reg 38
      if(paramUint(param , "PLL_N" , &LMX2582_PLL_N , value , 0 , 4095)) goto handled;
//Reg 39
      if(paramByteValid(param , "PFD_DLY" , &LMX2582_PFD_DLY , value , okPfdDly , "1, 2, 4, 8, 16")) goto handled;
//Reg 40
//Reg 41
//combined 32 bit variable for conveniance
      if(paramUint32(param , "PLL_DEN" , &LMX2582_PLL_DEN , value , 1 , 0xFFFFFFFF)) goto handled;
//Reg 42
//Reg 43
//combined 32 bit variable for conveniance
      if(paramUint32(param , "MASH_SEED" , &LMX2582_MASH_SEED , value , 0 , 0xFFFFFFFF)) goto handled;
//Reg 44
//Reg 45
//combined 32 bit variable for conveniance
      if(paramUint32(param , "PLL_NUM" , &LMX2582_PLL_NUM , value , 0 , 0xFFFFFFFF)) goto handled;
//Reg 46
      if(paramByteValid(param , "OUTA_POW" , &LMX2582_OUTA_POW , value , okPow , "0 to 31, 48 to 63 (extra boost)")) goto handled;
      if(paramBool(param , "OUTB_PD" , &LMX2582_OUTB_PD , value)) goto handled;
      if(paramBool(param , "OUTA_PD" , &LMX2582_OUTA_PD , value)) goto handled;
      if(paramByte(param , "MASH_ORDER" , &LMX2582_MASH_ORDER , value , 0 , 4)) goto handled;
//Reg 47
      if(paramByte(param , "OUTA_MUX" , &LMX2582_OUTA_MUX , value , 0 , 1)) goto handled;
      if(paramByteValid(param , "OUTB_POW" , &LMX2582_OUTB_POW , value , okPow , "0 to 31, 48 to 63 (extra boost)")) goto handled;
//Reg 48
      if(paramByte(param , "OUTB_MUX" , &LMX2582_OUTB_MUX , value , 0 , 1)) goto handled;
//Reg 59
      if(paramBool(param , "MUXOUT_HDRV" , &LMX2582_MUXOUT_HDRV , value)) goto handled;
//Reg 61
      if(paramBool(param , "LD_TYPE" , &LMX2582_LD_TYPE , value)) goto handled;
//Reg 64
      if(paramBool(param , "ACAL_FAST" , &LMX2582_ACAL_FAST , value)) goto handled;
      if(paramBool(param , "FCAL_FAST" , &LMX2582_FCAL_FAST , value)) goto handled;
      if(paramByte(param , "AJUMP_SIZE" , &LMX2582_AJUMP_SIZE , value , 0 , 7)) goto handled;
      if(paramByte(param , "FJUMP_SIZE" , &LMX2582_FJUMP_SIZE , value , 0 , 15)) goto handled;

    handled:
    encodeRegs();
    update();

  }
}

void init(void) override
{
  numberOfRegs = 65;                   //registers R0 - R64 (only those listed in validRegs[] are written. Readback registers R68 - R70 not used.)
  numberOfBits = 24;                   //number of bits in each register. Top 8 bits are R/W + register address, low 16 bits are data.
  maxPfd = 200.0;                      //normal (dual loop) PFD maximum. 200-400 MHz extended mode is integer-N only so is not used.
  minPfd = 0.25;                       //extended low PFD mode minimum (PLL_N_PRE is automatically set to /4 below 5 MHz)
  maxOsc = 1401;                       //datasheet reference input range is 5 - 1400 MHz. Deliberately 1 MHz wider each end
  minOsc = 4;                          //to allow a reference oscillator that is slightly low or high in frequency.
  jt4Only = false;
  jtDisable = false;
  pinMode(LMX2582CEPin,OUTPUT);
  digitalWrite(LMX2582CEPin,HIGH);
  pinMode(LMX2582CSBPin,OUTPUT);
  digitalWrite(LMX2582CSBPin,HIGH);
  SPI.setRX(LMX2582MUXPin);
  SPI.setTX(LMX2582SDIPin);
  SPI.setSCK(LMX2582SCKPin);
  SPI.beginTransaction(SPISettings(400000,MSBFIRST,SPI_MODE0));
  SPI.begin();
}

void LMX2582Send(int32_t val)
{
  int a=(val >> 16) & 0xFF;
  int b=(val >> 8) & 0xFF;
  int c=val & 0xFF;

  digitalWrite(LMX2582CSBPin,LOW);
  delayMicroseconds(10);
  SPI.transfer((byte)a);
  SPI.transfer((byte)b);
  SPI.transfer((byte)c);
  delayMicroseconds(10);
  digitalWrite(LMX2582CSBPin,HIGH);
  delayMicroseconds(10);
}

void LMX2582FastSend(int32_t val1, int32_t val2)
{
  int a=(val1 >> 16) & 0xFF;
  int b=(val1 >> 8) & 0xFF;
  int c=val1 & 0xFF;
  int d=(val2 >> 16) & 0xFF;
  int e=(val2 >> 8) & 0xFF;
  int f=val2 & 0xFF;

  digitalWrite(LMX2582CSBPin,LOW);
  delayMicroseconds(1);
  SPI.transfer((byte)a);
  SPI.transfer((byte)b);
  SPI.transfer((byte)c);
  digitalWrite(LMX2582CSBPin,HIGH);
  digitalWrite(LMX2582CSBPin,LOW);
  SPI.transfer((byte)d);
  SPI.transfer((byte)e);
  SPI.transfer((byte)f);
  delayMicroseconds(1);
  digitalWrite(LMX2582CSBPin,HIGH);
  delayMicroseconds(1);
}

//Single register version of LMX2582FastSend(). Needed because the LMX2582 N divider is in one register (R38).
void LMX2582FastSendOne(int32_t val)
{
  int a=(val >> 16) & 0xFF;
  int b=(val >> 8) & 0xFF;
  int c=val & 0xFF;

  digitalWrite(LMX2582CSBPin,LOW);
  delayMicroseconds(1);
  SPI.transfer((byte)a);
  SPI.transfer((byte)b);
  SPI.transfer((byte)c);
  delayMicroseconds(1);
  digitalWrite(LMX2582CSBPin,HIGH);
  delayMicroseconds(1);
}

//Build the R38 register word for a given N value. PLL_N occupies bits 12:1.
uint32_t nReg(uint32_t n)
{
  return (38UL << 16) | ((n & 0xFFF) << 1);
}


void encodeRegs(void) override
{
  // first we initialise the address field of all the registers.
  for(int r = 0 ;r < numberOfRegs ; r++)
    {
      chanData[channel].reg[r] = r << 16;
    }

  //next we map the parameters into the registers. Most registers also have additional fixed bit patterns defined in the datasheet (Figure 7-5).
  chanData[channel].reg[0] = (chanData[channel].reg[0]) | (LMX2582_LD_EN << 13) | (1 << 9) | (LMX2582_FCAL_HPFD_ADJ << 7) | (LMX2582_FCAL_LPFD_ADJ << 5) | (LMX2582_ACAL_EN << 4) | (LMX2582_FCAL_EN << 3) | (LMX2582_MUXOUT_SEL << 2) | (LMX2582_RESET << 1) | (LMX2582_POWERDOWN);
  chanData[channel].reg[1] = (chanData[channel].reg[1]) | (0b100000001 << 3) | (LMX2582_CAL_CLK_DIV & 0x07);
  chanData[channel].reg[2] = (chanData[channel].reg[2]) | (0b101 << 8);
  chanData[channel].reg[4] = (chanData[channel].reg[4]) | (LMX2582_ACAL_CMP_DLY << 8) | (0b1000011);
  chanData[channel].reg[7] = (chanData[channel].reg[7]) | (0b10100010110010);
  chanData[channel].reg[8] = (chanData[channel].reg[8]) | (LMX2582_VCO_IDAC_OVR << 13) | (1 << 12) | (LMX2582_VCO_CAPCTRL_OVR << 10) | (0b10000100);
  chanData[channel].reg[9] = (chanData[channel].reg[9]) | (LMX2582_OSC_2X << 11) | (LMX2582_REF_EN << 9) | (0b100000010);
  chanData[channel].reg[10] = (chanData[channel].reg[10]) | (1 << 12) | ((LMX2582_MULT & 0x1F) << 7) | (0b1011000);
  chanData[channel].reg[11] = (chanData[channel].reg[11]) | (LMX2582_PLL_R << 4) | (0b1000);
  chanData[channel].reg[12] = (chanData[channel].reg[12]) | (0b111 << 12) | (LMX2582_PLL_R_PRE & 0xFFF);
  chanData[channel].reg[13] = (chanData[channel].reg[13]) | (LMX2582_CP_EN << 14) | (LMX2582_PFD_CTL & 0x03);
  chanData[channel].reg[14] = (chanData[channel].reg[14]) | ((LMX2582_CP_IDN & 0x1F) << 7) | ((LMX2582_CP_IUP & 0x1F) << 2) | (LMX2582_CP_ICOARSE & 0x03);
  chanData[channel].reg[19] = (chanData[channel].reg[19]) | ((LMX2582_VCO_IDAC & 0x1FF) << 3) | (0b101);
  chanData[channel].reg[20] = (chanData[channel].reg[20]) | (LMX2582_ACAL_VCO_IDAC_STRT & 0x1FF);
  chanData[channel].reg[22] = (chanData[channel].reg[22]) | (0b100011 << 8) | (LMX2582_VCO_CAPCTRL);
  chanData[channel].reg[23] = (chanData[channel].reg[23]) | (1 << 15) | (LMX2582_FCAL_VCO_SEL_STRT << 14) | ((LMX2582_VCO_SEL & 0x07) << 11) | (LMX2582_VCO_SEL_FORCE << 10) | (0b1000010);
  chanData[channel].reg[24] = (chanData[channel].reg[24]) | (0b10100001001);
  chanData[channel].reg[25] = (chanData[channel].reg[25]) | (0b0);
  chanData[channel].reg[28] = (chanData[channel].reg[28]) | (0b10100100100100);
  chanData[channel].reg[29] = (chanData[channel].reg[29]) | (0b10000100);
  chanData[channel].reg[30] = (chanData[channel].reg[30]) | (LMX2582_MASH_DITHER << 10) | ((LMX2582_VTUNE_ADJ & 0x03) << 6) | (0b110100);
  chanData[channel].reg[31] = (chanData[channel].reg[31]) | (LMX2582_VCO_DISTB_PD << 10) | (LMX2582_VCO_DISTA_PD << 9) | (LMX2582_CHDIV_DIST_PD << 7) | (0b1);
  chanData[channel].reg[32] = (chanData[channel].reg[32]) | (0b10000100001010);
  chanData[channel].reg[33] = (chanData[channel].reg[33]) | (0b10101000001010);
  chanData[channel].reg[34] = (chanData[channel].reg[34]) | (0b1100001111 << 6) | (LMX2582_CHDIV_EN << 5) | (0b1010);
  chanData[channel].reg[35] = (chanData[channel].reg[35]) | ((LMX2582_CHDIV_SEG2 & 0x0F) << 9) | (LMX2582_CHDIV_SEG3_EN << 8) | (LMX2582_CHDIV_SEG2_EN << 7) | (0b11 << 3) | (LMX2582_CHDIV_SEG1 << 2) | (LMX2582_CHDIV_SEG1_EN << 1) | (0b1);
  chanData[channel].reg[36] = (chanData[channel].reg[36]) | (LMX2582_CHDIV_DISTB_EN << 11) | (LMX2582_CHDIV_DISTA_EN << 10) | ((LMX2582_CHDIV_SEG_SEL & 0x07) << 4) | (LMX2582_CHDIV_SEG3 & 0x0F);
  chanData[channel].reg[37] = (chanData[channel].reg[37]) | (1 << 14) | (LMX2582_PLL_N_PRE << 12);
  chanData[channel].reg[38] = (chanData[channel].reg[38]) | ((LMX2582_PLL_N & 0xFFF) << 1);
  chanData[channel].reg[39] = (chanData[channel].reg[39]) | (1 << 15) | ((LMX2582_PFD_DLY & 0x3F) << 8) | (0b100);
  //split 32 bit variable into two parts
  LMX2582_PLL_DENH = (LMX2582_PLL_DEN >> 16) & 0xFFFF;
  LMX2582_PLL_DENL = LMX2582_PLL_DEN & 0xFFFF;
  chanData[channel].reg[40] = (chanData[channel].reg[40]) | (LMX2582_PLL_DENH);
  chanData[channel].reg[41] = (chanData[channel].reg[41]) | (LMX2582_PLL_DENL);
  //split 32 bit variable into two parts
  LMX2582_MASH_SEEDH = (LMX2582_MASH_SEED >> 16) & 0xFFFF;
  LMX2582_MASH_SEEDL = LMX2582_MASH_SEED & 0xFFFF;
  chanData[channel].reg[42] = (chanData[channel].reg[42]) | (LMX2582_MASH_SEEDH);
  chanData[channel].reg[43] = (chanData[channel].reg[43]) | (LMX2582_MASH_SEEDL);
  //split 32 bit variable into two parts
  LMX2582_PLL_NUMH = (LMX2582_PLL_NUM >> 16) & 0xFFFF;
  LMX2582_PLL_NUML = LMX2582_PLL_NUM & 0xFFFF;
  chanData[channel].reg[44] = (chanData[channel].reg[44]) | (LMX2582_PLL_NUMH);
  chanData[channel].reg[45] = (chanData[channel].reg[45]) | (LMX2582_PLL_NUML);
  chanData[channel].reg[46] = (chanData[channel].reg[46]) | ((LMX2582_OUTA_POW & 0x3F) << 8) | (LMX2582_OUTB_PD << 7) | (LMX2582_OUTA_PD << 6) | (1 << 5) | (LMX2582_MASH_ORDER & 0x07);
  chanData[channel].reg[47] = (chanData[channel].reg[47]) | ((LMX2582_OUTA_MUX & 0x03) << 11) | (0b11 << 6) | (LMX2582_OUTB_POW & 0x3F);
  chanData[channel].reg[48] = (chanData[channel].reg[48]) | (0b11111111 << 2) | (LMX2582_OUTB_MUX & 0x03);
  chanData[channel].reg[59] = (chanData[channel].reg[59]) | (LMX2582_MUXOUT_HDRV << 5);
  chanData[channel].reg[61] = (chanData[channel].reg[61]) | (LMX2582_LD_TYPE);
  chanData[channel].reg[62] = (chanData[channel].reg[62]) | (0b0);
  chanData[channel].reg[64] = (chanData[channel].reg[64]) | (LMX2582_ACAL_FAST << 9) | (LMX2582_FCAL_FAST << 8) | ((LMX2582_AJUMP_SIZE & 0x07) << 5) | (1 << 4) | (LMX2582_FJUMP_SIZE & 0x0F);
}

void decodeRegs(void) override
{
//Reg 0
LMX2582_LD_EN = (chanData[channel].reg[0] >> 13) & 0x01;
LMX2582_FCAL_HPFD_ADJ = (chanData[channel].reg[0] >> 7) & 0x03;
LMX2582_FCAL_LPFD_ADJ = (chanData[channel].reg[0] >> 5) & 0x03;
LMX2582_ACAL_EN = (chanData[channel].reg[0] >> 4) & 0x01;
LMX2582_FCAL_EN = (chanData[channel].reg[0] >> 3) & 0x01;
LMX2582_MUXOUT_SEL = (chanData[channel].reg[0] >> 2) & 0x01;
LMX2582_RESET = (chanData[channel].reg[0] >> 1) & 0x01;
LMX2582_POWERDOWN = (chanData[channel].reg[0]) & 0x01;
//Reg 1
LMX2582_CAL_CLK_DIV = (chanData[channel].reg[1]) & 0x07;
//Reg 4
LMX2582_ACAL_CMP_DLY = (chanData[channel].reg[4] >> 8) & 0xFF;
//Reg 8
LMX2582_VCO_IDAC_OVR = (chanData[channel].reg[8] >> 13) & 0x01;
LMX2582_VCO_CAPCTRL_OVR = (chanData[channel].reg[8] >> 10) & 0x01;
//Reg 9
LMX2582_OSC_2X = (chanData[channel].reg[9] >> 11) & 0x01;
LMX2582_REF_EN = (chanData[channel].reg[9] >> 9) & 0x01;
//Reg 10
LMX2582_MULT = (chanData[channel].reg[10] >> 7) & 0x1F;
//Reg 11
LMX2582_PLL_R = (chanData[channel].reg[11] >> 4) & 0xFF;
//Reg 12
LMX2582_PLL_R_PRE = (chanData[channel].reg[12]) & 0xFFF;
//Reg 13
LMX2582_CP_EN = (chanData[channel].reg[13] >> 14) & 0x01;
LMX2582_PFD_CTL = (chanData[channel].reg[13]) & 0x03;
//Reg 14
LMX2582_CP_IDN = (chanData[channel].reg[14] >> 7) & 0x1F;
LMX2582_CP_IUP = (chanData[channel].reg[14] >> 2) & 0x1F;
LMX2582_CP_ICOARSE = (chanData[channel].reg[14]) & 0x03;
//Reg 19
LMX2582_VCO_IDAC = (chanData[channel].reg[19] >> 3) & 0x1FF;
//Reg 20
LMX2582_ACAL_VCO_IDAC_STRT = (chanData[channel].reg[20]) & 0x1FF;
//Reg 22
LMX2582_VCO_CAPCTRL = (chanData[channel].reg[22]) & 0xFF;
//Reg 23
LMX2582_FCAL_VCO_SEL_STRT = (chanData[channel].reg[23] >> 14) & 0x01;
LMX2582_VCO_SEL = (chanData[channel].reg[23] >> 11) & 0x07;
LMX2582_VCO_SEL_FORCE = (chanData[channel].reg[23] >> 10) & 0x01;
//Reg 30
LMX2582_MASH_DITHER = (chanData[channel].reg[30] >> 10) & 0x01;
LMX2582_VTUNE_ADJ = (chanData[channel].reg[30] >> 6) & 0x03;
//Reg 31
LMX2582_VCO_DISTB_PD = (chanData[channel].reg[31] >> 10) & 0x01;
LMX2582_VCO_DISTA_PD = (chanData[channel].reg[31] >> 9) & 0x01;
LMX2582_CHDIV_DIST_PD = (chanData[channel].reg[31] >> 7) & 0x01;
//Reg 34
LMX2582_CHDIV_EN = (chanData[channel].reg[34] >> 5) & 0x01;
//Reg 35
LMX2582_CHDIV_SEG2 = (chanData[channel].reg[35] >> 9) & 0x0F;
LMX2582_CHDIV_SEG3_EN = (chanData[channel].reg[35] >> 8) & 0x01;
LMX2582_CHDIV_SEG2_EN = (chanData[channel].reg[35] >> 7) & 0x01;
LMX2582_CHDIV_SEG1 = (chanData[channel].reg[35] >> 2) & 0x01;
LMX2582_CHDIV_SEG1_EN = (chanData[channel].reg[35] >> 1) & 0x01;
//Reg 36
LMX2582_CHDIV_DISTB_EN = (chanData[channel].reg[36] >> 11) & 0x01;
LMX2582_CHDIV_DISTA_EN = (chanData[channel].reg[36] >> 10) & 0x01;
LMX2582_CHDIV_SEG_SEL = (chanData[channel].reg[36] >> 4) & 0x07;
LMX2582_CHDIV_SEG3 = (chanData[channel].reg[36]) & 0x0F;
//Reg 37
LMX2582_PLL_N_PRE = (chanData[channel].reg[37] >> 12) & 0x01;
//Reg 38
LMX2582_PLL_N = (chanData[channel].reg[38] >> 1) & 0xFFF;
//Reg 39
LMX2582_PFD_DLY = (chanData[channel].reg[39] >> 8) & 0x3F;
//Reg 40
LMX2582_PLL_DENH = (chanData[channel].reg[40]) & 0xFFFF;        //High 16 bits
//Reg 41
LMX2582_PLL_DENL = (chanData[channel].reg[41]) & 0xFFFF ;        //Low 16 bits
//combined 32 bit variable for conveniance
LMX2582_PLL_DEN = ((uint32_t) LMX2582_PLL_DENH << 16) | LMX2582_PLL_DENL;
//Reg 42
LMX2582_MASH_SEEDH = (chanData[channel].reg[42]) & 0xFFFF;        //High 16 bits
//Reg 43
LMX2582_MASH_SEEDL = (chanData[channel].reg[43]) & 0xFFFF;        //Low 16 bits
//combined 32 bit variable for conveniance
LMX2582_MASH_SEED = ((uint32_t) LMX2582_MASH_SEEDH << 16) | LMX2582_MASH_SEEDL ;
//Reg 44
LMX2582_PLL_NUMH = (chanData[channel].reg[44]) & 0xFFFF;        //High 16 bits
//Reg 45
LMX2582_PLL_NUML = (chanData[channel].reg[45]) & 0xFFFF;        //Low 16 bits
//combined 32 bit variable for conveniance
LMX2582_PLL_NUM = ((uint32_t) LMX2582_PLL_NUMH << 16) | LMX2582_PLL_NUML;
//Reg 46
LMX2582_OUTA_POW = (chanData[channel].reg[46] >> 8) & 0x3F;
LMX2582_OUTB_PD = (chanData[channel].reg[46] >> 7) & 0x01;
LMX2582_OUTA_PD = (chanData[channel].reg[46] >> 6) & 0x01;
LMX2582_MASH_ORDER = (chanData[channel].reg[46]) & 0x07;
//Reg 47
LMX2582_OUTA_MUX = (chanData[channel].reg[47] >> 11) & 0x03;
LMX2582_OUTB_POW = (chanData[channel].reg[47]) & 0x3F;
//Reg 48
LMX2582_OUTB_MUX = (chanData[channel].reg[48]) & 0x03;
//Reg 59
LMX2582_MUXOUT_HDRV = (chanData[channel].reg[59] >> 5) & 0x01;
//Reg 61
LMX2582_LD_TYPE = (chanData[channel].reg[61]) & 0x01;
//Reg 64
LMX2582_ACAL_FAST = (chanData[channel].reg[64] >> 9) & 0x01;
LMX2582_FCAL_FAST = (chanData[channel].reg[64] >> 8) & 0x01;
LMX2582_AJUMP_SIZE = (chanData[channel].reg[64] >> 5) & 0x07;
LMX2582_FJUMP_SIZE = (chanData[channel].reg[64]) & 0x0F;

}

//Follows the datasheet 7.5.1 power on sequence: soft reset, program the remaining registers (highest first), then frequency calibrate.
void update(void) override
{
  LMX2582Send(chanData[channel].reg[0] | 0x02);           //Reg 0 + reset bit
  LMX2582Send(chanData[channel].reg[0] & ~0x02UL);        //Reg 0 no reset
  for(int i = numberOfValidRegs - 1; i >= 0; i--)
  {
      LMX2582Send(chanData[channel].reg[validRegs[i]]);
  }
  delay(10);
  LMX2582Send(chanData[channel].reg[0] | 0x08);         //set FCAL_EN
}

//Set the calibration clock divider. Calibration clock = OSCin / 2^CAL_CLK_DIV and must be no more than 200 MHz
//(datasheet R1 description). With the 1400 MHz maximum OSCin a value of 3 is always enough.
void setCalClkDiv(void)
{
  byte d = 0;
  while(((refOsc / (double)(1 << d)) > 200.0) && (d < 3)) d++;
  LMX2582_CAL_CLK_DIV = d;
}

//Try to split a total division of fin/rpfd between the Pre-R divider and the Post-R divider
//while keeping every stage within the datasheet Table 7-1 limits.
//Returns true and sets *pre and *r if successful.
bool findDividers(double fin, double rpfd, unsigned int* pre, byte* r)
{
  double d = fin / rpfd;
  double dr = floor(d + 0.5);
  if(fabs(dr - d) > 0.000001) return false;            //not an integer division
  if(dr < 1.0) return false;
  uint32_t total = (uint32_t) dr;

  for(uint32_t post = 1; post <= 255; post++)           //prefer the Pre-R divider, only use the Post-R divider if needed (datasheet 7.3.2)
    {
      if(total % post) continue;
      uint32_t p = total / post;
      if(p > 4095) continue;                            //PLL_R_PRE is 12 bits
      double preOut = fin / (double) p;
      if(p > 1)
        {
          if((fin < 10.0) || (fin > 1401.0)) continue;   //Pre-R input range (1400 MHz max, +1 MHz margin to match maxOsc)
          if((preOut < 5.0) || (preOut > 700.0)) continue;   //Pre-R output range
        }
      if(post > 1)
        {
          if((preOut < 5.0) || (preOut > 250.0)) continue;   //Post-R input range
          if((rpfd < 0.25) || (rpfd > 125.0)) continue;      //Post-R output range
        }
      if(rpfd > 200.0) continue;                         //dual loop PFD maximum
      *pre = p;
      *r = post;
      return true;
    }
  return false;
}

double calcPfd(double rpfd) override
{
  unsigned int pre = 1;
  byte r = 1;
  byte mult = 1;
  bool dub = 0;
  double multInFreq;
  double multOutFreq;
  double d;

  //first try a simple division...
  if(findDividers(refOsc, rpfd, &pre, &r)) goto done;

  //next try using the doubler. Doubler input must be 5 - 200 MHz.
  if(refOsc <= 200.0)
    {
      dub = 1;
      if(findDividers(refOsc * 2.0, rpfd, &pre, &r)) goto done;
      dub = 0;
    }

  // Try using the Multiplier. Can't use doubler and Multiplier together (datasheet 7.3.2).
  // Multiplier input 40 - 70 MHz, output 180 - 250 MHz, MULT = 3,4,5 or 6.
  if(refOsc >= 40.0)
    {
      for(unsigned int p = 1; p <= 4095; p++)            //try all possible values of Pre divider
        {
          multInFreq = refOsc / (double) p;
          if(multInFreq < 40.0) break;                   //stop looking if the multiplier input frequency is less than minimum
          if(multInFreq > 70.0) continue;                //try the next pre divide value if the multiplier input frequency is above maximum
          if((p > 1) && (refOsc > 1401.0)) continue;     //Pre-R input limit (1400 MHz max, +1 MHz margin to match maxOsc)

          for(byte m = 3; m <= 6; m++)                   //try all values of the multiplier
            {
              multOutFreq = multInFreq * (double) m;
              if(multOutFreq > 250.0) break;             //stop looking if the multiplier output frequency is greater than allowed
              if(multOutFreq < 180.0) continue;          //try the next Multiplier if it below the allowed output frequency
              d = multOutFreq / rpfd;                    //we now have a multOutFreq which we can try to see if the rpfd can be achieved
              double dr = floor(d + 0.5);
              if(fabs(dr - d) > 0.000001) continue;      //not an integer division
              if((dr < 1.0) || (dr > 255.0)) continue;   //PLL_R is 8 bits
              if((dr == 1.0) && (multOutFreq > 200.0)) continue;               //multiplier output direct to PFD must be within PFD max
              if((dr > 1.0) && ((rpfd > 125.0) || (rpfd < 0.25))) continue;    //Post-R output range
              mult = m;
              pre = p;
              r = (byte) dr;
              goto done;                                 //found a solution!
            }
        }
    }


  Serial.print("Unable to achieve a PFD of ");
  Serial.print(rpfd, 6);
  Serial.print(" MHz With a Ref Oscillator of ");
  Serial.print(refOsc, 6);
  Serial.println(" MHz within the LMX2582 datasheet limits");
  Serial.println("PFD has not been changed");

  return getPfd();

  done:
  LMX2582_PLL_R_PRE = pre;        //set the Pre Multiplier Divider
  LMX2582_MULT = mult;            //set the multipier
  LMX2582_PLL_R = r;              //set the post multiplier divider
  LMX2582_OSC_2X = dub;           //Reference doubler if we need it
  LMX2582_PFD_CTL = 0;            //dual loop PFD
  setCalClkDiv();

//calculate the actual PFD set and adjust the HP and LP registers to suit
  rpfd = getPfd();
  setPfdAdj(rpfd);

  Serial.print("PFD changed to ");
  Serial.print(rpfd , 6);
  Serial.println(" MHz");
  return rpfd;
}

//Set FCAL_HPFD_ADJ, FCAL_LPFD_ADJ and PLL_N_PRE for the PFD frequency (datasheet R0 description and 7.3.3)
void setPfdAdj(double pfd)
{
  if(pfd <= 100.0) LMX2582_FCAL_HPFD_ADJ = 0;
  if((pfd > 100.0) && (pfd <= 150.0))  LMX2582_FCAL_HPFD_ADJ = 1;
  if((pfd > 150.0) && (pfd <= 200.0))  LMX2582_FCAL_HPFD_ADJ = 2;
  if(pfd > 200.0)  LMX2582_FCAL_HPFD_ADJ = 3;
  if(pfd >= 10.0) LMX2582_FCAL_LPFD_ADJ = 0;
  if((pfd >= 5.0) && (pfd < 10.0))  LMX2582_FCAL_LPFD_ADJ = 1;
  if((pfd >= 2.5) && (pfd < 5.0))  LMX2582_FCAL_LPFD_ADJ = 2;
  if(pfd < 2.5)  LMX2582_FCAL_LPFD_ADJ = 3;
  LMX2582_PLL_N_PRE = (pfd < 5.0) ? 1 : 0;             //extended low PFD mode requires the /4 prescaler
}


double getPfd(void) override
{
  double pfd = refOsc;
  double r = (double) LMX2582_PLL_R * LMX2582_PLL_R_PRE ;
  if(r < 1) r = 1;
  if(LMX2582_OSC_2X == 1) pfd = pfd * 2.0;
  pfd = (pfd * LMX2582_MULT) / r;
  return pfd;
}

//Returns the N divider prescaler value (2 or 4)
double getNPre(void)
{
  return LMX2582_PLL_N_PRE ? 4.0 : 2.0;
}

//Returns the total channel divider value from the segment registers (datasheet 7.3.7)
uint16_t getChdiv(void)
{
  uint16_t d;
  d = LMX2582_CHDIV_SEG1 ? 3 : 2;
  if(LMX2582_CHDIV_SEG_SEL >= 2) d = d * segValue(LMX2582_CHDIV_SEG2);
  if(LMX2582_CHDIV_SEG_SEL >= 4) d = d * segValue(LMX2582_CHDIV_SEG3);
  return d;
}

//convert a SEG2 / SEG3 register code to its division ratio
uint16_t segValue(byte code)
{
  switch(code)
  {
    case 1: return 2;
    case 2: return 4;
    case 4: return 6;
    case 8: return 8;
    default: return 1;
  }
}

//Minimum PLL_N for the current MASH order (datasheet Table 7-2)
uint16_t minN(void)
{
  switch(LMX2582_MASH_ORDER)
  {
    case 0: return 9;
    case 1: return 11;
    case 2: return 16;
    case 3: return 18;
    default: return 30;
  }
}

void setFrequency(double direct) override
{
  bool freqOK = false;
  double pfd;
  double freq;
  double vco;
  double n;
  double part;
  double err;
  uint32_t num;
  uint32_t den;
  int chd = -1;                 //-1 = VCO direct, otherwise index into the channel divider table
  uint16_t division = 1;
  bool maxDivisor;

  if((chanData[channel].fskMode) || (chanData[channel].jtMode))           //if we are using any FSK keying then we must keep the divisor to the same value to avoid glitches
  {
    maxDivisor = true;
  }
  else
  {
    maxDivisor = false;
  }

  pfd = getPfd();

  freqOK = false;
  if(direct == 0)
   {
    while(!freqOK)
    {
      if(chanData[channel].extMult > 1)
      {
      Serial.print("\nEnter Required Final Multiplied Frequency in MHz -->");
      freq = inputFloat() / (double) chanData[channel].extMult;
      }
      else
      {
      Serial.print("\nEnter Required Frequency in MHz -->");
      freq = inputFloat();
      }
      if((freq >= 20.0) && (freq <= 5500.000))
        {
          freqOK = true;
        }
      else
        {
          Serial.println("Synthesiser Frequency must be between 20 and 5500 MHz");
        }
    }
   }
   else
   {
    freq = direct;
   }

//Calculate the required output divider (datasheet Table 7-4). Above 3550 MHz the VCO is used directly.
  if(freq < 3550.0)
    {
      for(int i = 0; i < numberOfDividers; i++)
        {
          if(freq >= (double) chdivMinOut[i])
            {
              chd = i;
              break;
            }
        }
      if(chd < 0) chd = numberOfDividers - 1;          //below 20 MHz - use the largest divider
    }

  if(chd < 0)
    {
      //VCO direct to output A, channel divider powered down
      LMX2582_OUTA_MUX = 1;
      LMX2582_VCO_DISTA_PD = 0;
      LMX2582_CHDIV_DISTA_EN = 0;
      LMX2582_CHDIV_EN = 0;
      LMX2582_CHDIV_DIST_PD = 1;
      LMX2582_CHDIV_SEG1_EN = 0;
      LMX2582_CHDIV_SEG2_EN = 0;
      LMX2582_CHDIV_SEG3_EN = 0;
      division = 1;
    }
  else
    {
      //Channel divider to output A, VCO direct buffer powered down
      LMX2582_OUTA_MUX = 0;
      LMX2582_VCO_DISTA_PD = 1;
      LMX2582_CHDIV_DISTA_EN = 1;
      LMX2582_CHDIV_EN = 1;
      LMX2582_CHDIV_DIST_PD = 0;
      LMX2582_CHDIV_SEG1 = chdivSeg1[chd];
      LMX2582_CHDIV_SEG2 = chdivSeg2[chd];
      LMX2582_CHDIV_SEG3 = chdivSeg3[chd];
      LMX2582_CHDIV_SEG_SEL = chdivSegSel[chd];
      LMX2582_CHDIV_SEG1_EN = 1;
      LMX2582_CHDIV_SEG2_EN = (chdivSegSel[chd] >= 2);
      LMX2582_CHDIV_SEG3_EN = (chdivSegSel[chd] >= 4);
      division = chdivValue[chd];
    }

//adjust the vco frequency to allow for the output division
  vco = freq * (double) division;

  LMX2582_VTUNE_ADJ = (vco >= 6500.0) ? 3 : 0;          //datasheet R30 description

  setPfdAdj(pfd);                                        //also selects the N prescaler (/2, or /4 for PFD below 5 MHz)
  setCalClkDiv();

  n = vco / (pfd * getNPre());

  if(n >= 4096.0)
    {
      Serial.println("Frequency can not be set. The N divider would exceed its maximum of 4095. Increase the PFD.");
      return;
    }

  LMX2582_PLL_N = (unsigned int) n;

  n = n - (unsigned int) n;

//n now has the desired fractional part. Now we need to work out the f/d to acheive this


 // first try to calculate the fraction up to a denominator of FFFF. This is an arbritary limit to keep the speed up.
 err = 1.0;
 uint32_t  bestnum = 0;
 uint32_t  bestden = 2;

if(! maxDivisor)
{
   for(den = 2; den <= 0xFFFF ; den++)
     {
      num = int((den * n) + 0.0000000001 );
      if(num < 1) continue;

      part = (double) num / (double) den;
      if(fabs(part-n) < 0.0000000001)
        {
          bestnum = num;
          bestden = den;
          err = 0;
          goto done;
        }
      if(fabs((part - n)) < err)
        {
            bestnum = num;
            bestden = den;
            err = fabs(part - n);
        }
     }

}
// finally try using the maximum value for the Denominator to see if that gives a smaller error than we already have.

    den = 0xFFFFFFFF;                     //using the highest number to give the finest tuning steps.
    num = (uint32_t) ((double) den * n);
    part = (double) num / (double) den;
       if(fabs((part - n)) < err)
        {
            bestnum = num;
            bestden = den;
        }

  done:
//program the fraction division
  LMX2582_PLL_DEN = bestden;
  LMX2582_PLL_NUM = bestnum;

//PFD delay recommended setting for the MASH order (datasheet Table 7-2)
  switch(LMX2582_MASH_ORDER)
  {
    case 0:
    case 1:
      LMX2582_PFD_DLY = 1;
      break;
    case 2:
    case 3:
      LMX2582_PFD_DLY = 2;
      break;
    default:
      LMX2582_PFD_DLY = 8;
      break;
  }

  encodeRegs();
}

void calcFreq(void) override
{
  double pfd;
  double vco;
  double n;
  double nf;
  double fr;
  double fout;

  uint16_t diva;

  if(LMX2582_OUTA_MUX == 0)
    {
      diva = getChdiv();
    }
  else
    {
      diva = 1;
    }


  n = (double) LMX2582_PLL_N;

  Serial.println();
  Serial.print("Chip type is ");
  Serial.println(name);
  Serial.print("Reference Oscillator = ");
  Serial.print(refOsc,10);
  Serial.println(" MHz");

  pfd = getPfd();

  Serial.print("PFD = ");
  Serial.print(pfd , 10);
  Serial.println(" MHz");

  fr = (double) LMX2582_PLL_NUM / (double) LMX2582_PLL_DEN;

  nf = (n + fr);

  vco = pfd * getNPre() * nf;

  Serial.print("N Prescaler = /");
  Serial.println((int) getNPre());

  Serial.print("N = ");
  Serial.print(nf , 10);
  Serial.print(" ( ");
  Serial.print(LMX2582_PLL_N);
  Serial.print(" + (");
  Serial.print(LMX2582_PLL_NUM);
  Serial.print("/");
  Serial.print(LMX2582_PLL_DEN);
  Serial.println(") )");

  Serial.print("VCO = ");
  Serial.print(vco , 10);
  Serial.println(" MHz");

  Serial.print("O/P Divider = /");
  Serial.println(diva);

  fout = vco / diva;

  Serial.print("Output Frequency = ");
  Serial.print(fout , 10);
  Serial.println(" MHz");

  if(chanData[channel].extMult > 1)
    {
      Serial.print("External Multipler = X");
      Serial.println(chanData[channel].extMult);
      Serial.print("Final Multiplied Frequency = ");
      Serial.print(fout * (double) chanData[channel].extMult, 10);
      Serial.println(" MHz");
    }

  //Datasheet limit checks
  if((vco < 3550.0) || (vco > 7100.0))
    {
      Serial.println("WARNING: VCO frequency is outside the 3550 - 7100 MHz datasheet range");
    }
  if((fout < 20.0) || (fout > 5500.0))
    {
      Serial.println("WARNING: Output frequency is outside the 20 - 5500 MHz datasheet range");
    }
  if((pfd < 0.25) || (pfd > 200.0))
    {
      Serial.println("WARNING: PFD is outside the 0.25 - 200 MHz range supported for fractional-N");
    }
  if(LMX2582_PLL_N < minN())
    {
      Serial.print("WARNING: N = ");
      Serial.print(LMX2582_PLL_N);
      Serial.print(" is below the datasheet minimum of ");
      Serial.print(minN());
      Serial.print(" for MASH_ORDER ");
      Serial.print(LMX2582_MASH_ORDER);
      Serial.println(". Reduce the PFD or the MASH order.");
    }
}

double getFrequency(void) override
{
  double pfd;
  double vco;
  double n;
  double nf;
  double fr;

  uint16_t diva;

  if(LMX2582_OUTA_MUX == 0)
    {
      diva = getChdiv();
    }
  else
    {
      diva = 1;
    }

  n = (double) LMX2582_PLL_N;

  pfd = getPfd();

  fr = (double) LMX2582_PLL_NUM / (double) LMX2582_PLL_DEN;

  nf = (n + fr);

  vco = pfd * getNPre() * nf;

  return vco / diva;
}

//Frequency change sequence follows the datasheet 7.5.2 order: N (R38), then numerator (R45, R44), then denominator (R41, R40).
void jtShift(uint8_t val) override
{
  static uint8_t lastval;
  static uint32_t lastN;

  if(val == lastval) return;
  lastval = val;

  if(lastN != jtN[val])
   {
    LMX2582FastSendOne(nReg(jtN[val]));
    lastN = jtN[val];
   }

  LMX2582FastSend((45 << 16) | (jtNum[val] & 0xFFFF) ,(44 << 16) | ((jtNum[val] >> 16) & 0xFFFF));
}

void fskKey(bool key) override
{
  static bool lastkey;

  if(key == lastkey) return;
  lastkey = key;

  if(key)
    {
      LMX2582FastSendOne(chanData[channel].reg[38]);
      LMX2582FastSend(chanData[channel].reg[45] ,chanData[channel].reg[44]);
      LMX2582FastSend(chanData[channel].reg[41] ,chanData[channel].reg[40]);
    }
  else
    {
        LMX2582FastSendOne(nReg(cwidKeyUpN));
        LMX2582FastSend((45 << 16) | (cwidKeyUpNum & 0xFFFF) ,(44 << 16) | ((cwidKeyUpNum >> 16) & 0xFFFF));
        LMX2582FastSend((41 << 16) | (cwidKeyUpDen & 0xFFFF) ,(40 << 16) | ((cwidKeyUpDen >> 16) & 0xFFFF));
    }
}

void extKey(bool key) override
{
  if(key)
    {
      LMX2582FastSendOne(chanData[channel].reg[38]);
      LMX2582FastSend(chanData[channel].reg[45] ,chanData[channel].reg[44]);
      LMX2582FastSend(chanData[channel].reg[41] ,chanData[channel].reg[40]);
    }
  else
    {
      LMX2582FastSendOne(nReg(ExtKeyUpN));
      LMX2582FastSend((45 << 16) | (ExtKeyUpNum & 0xFFFF) ,(44 << 16) | ((ExtKeyUpNum >> 16) & 0xFFFF));
      LMX2582FastSend((41 << 16) | (ExtKeyUpDen & 0xFFFF) ,(40 << 16) | ((ExtKeyUpDen >> 16) & 0xFFFF));
    }
}

void saveFskShift(void) override
{
  cwidKeyUpN = LMX2582_PLL_N;
  cwidKeyUpDen = LMX2582_PLL_DEN;
  cwidKeyUpNum = LMX2582_PLL_NUM;
}

void saveKeyShift(void) override
{
  ExtKeyUpN = LMX2582_PLL_N;
  ExtKeyUpDen = LMX2582_PLL_DEN;
  ExtKeyUpNum = LMX2582_PLL_NUM;
}

void saveJt(uint8_t index) override
{
  jtN[index] = LMX2582_PLL_N;
  jtDen[index] = LMX2582_PLL_DEN;
  jtNum[index] = LMX2582_PLL_NUM;
}

//--- RF output power/enable control, used by the touchscreen UI ---
//OUTA_POW valid values are 0 - 31 and 48 - 63 (extra boost). 32 - 47 are not valid (datasheet R46 description),
//so a request in that gap jumps to the next valid value in the direction of travel.
uint8_t getPower(void) override { return LMX2582_OUTA_POW; }
void setPower(uint8_t p) override
{
  if(p > 63) p = 63;
  if((p > 31) && (p < 48))
    {
      if(LMX2582_OUTA_POW <= 31) p = 48; else p = 31;
    }
  LMX2582_OUTA_POW = p;
  encodeRegs();
  update();
}
bool getOutput(void) override { return !LMX2582_OUTA_PD; }
void enableOutput(bool o) override
{
  digitalWrite(LMX2582CEPin,o);
  LMX2582_OUTA_PD = !o;
  encodeRegs();
  update();
}

};

LMX2582Chip lmx2582Chip;             //the single instance of this chip driver
