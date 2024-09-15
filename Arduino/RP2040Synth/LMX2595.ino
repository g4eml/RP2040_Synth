//Chip specific functions for LMX2595 device

#define LMX2595CEPin 3         //GPO 3 Connect to LMX2595  CE Pin
#define LMX2595MUXPin 4         //GPO 4 Connect to LMX2595  MUXout Pin
#define LMX2595CSBPin 5         //GPO 5 Connect to LMX2595  CSB Pin
#define LMX2595SCKPin 6         //GPO 6 Connect to LMX2595  SCK Pin 
#define LMX2595SDIPin 7         //GPO 7 Connect to LMX2595  SDI Pin


//LMX2595 Register Parameters.

//Reg 0
bool LMX2595_RAMP_EN;
bool LMX2595_VCO_PHASE_SYNC;
bool LMX2595_OUT_MUTE;
byte LMX2595_FCAL_HPFD_ADJ;
byte LMX2595_FCAL_LPFD_ADJ;
bool LMX2595_FCAL_EN;
bool LMX2595_MUXOUT_LD_SEL;
bool LMX2595_RESET;
bool LMX2595_POWERDOWN;
//Reg 1
byte LMX2595_CAL_CLK_DIV;
//Reg 4
byte LMX2595_ACAL_CMP_DLY;
//Reg 7
bool LMX2595_OUT_FORCE;
//Reg 8
bool LMX2595_VCO_DACISET_FORCE;
bool LMX2595_VCO_CAPCTRL_FORCE;
//Reg 9
bool LMX2595_OSC_2X;
//Reg 10
byte LMX2595_MULT;
//Reg 11
byte LMX2595_PLL_R;
//Reg 12
unsigned int LMX2595_PLL_R_PRE;
//Reg 14
byte LMX2595_CPG;
//Reg 16
unsigned int LMX2595_VCO_DACISET;
//Reg 17
unsigned int LMX2595_VCO_DACISET_STRT;
//Reg 19
byte LMX2595_VCO_CAPCTRL;
//Reg 20
byte LMX2595_VCO_SEL;
bool LMX2595_VCO_SEL_FORCE;
//Reg 25
unsigned int LMX2595_DBLR_IBIAS_CTRL1;
//Reg 27
bool LMX2595_VCO2X_EN;
//Reg 31
bool LMX2595_SEG1_EN;
//Reg 34
byte LMX2595_PLL_NH;                //High 3 bits
//Reg 36
unsigned int LMX2595_PLL_NL;       //Low 16 bits
//combined 19 bit variable for conveniance 
uint32_t LMX2595_PLL_N;
//Reg 37
bool LMX2595_MASH_SEED_EN;
byte LMX2595_PFD_DLY_SEL;
//Reg 38
unsigned int LMX2595_PLL_DENH;        //High 16 bits
//Reg 39
unsigned int LMX2595_PLL_DENL;        //Low 16 bits
//combined 32 bit variable for conveniance 
uint32_t LMX2595_PLL_DEN;
//Reg 40
unsigned int LMX2595_MASH_SEEDH;        //High 16 bits
//Reg 41
unsigned int LMX2595_MASH_SEEDL;        //Low 16 bits
//combined 32 bit variable for conveniance 
uint32_t LMX2595_MASH_SEED;
//Reg 42
unsigned int LMX2595_PLL_NUMH;        //High 16 bits
//Reg 43
unsigned int LMX2595_PLL_NUML;        //Low 16 bits
//combined 32 bit variable for conveniance 
uint32_t LMX2595_PLL_NUM;
//Reg 44
byte LMX2595_OUTA_PWR;
bool LMX2595_OUTB_PD;
bool LMX2595_OUTA_PD;
bool LMX2595_MASH_RESET_N;
byte LMX2595_MASH_ORDER;
//Reg 45
byte LMX2595_OUTA_MUX;
byte LMX2595_OUT_ISET;
byte LMX2595_OUTB_PWR;
//Reg 46
byte LMX2595_OUTB_MUX;
//Reg 58
bool LMX2595_INPIN_IGNORE;
bool LMX2595_INPIN_HYST;
byte LMX2595_INPIN_LVL;
byte LMX2595_INPIN_FMT;
//Reg 59
bool LMX2595_LD_TYPE;
//Reg 60
unsigned int LMX2595_LD_DLY;
//Reg 69
unsigned int LMX2595_MASH_RST_COUNTH;       //High 16 bits
//Reg 70
unsigned int LMX2595_MASH_RST_COUNTL;       //Low 16 bits
//combined 32 bit variable for conveniance 
uint32_t LMX2595_MASH_RST_COUNT;
//Reg 71
byte LMX2595_SYSREF_DIV_PRE;
bool LMX2595_SYSREF_PULSE;
bool LMX2595_SYSREF_EN;
bool LMX2595_SYSREF_REPEAT;
//Reg 72
unsigned int LMX2595_SYSREF_DIV;
//Reg 73
byte LMX2595_JESD_DAC2_CTRL;
byte LMX2595_JESD_DAC1_CTRL;
//Reg 74
byte LMX2595_SYSREF_PULSE_CNT;
byte LMX2595_JESD_DAC4_CTRL;
byte LMX2595_JESD_DAC3_CTRL;
//Reg 75
byte LMX2595_CHDIV;
//Reg 78
bool LMX2595_RAMP_THRESH32;
bool LMX2595_QUICK_RECAL_EN;
byte LMX2595_VCO_CAPCTRL_STRT;

//Ramping and readback Registers R79 - R112 not used so not included. 




void LMX2595SetDefault(void)
{
//Reg 0
LMX2595_RAMP_EN = 0;
LMX2595_VCO_PHASE_SYNC = 0;
LMX2595_OUT_MUTE = 0;
LMX2595_FCAL_HPFD_ADJ = 0;
LMX2595_FCAL_LPFD_ADJ = 0;
LMX2595_FCAL_EN = 0;
LMX2595_MUXOUT_LD_SEL = 1;
LMX2595_RESET = 0;
LMX2595_POWERDOWN =0;
//Reg 1
LMX2595_CAL_CLK_DIV = 0;
//Reg 4
LMX2595_ACAL_CMP_DLY = 25;
//Reg 7
LMX2595_OUT_FORCE = 1;
//Reg 8
LMX2595_VCO_DACISET_FORCE = 0;
LMX2595_VCO_CAPCTRL_FORCE = 0;
//Reg 9
LMX2595_OSC_2X = 0;
//Reg 10
LMX2595_MULT = 1;
//Reg 11
LMX2595_PLL_R = 1;
//Reg 12
LMX2595_PLL_R_PRE = 1;
//Reg 14
LMX2595_CPG = 4;
//Reg 16
LMX2595_VCO_DACISET = 128;
//Reg 17
LMX2595_VCO_DACISET_STRT = 300;
//Reg 19
LMX2595_VCO_CAPCTRL = 183;
//Reg 20
LMX2595_VCO_SEL = 4;
LMX2595_VCO_SEL_FORCE = 0;
//Reg 25
LMX2595_DBLR_IBIAS_CTRL1 = 3115;
//Reg 27
LMX2595_VCO2X_EN = 0;
//Reg 31
LMX2595_SEG1_EN = 1;
//Reg 34
LMX2595_PLL_NH = 0;                //High 3 bits
//Reg 36
LMX2595_PLL_NL = 100;             //Low 16 bits
//combined 19 bit variable for conveniance 
LMX2595_PLL_N = 100;
//Reg 37
LMX2595_MASH_SEED_EN = 0;
LMX2595_PFD_DLY_SEL = 2;
//Reg 38
LMX2595_PLL_DENH = 65535;        //High 16 bits
//Reg 39
LMX2595_PLL_DENL = 65535 ;        //Low 16 bits
//combined 32 bit variable for conveniance 
LMX2595_PLL_DEN = 4294967295;     //0xFFFFFFFF max value fo rfinest resolution. 
//Reg 40
LMX2595_MASH_SEEDH = 0;        //High 16 bits
//Reg 41
LMX2595_MASH_SEEDL = 0;        //Low 16 bits
//combined 32 bit variable for conveniance 
LMX2595_MASH_SEED = 0;
//Reg 42
LMX2595_PLL_NUMH = 0;        //High 16 bits
//Reg 43
LMX2595_PLL_NUML = 0;        //Low 16 bits
//combined 32 bit variable for conveniance 
LMX2595_PLL_NUM = 0;
//Reg 44
LMX2595_OUTA_PWR = 31;
LMX2595_OUTB_PD = 1;
LMX2595_OUTA_PD = 0;
LMX2595_MASH_RESET_N = 1;
LMX2595_MASH_ORDER = 3;
//Reg 45
LMX2595_OUTA_MUX = 0;
LMX2595_OUT_ISET = 0;
LMX2595_OUTB_PWR = 31;
//Reg 46
LMX2595_OUTB_MUX = 0;
//Reg 58
LMX2595_INPIN_IGNORE = 1;
LMX2595_INPIN_HYST = 0;
LMX2595_INPIN_LVL = 1;
LMX2595_INPIN_FMT = 0;
//Reg 59
LMX2595_LD_TYPE = 1;
//Reg 60
LMX2595_LD_DLY = 1000;
//Reg 69
LMX2595_MASH_RST_COUNTH = 0;       //High 16 bits
//Reg 70
LMX2595_MASH_RST_COUNTL = 0;       //Low 16 bits
//combined 32 bit variable for conveniance 
LMX2595_MASH_RST_COUNT = 0;
//Reg 71
LMX2595_SYSREF_DIV_PRE = 4;
LMX2595_SYSREF_PULSE = 0;
LMX2595_SYSREF_EN = 0;
LMX2595_SYSREF_REPEAT = 0;
//Reg 72
LMX2595_SYSREF_DIV = 0;
//Reg 73
LMX2595_JESD_DAC2_CTRL = 0;
LMX2595_JESD_DAC1_CTRL = 63;
//Reg 74
LMX2595_SYSREF_PULSE_CNT = 0;
LMX2595_JESD_DAC4_CTRL = 0;
LMX2595_JESD_DAC3_CTRL = 0;
//Reg 75
LMX2595_CHDIV = 2;
//Reg 78
LMX2595_RAMP_THRESH32 = 0;
LMX2595_QUICK_RECAL_EN = 0;
LMX2595_VCO_CAPCTRL_STRT = 0;



  LMX2595SetFrequency(0);

  LMX2595EncodeRegs();
  LMX2595Update();
}

void LMX2595SetParameters(void)
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
        Serial.println("Enter blank line to exit");
        Serial.println("Valid Parameter Names (See LMX2595 Data Sheet for details) are:-");
        Serial.println("Reg 0 = RAMP_EN VCO_PHASE_SYNC LMX2595_OUT_MUTE FCAL_HPFD_ADJ FCAL_LPFD_ADJ FCAL_EN MUXOUT_LD_SEL RESET POWERDOWN");
        Serial.println("Reg 1 = CAL_CLK_DIV");  
        Serial.println("Reg 4 = ACAL_CMP_DLY");  
        Serial.println("Reg 7 = OUT_FORCE"); 
        Serial.println("Reg 8 = VCO_DACISET_FORCE VCO_CAPCTRL_FORCE");
        Serial.println("Reg 9 = OSC_2X");  
        Serial.println("Reg 10 = MULT"); 
        Serial.println("Reg 11 = PLL_R");  
        Serial.println("Reg 12 = PLL_R_PRE");
        Serial.println("Reg 14 = CPG"); 
        Serial.println("Reg 16 = VCO_DACISET"); 
        Serial.println("Reg 17 = VCO_DACISET_STRT"); 
        Serial.println("Reg 19 = VCO_CAPCTRL");  
        Serial.println("Reg 20 = VCO_SEL VCO_SEL_FORCE ");
        Serial.println("Reg 25 = DBLR_IBIAS_CTRL1"); 
        Serial.println("Reg 27 = VCO2X_EN"); 
        Serial.println("Reg 31 = SEG1_EN");
        Serial.println("Reg 34/36 = PLL_N");
        Serial.println("Reg 37 = MASH_SEED_EN PFD_DLY_SEL"); 
        Serial.println("Reg 38/39 = PLL_DEN");  
        Serial.println("Reg 40/41 = MASH_SEED"); 
        Serial.println("Reg 42/43 = PLL_NUM");
        Serial.println("Reg 44 = OUTA_PWR OUTB_PD OUTA_PD MASH_RESET_N MASH_ORDER");
        Serial.println("Reg 45 = OUTA_MUX OUT_ISET OUTB_PWR");
        Serial.println("Reg 46 = OUTB_MUX"); 
        Serial.println("Reg 58 = INPIN_IGNORE INPIN_HYST INPIN_LVL INPIN_FMT"); 
        Serial.println("Reg 59 = LD_TYPE"); 
        Serial.println("Reg 60 = LD_DLY");
        Serial.println("Reg 69/70 = MASH_RST_COUNT");
        Serial.println("Reg 71 = SYSREF_DIV_PRE SYSREF_PULSE SYSREF_EN SYSREF_REPEAT");
        Serial.println("Reg 72 = SYSREF_DIV");
        Serial.println("Reg 73 = JESD_DAC2_CTRL JESD_DAC1_CTRL"); 
        Serial.println("Reg 74 = SYSREF_PULSE_CNT JESD_DAC4_CTRL JESD_DAC3_CTRL");
        Serial.println("Reg 75 = CHDIV"); 
        Serial.println("Reg 78 = RAMP_THRESH32 QUICK_RECAL_EN VCO_CAPCTRL_STRT");
        Serial.println("Ramping and readback registers R79 - R112 are not used or supported.");

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
      if(paramBool(param , "RAMP_EN" , &LMX2595_RAMP_EN , value)) goto handled;
      if(paramBool(param , "VCO_PHASE_SYNC" , &LMX2595_VCO_PHASE_SYNC , value)) goto handled;
      if(paramBool(param , "OUT_MUTE" , &LMX2595_OUT_MUTE , value)) goto handled;
      if(paramByte(param , "FCAL_HPFD_ADJ" , &LMX2595_FCAL_HPFD_ADJ , value , 0 , 3)) goto handled;
      if(paramByte(param , "FCAL_LPFD_ADJ" , &LMX2595_FCAL_LPFD_ADJ , value , 0 , 3)) goto handled;
      if(paramBool(param , "FCAL_EN" , &LMX2595_FCAL_EN , value)) goto handled;
      if(paramBool(param , "MUX_OUT_LD_SEL" , &LMX2595_MUXOUT_LD_SEL , value)) goto handled;
      if(paramBool(param , "RESET" , &LMX2595_RESET , value)) goto handled;
      if(paramBool(param , "POWERDOWN" , &LMX2595_POWERDOWN , value)) goto handled;
//Reg 1
      if(paramByte(param , "CAL_CLK_DIV" , &LMX2595_CAL_CLK_DIV , value , 0 , 3)) goto handled;
//Reg 4
      if(paramByte(param , "ACAL_CMP_DELAY" , &LMX2595_ACAL_CMP_DLY , value , 0 , 255)) goto handled;
//Reg 7
      if(paramBool(param , "OUT_FORCE" , &LMX2595_OUT_FORCE , value)) goto handled;
//Reg 8
      if(paramBool(param , "VCO_DACISET_FORCE" , &LMX2595_VCO_DACISET_FORCE , value)) goto handled;
      if(paramBool(param , "VCO_CAPCTRL_FORCE" , &LMX2595_VCO_CAPCTRL_FORCE , value)) goto handled;
//Reg 9
      if(paramBool(param , "OSC_2X" , &LMX2595_OSC_2X , value)) goto handled;
//Reg 10
      if(paramByte(param , "MULT" , &LMX2595_MULT , value , 1 , 7)) goto handled;
//Reg 11
      if(paramByte(param , "PLL_R" , &LMX2595_PLL_R , value , 0 , 255)) goto handled;
//Reg 12
      if(paramUint(param , "PLL_R_PRE" , &LMX2595_PLL_R_PRE , value , 0 , 4095)) goto handled;
//Reg 14
      if(paramByte(param , "CPG" , &LMX2595_CPG , value , 0 , 7)) goto handled;
//Reg 16
      if(paramUint(param , "VCO_DACISET" , &LMX2595_VCO_DACISET , value , 0 , 512)) goto handled;
//Reg 17
      if(paramUint(param , "VCo_DACISET_STRT" , &LMX2595_VCO_DACISET_STRT , value , 0 , 512)) goto handled;
//Reg 19
      if(paramByte(param , "VCO_CAP_CTRL" , &LMX2595_VCO_CAPCTRL , value , 0 , 255)) goto handled;
//Reg 20
      if(paramByte(param , "VCO_SEL" , &LMX2595_VCO_SEL , value , 0 , 7)) goto handled;
      if(paramBool(param , "VCO_SEL_FORCE" , &LMX2595_VCO_SEL_FORCE , value)) goto handled;
//Reg 25
      if(paramUint(param , "DBLR_IBIAS_CTRL1" , &LMX2595_DBLR_IBIAS_CTRL1 , value , 0 , 65535)) goto handled;
//Reg 27
      if(paramBool(param , "VCO2X_EN" , &LMX2595_VCO2X_EN , value)) goto handled;
//Reg 31
      if(paramBool(param , "SEG1_EN" , &LMX2595_SEG1_EN , value)) goto handled;
//Reg 34
//Reg 36
//combined 19 bit variable for conveniance 
      if(paramUint32(param , "PLL_N" , &LMX2595_PLL_N , value , 0 , 0x7FFFF)) goto handled;
//Reg 37
      if(paramBool(param , "MASH_SEED_EN" , &LMX2595_MASH_SEED_EN , value)) goto handled;
      if(paramByte(param , "PFD_DLY_SEL" , &LMX2595_PFD_DLY_SEL , value , 0 , 64)) goto handled;
//Reg 38
//Reg 39       //Low 16 bits
//combined 32 bit variable for conveniance 
      if(paramUint32(param , "PLL_DEN" , &LMX2595_PLL_DEN , value , 0 , 0xFFFFFFFF)) goto handled; 
//Reg 40
//Reg 41
//combined 32 bit variable for conveniance 
      if(paramUint32(param , "MASH_SEED" , &LMX2595_MASH_SEED , value , 0 , 0xFFFFFFFF)) goto handled;
//Reg 42
//Reg 43
//combined 32 bit variable for conveniance 
      if(paramUint32(param , "PLL_NUM" , &LMX2595_PLL_NUM , value , 0 , 0xFFFFFFFF)) goto handled;
//Reg 44
      if(paramByte(param , "OUTA_PWR" , &LMX2595_OUTA_PWR , value , 0 , 63)) goto handled;
      if(paramBool(param , "OUTB_PD" , &LMX2595_OUTB_PD , value)) goto handled;
      if(paramBool(param , "OUTA_PD" , &LMX2595_OUTA_PD , value)) goto handled;
      if(paramBool(param , "MASH_RESET_N" , &LMX2595_MASH_RESET_N , value)) goto handled;
      if(paramByte(param , "MASH_ORDER" , &LMX2595_MASH_ORDER , value , 0 , 4)) goto handled;
//Reg 45
      if(paramByte(param , "OUTA_MUX" , &LMX2595_OUTA_MUX , value , 0 , 3)) goto handled;
      if(paramByte(param , "OUT_ISET" , &LMX2595_OUT_ISET , value , 0 , 3)) goto handled;
      if(paramByte(param , "OUTB_PWR" , &LMX2595_OUTB_PWR , value , 0 , 63)) goto handled;
//Reg 46
      if(paramByte(param , "OUTB_MUX" , &LMX2595_OUTB_MUX , value , 0 , 3)) goto handled;
//Reg 58
      if(paramBool(param , "INPIN_IGNORE" , &LMX2595_INPIN_IGNORE , value)) goto handled;
      if(paramBool(param , "INPIN_HYST" , &LMX2595_INPIN_HYST , value)) goto handled;
      if(paramByte(param , "INPIN_LVL" , &LMX2595_INPIN_LVL , value , 0 , 3)) goto handled;
      if(paramByte(param , "INPIN_FMT" , &LMX2595_INPIN_FMT , value , 0 , 7)) goto handled;
//Reg 59
      if(paramBool(param , "LD_TYPE" , &LMX2595_LD_TYPE , value)) goto handled;
//Reg 60
      if(paramUint(param , "LD_DLY" , &LMX2595_LD_DLY , value , 0 , 65535)) goto handled;
//Reg 69
//Reg 70    
//combined 32 bit variable for conveniance 
      if(paramUint32(param , "MASH_RST_COUNT" , &LMX2595_MASH_RST_COUNT , value , 0 , 0xFFFFFFFF))  goto handled;
//Reg 71
      if(paramByte(param , "SYSREF_DIV_PRE" , &LMX2595_SYSREF_DIV_PRE , value , 1 , 4)) goto handled;
      if(paramBool(param , "SYSREF_PULSE" , &LMX2595_SYSREF_PULSE , value)) goto handled;
      if(paramBool(param , "SYSREF_EN" , &LMX2595_SYSREF_EN , value)) goto handled;
      if(paramBool(param , "SYSREF_REPEAT" , &LMX2595_SYSREF_REPEAT , value)) goto handled;
//Reg 72
      if(paramUint(param , "SYSREF_DIV" , &LMX2595_SYSREF_DIV , value , 0 , 2047)) goto handled;
//Reg 73
      if(paramByte(param , "JESD_DAC2_CTRL" , &LMX2595_JESD_DAC2_CTRL , value , 0 , 63)) goto handled;
      if(paramByte(param , "JESD_DAC1_CTRL" , &LMX2595_JESD_DAC1_CTRL , value , 0 , 63)) goto handled;
//Reg 74
      if(paramByte(param , "SYSREF_PULSE_CNT" , &LMX2595_SYSREF_PULSE_CNT , value , 0 , 15)) goto handled;
      if(paramByte(param , "JESD_DAC4_CTRL" , &LMX2595_JESD_DAC4_CTRL , value , 0 , 63)) goto handled;
      if(paramByte(param , "JESD_DAC3_CTRL" , &LMX2595_JESD_DAC3_CTRL , value , 0 , 63)) goto handled;
//Reg 75
      if(paramByte(param , "CHDIV" , &LMX2595_CHDIV , value , 0 , 17)) goto handled;
//Reg 78
      if(paramBool(param , "RAMP_THRESH32" , &LMX2595_RAMP_THRESH32 , value)) goto handled;
      if(paramBool(param , "QUICK_RECAL_EN" , &LMX2595_QUICK_RECAL_EN , value)) goto handled;
      if(paramByte(param , "VCO_CAPCTRL_STRT" , &LMX2595_VCO_CAPCTRL_STRT , value , 0 , 255)) goto handled;

    handled:
    LMX2595EncodeRegs();
    LMX2595Update();

  }
}

void LMX2595Init(void)
{
  int chip = LMX2595;                  //index to the current chip type
  eeprom.numberOfRegs = 79;                   //number of registers in the current chip type (ramping and readback registers 79 - 112 not used. )
  eeprom.numberOfBits = 24;                   //number of bits in each register. Top 8 bits are register address low 16 bits are data. 
  eeprom.maxPfd = 250.0;
  pinMode(LMX2595CEPin,OUTPUT);
  digitalWrite(LMX2595CEPin,HIGH); 
  pinMode(LMX2595CSBPin,OUTPUT);
  digitalWrite(LMX2595CSBPin,HIGH);
  SPI.setRX(LMX2595MUXPin);
  SPI.setTX(LMX2595SDIPin);
  SPI.setSCK(LMX2595SCKPin);
  SPI.beginTransaction(SPISettings(400000,MSBFIRST,SPI_MODE0));
  SPI.begin();
}

void LMX2595Send(int32_t val)
{
  int a=(val >> 16) & 0xFF;
  int b=(val >> 8) & 0xFF;
  int c=val & 0xFF;
  
  digitalWrite(LMX2595CSBPin,LOW);
  delayMicroseconds(10);
  SPI.transfer((byte)a);
  SPI.transfer((byte)b);
  SPI.transfer((byte)c);
  delayMicroseconds(10);
  digitalWrite(LMX2595CSBPin,HIGH);
  delayMicroseconds(10);  
}


void LMX2595EncodeRegs(void)
{
  // first we initialise the address field of all the registers. 
  for(int r = 0 ;r < eeprom.numberOfRegs ; r++)
    {
      eeprom.reg[r] = r << 16;
    }
  
  //next we map the parameters into the registers. Most registera also have additionla fixed bit patterns defined in the datasheet. 
  eeprom.reg[0] = (eeprom.reg[0]) | (LMX2595_RAMP_EN << 15) | (LMX2595_VCO_PHASE_SYNC << 14) | (0b1001 << 10) | (LMX2595_OUT_MUTE << 9) | (LMX2595_FCAL_HPFD_ADJ << 7) | (LMX2595_FCAL_LPFD_ADJ << 5) | (1 << 4) | (LMX2595_FCAL_EN << 3) | (LMX2595_MUXOUT_LD_SEL << 2) | (LMX2595_RESET << 1) | (LMX2595_POWERDOWN);
  eeprom.reg[1] = (eeprom.reg[1]) | (0b100000001 << 3) | (LMX2595_CAL_CLK_DIV);
  eeprom.reg[2] = (eeprom.reg[2]) | (0b101 << 8);
  eeprom.reg[3] = (eeprom.reg[3]) | (0b11001000010);
  eeprom.reg[4] = (eeprom.reg[4]) | (LMX2595_ACAL_CMP_DLY << 8) | (0b1000011);
  eeprom.reg[5] = (eeprom.reg[5]) | (0b11001 << 3);
  eeprom.reg[6] = (eeprom.reg[6]) | (0b1100100000000010);
  eeprom.reg[7] = (eeprom.reg[7]) | (LMX2595_OUT_FORCE << 14) | (0b10110010);
  eeprom.reg[8] = (eeprom.reg[8]) | (LMX2595_VCO_DACISET_FORCE <<14) | ( 1 << 13) | (LMX2595_VCO_CAPCTRL_FORCE << 11);
  eeprom.reg[9] = (eeprom.reg[9]) | (LMX2595_OSC_2X << 12) | (0b11000000100);
  eeprom.reg[10] = (eeprom.reg[10]) | (1 << 12) | (LMX2595_MULT << 7) | (0b1011000);
  eeprom.reg[11] = (eeprom.reg[11]) | (LMX2595_PLL_R << 4) | (0b1000);
  eeprom.reg[12] = (eeprom.reg[12]) | (0b101 << 12) | (LMX2595_PLL_R_PRE);
  eeprom.reg[13] = (eeprom.reg[13]) | (1 << 14);
  eeprom.reg[14] = (eeprom.reg[14]) | (0b1111 << 9) | (LMX2595_CPG << 4);
  eeprom.reg[15] = (eeprom.reg[15]) | (0b11001001111); 
  eeprom.reg[16] = (eeprom.reg[16]) | (LMX2595_VCO_DACISET);
  eeprom.reg[17] = (eeprom.reg[17]) | (LMX2595_VCO_DACISET_STRT);
  eeprom.reg[18] = (eeprom.reg[18]) | (0b1100100); 
  eeprom.reg[19] = (eeprom.reg[19]) | (0b100111 << 8) | (LMX2595_VCO_CAPCTRL); 
  eeprom.reg[20] = (eeprom.reg[20]) | (0b11 << 14) | (LMX2595_VCO_SEL << 11) | (LMX2595_VCO_SEL_FORCE <<10) | (0b1001000); 
  eeprom.reg[21] = (eeprom.reg[21]) | (0b10000000001); 
  eeprom.reg[22] = (eeprom.reg[22]) | (0b1);
  eeprom.reg[23] = (eeprom.reg[23]) | (0b1111100);
  eeprom.reg[24] = (eeprom.reg[24]) | (0b11100011010);
  eeprom.reg[25] = (eeprom.reg[25]) | (LMX2595_DBLR_IBIAS_CTRL1);
  eeprom.reg[26] = (eeprom.reg[26]) | (0b110110110000); 
  eeprom.reg[27] = (eeprom.reg[27]) | (1 << 1) | (LMX2595_VCO2X_EN); 
  eeprom.reg[28] = (eeprom.reg[28]) | (0b10010001000); 
  eeprom.reg[29] = (eeprom.reg[29]) | (0b11000110001100); 
  eeprom.reg[30] = (eeprom.reg[30]) | (0b11000110001100); 
  eeprom.reg[31] = (eeprom.reg[31]) | (LMX2595_SEG1_EN << 14) | (0b1111101100);
  eeprom.reg[32] = (eeprom.reg[32]) | (0b1110010011); 
  eeprom.reg[33] = (eeprom.reg[33]) | (0b1111000100001);
  //split 32 bit variable into two parts
  LMX2595_PLL_NH = (LMX2595_PLL_N >> 16) & 0x7;
  LMX2595_PLL_NL = LMX2595_PLL_N & 0xFFFF;
  eeprom.reg[34] = (eeprom.reg[34]) | (LMX2595_PLL_NH);
  eeprom.reg[35] = (eeprom.reg[35]) | (0b100); 
  eeprom.reg[36] = (eeprom.reg[36]) | (LMX2595_PLL_NL); 
  eeprom.reg[37] = (eeprom.reg[37]) | (LMX2595_MASH_SEED_EN << 15) | (LMX2595_PFD_DLY_SEL << 8) | (0b100);
  //split 32 bit variable into two parts
  LMX2595_PLL_DENH = (LMX2595_PLL_DEN >> 16) & 0xFFFF;
  LMX2595_PLL_DENL = LMX2595_PLL_DEN & 0xFFFF; 
  eeprom.reg[38] = (eeprom.reg[38]) | (LMX2595_PLL_DENH);
  eeprom.reg[39] = (eeprom.reg[39]) | (LMX2595_PLL_DENL); 
  //split 32 bit variable into two parts
  LMX2595_MASH_SEEDH = (LMX2595_MASH_SEED >> 16) & 0xFFFF;
  LMX2595_MASH_SEEDL = LMX2595_MASH_SEED & 0xFFFF; 
  eeprom.reg[40] = (eeprom.reg[40]) | (LMX2595_MASH_SEEDH);
  eeprom.reg[41] = (eeprom.reg[41]) | (LMX2595_MASH_SEEDL);  
  //split 32 bit variable into two parts
  LMX2595_PLL_NUMH = (LMX2595_PLL_NUM >> 16) & 0xFFFF;
  LMX2595_PLL_NUML = LMX2595_PLL_NUM & 0xFFFF; 
  eeprom.reg[42] = (eeprom.reg[42]) | (LMX2595_PLL_NUMH);
  eeprom.reg[43] = (eeprom.reg[43]) | (LMX2595_PLL_NUML); 
  eeprom.reg[44] = (eeprom.reg[44]) | (LMX2595_OUTA_PWR << 8) | (LMX2595_OUTB_PD << 7) | (LMX2595_OUTA_PD << 6) | (LMX2595_MASH_RESET_N << 5) | (LMX2595_MASH_ORDER);
  eeprom.reg[45] = (eeprom.reg[45]) | (0b11 <<14) | (LMX2595_OUTA_MUX << 11) | (LMX2595_OUT_ISET << 9) | (0b11 <<6) | (LMX2595_OUTB_PWR);
  eeprom.reg[46] = (eeprom.reg[46]) | (0b111111111 <<2) | (LMX2595_OUTB_MUX);
  eeprom.reg[47] = (eeprom.reg[47]) | (0b1100000000);
  eeprom.reg[48] = (eeprom.reg[48]) | (0b1100000000); 
  eeprom.reg[49] = (eeprom.reg[49]) | (0b100000110000000); 
  eeprom.reg[50] = (eeprom.reg[50]) | (0b0); 
  eeprom.reg[51] = (eeprom.reg[51]) | (0b10000000); 
  eeprom.reg[52] = (eeprom.reg[52]) | (0b100000100000); 
  eeprom.reg[53] = (eeprom.reg[53]) | (0b0); 
  eeprom.reg[54] = (eeprom.reg[54]) | (0b0); 
  eeprom.reg[55] = (eeprom.reg[55]) | (0b0); 
  eeprom.reg[56] = (eeprom.reg[56]) | (0b0); 
  eeprom.reg[57] = (eeprom.reg[57]) | (0b100000); 
  eeprom.reg[58] = (eeprom.reg[58]) | (LMX2595_INPIN_IGNORE << 15) | (LMX2595_INPIN_HYST << 14) | (LMX2595_INPIN_LVL << 12) | (LMX2595_INPIN_FMT << 9) | (0b1); 
  eeprom.reg[59] = (eeprom.reg[59]) | (LMX2595_LD_TYPE);
  eeprom.reg[60] = (eeprom.reg[60]) | (LMX2595_LD_DLY);
  eeprom.reg[61] = (eeprom.reg[61]) | (0b10101000); 
  eeprom.reg[62] = (eeprom.reg[62]) | (0b1100100010); 
  eeprom.reg[63] = (eeprom.reg[63]) | (0b0); 
  eeprom.reg[64] = (eeprom.reg[64]) | (0b1001110001000); 
  eeprom.reg[65] = (eeprom.reg[65]) | (0b0); 
  eeprom.reg[66] = (eeprom.reg[66]) | (0b111110100); 
  eeprom.reg[67] = (eeprom.reg[67]) | (0b0); 
  eeprom.reg[68] = (eeprom.reg[68]) | (0b1111101000); 
  //split 32 bit variable into two parts
  LMX2595_MASH_RST_COUNTH = (LMX2595_MASH_RST_COUNT >> 16) & 0xFFFF;
  LMX2595_MASH_RST_COUNTL = LMX2595_MASH_RST_COUNT & 0xFFFF; 
  eeprom.reg[69] = (eeprom.reg[69]) | (LMX2595_MASH_RST_COUNTH);
  eeprom.reg[70] = (eeprom.reg[70]) | (LMX2595_MASH_RST_COUNTL); 
  eeprom.reg[71] = (eeprom.reg[71]) | (LMX2595_SYSREF_DIV_PRE << 5) | (LMX2595_SYSREF_PULSE << 4) | (LMX2595_SYSREF_EN << 3) | (LMX2595_SYSREF_REPEAT << 2) | (0b1); 
  eeprom.reg[72] = (eeprom.reg[72]) | (LMX2595_SYSREF_DIV);
  eeprom.reg[73] = (eeprom.reg[73]) | (LMX2595_JESD_DAC2_CTRL << 6) | (LMX2595_JESD_DAC1_CTRL);
  eeprom.reg[74] = (eeprom.reg[74]) | (LMX2595_SYSREF_PULSE_CNT << 12) | (LMX2595_JESD_DAC4_CTRL << 6) | (LMX2595_JESD_DAC3_CTRL);
  eeprom.reg[75] = (eeprom.reg[75]) | (0b1 << 11) | (LMX2595_CHDIV<< 6);
  eeprom.reg[76] = (eeprom.reg[76]) | (0b1100); 
  eeprom.reg[77] = (eeprom.reg[77]) | (0b0); 
  eeprom.reg[78] = (eeprom.reg[78]) | (LMX2595_RAMP_THRESH32 << 11) | (LMX2595_QUICK_RECAL_EN << 9) | (LMX2595_VCO_CAPCTRL_STRT << 1) | (0b1); 
}

void LMX2595DecodeRegs(void)
{
//Reg 0
LMX2595_RAMP_EN = (eeprom.reg[0] >> 15) & 0x01;
LMX2595_VCO_PHASE_SYNC = (eeprom.reg[0] >> 14) & 0x01;
LMX2595_OUT_MUTE = (eeprom.reg[0] >> 9) & 0x01;
LMX2595_FCAL_HPFD_ADJ = (eeprom.reg[0] >> 7) & 0x03;
LMX2595_FCAL_LPFD_ADJ = (eeprom.reg[0] >> 5) & 0x03;
LMX2595_FCAL_EN = (eeprom.reg[0] >> 3) & 0x01;
LMX2595_MUXOUT_LD_SEL = (eeprom.reg[0] >> 2) & 0x01;
LMX2595_RESET = (eeprom.reg[0] >> 1) & 0x01;
LMX2595_POWERDOWN = (eeprom.reg[0]) & 0x01;
//Reg 1
LMX2595_CAL_CLK_DIV = (eeprom.reg[1]) & 0x07;
//Reg 4
LMX2595_ACAL_CMP_DLY = (eeprom.reg[4] >> 8) & 0xFF;
//Reg 7
LMX2595_OUT_FORCE = (eeprom.reg[7] >> 14) & 0x01;
//Reg 8
LMX2595_VCO_DACISET_FORCE = (eeprom.reg[8] >> 14) & 0x01;
LMX2595_VCO_CAPCTRL_FORCE = (eeprom.reg[8] >> 11) & 0x01;
//Reg 9
LMX2595_OSC_2X = (eeprom.reg[9] >> 12) & 0x01;
//Reg 10
LMX2595_MULT = (eeprom.reg[10] >> 7) & 0x1F;
//Reg 11
LMX2595_PLL_R = (eeprom.reg[11] >> 4) & 0xFF;
//Reg 12
LMX2595_PLL_R_PRE = (eeprom.reg[12]) & 0xFFF;
//Reg 14
LMX2595_CPG = (eeprom.reg[14] >> 4) & 0x07;
//Reg 16
LMX2595_VCO_DACISET = (eeprom.reg[16]) & 0x1FF;
//Reg 17
LMX2595_VCO_DACISET_STRT = (eeprom.reg[17]) & 0x1FF;
//Reg 19
LMX2595_VCO_CAPCTRL = (eeprom.reg[19]) & 0xFF;
//Reg 20
LMX2595_VCO_SEL = (eeprom.reg[20] >> 11) & 0x07;
LMX2595_VCO_SEL_FORCE = (eeprom.reg[20] >> 10) & 0x01;
//Reg 25
LMX2595_DBLR_IBIAS_CTRL1 = (eeprom.reg[25]) & 0xFFFF;
//Reg 27
LMX2595_VCO2X_EN = (eeprom.reg[27]) & 0x01;
//Reg 31
LMX2595_SEG1_EN = (eeprom.reg[31] >> 14) & 0x01;
//Reg 34
LMX2595_PLL_NH = (eeprom.reg[34]) & 0x07;                //High 3 bits
//Reg 36
LMX2595_PLL_NL = (eeprom.reg[36]) & 0xFFFF;             //Low 16 bits
//combined 19 bit variable for conveniance 
LMX2595_PLL_N = (LMX2595_PLL_NH << 16) | LMX2595_PLL_NL ;
//Reg 37
LMX2595_MASH_SEED_EN = (eeprom.reg[37] >> 15) & 0x01;
LMX2595_PFD_DLY_SEL = (eeprom.reg[37] >> 8) & 0x3F;
//Reg 38
LMX2595_PLL_DENH = (eeprom.reg[38]) & 0xFFFF;        //High 16 bits
//Reg 39
LMX2595_PLL_DENL = (eeprom.reg[39]) & 0xFFFF ;        //Low 16 bits
//combined 32 bit variable for conveniance 
LMX2595_PLL_DEN = (LMX2595_PLL_DENH << 16) | LMX2595_PLL_DENL;    
//Reg 40
LMX2595_MASH_SEEDH = (eeprom.reg[40]) & 0xFFFF;        //High 16 bits
//Reg 41
LMX2595_MASH_SEEDL = (eeprom.reg[41]) & 0xFFFF;        //Low 16 bits
//combined 32 bit variable for conveniance 
LMX2595_MASH_SEED = (LMX2595_MASH_SEEDH << 16) | LMX2595_MASH_SEEDL ;
//Reg 42
LMX2595_PLL_NUMH = (eeprom.reg[42]) & 0xFFFF;        //High 16 bits
//Reg 43
LMX2595_PLL_NUML = (eeprom.reg[43]) & 0xFFFF;        //Low 16 bits
//combined 32 bit variable for conveniance 
LMX2595_PLL_NUM = (LMX2595_PLL_NUMH <<16) | LMX2595_PLL_NUML;
//Reg 44
LMX2595_OUTA_PWR = (eeprom.reg[44] >> 8) & 0x3F;
LMX2595_OUTB_PD = (eeprom.reg[44] >> 7) & 0x01;
LMX2595_OUTA_PD = (eeprom.reg[44] >> 6) & 0x01;
LMX2595_MASH_RESET_N = (eeprom.reg[44] >> 5) & 0x01;
LMX2595_MASH_ORDER = (eeprom.reg[44]) & 0x07;
//Reg 45
LMX2595_OUTA_MUX = (eeprom.reg[45] >> 11) & 0x03;
LMX2595_OUT_ISET = (eeprom.reg[45] >> 9) & 0x03;
LMX2595_OUTB_PWR = (eeprom.reg[45] ) & 0x3F;
//Reg 46
LMX2595_OUTB_MUX = (eeprom.reg[46]) & 0x03;
//Reg 58
LMX2595_INPIN_IGNORE = (eeprom.reg[58] >> 15) & 0x01;
LMX2595_INPIN_HYST = (eeprom.reg[58] >> 14) & 0x01;
LMX2595_INPIN_LVL = (eeprom.reg[58] >> 12) & 0x03;
LMX2595_INPIN_FMT = (eeprom.reg[58] >> 9) & 0x07;
//Reg 59
LMX2595_LD_TYPE = (eeprom.reg[59]) & 0x01;
//Reg 60
LMX2595_LD_DLY = (eeprom.reg[60]) & 0xFFFF;
//Reg 69
LMX2595_MASH_RST_COUNTH = (eeprom.reg[69]) & 0xFFFF;       //High 16 bits
//Reg 70
LMX2595_MASH_RST_COUNTL = (eeprom.reg[70]) & 0xFFFF;       //Low 16 bits
//combined 32 bit variable for conveniance 
LMX2595_MASH_RST_COUNT = (LMX2595_MASH_RST_COUNTH << 16) | LMX2595_MASH_RST_COUNTL;
//Reg 71
LMX2595_SYSREF_DIV_PRE = (eeprom.reg[71] >> 5) & 0x07;
LMX2595_SYSREF_PULSE = (eeprom.reg[71] >> 4) & 0x01;
LMX2595_SYSREF_EN = (eeprom.reg[71] >> 3) & 0x01;
LMX2595_SYSREF_REPEAT = (eeprom.reg[71] >> 2) & 0x01;
//Reg 72
LMX2595_SYSREF_DIV = (eeprom.reg[72]) & 0x7FF;
//Reg 73
LMX2595_JESD_DAC2_CTRL = (eeprom.reg[73] >> 6) & 0x3F;
LMX2595_JESD_DAC1_CTRL = (eeprom.reg[73]) & 0x3F;
//Reg 74
LMX2595_SYSREF_PULSE_CNT = (eeprom.reg[74] >> 12) & 0x0F;
LMX2595_JESD_DAC4_CTRL = (eeprom.reg[74] >> 6) & 0x3F;
LMX2595_JESD_DAC3_CTRL = (eeprom.reg[74]) & 0x3F;
//Reg 75
LMX2595_CHDIV = (eeprom.reg[75] >> 6) & 0x1F;
//Reg 78
LMX2595_RAMP_THRESH32 = (eeprom.reg[78] >> 11) & 0x01;
LMX2595_QUICK_RECAL_EN = (eeprom.reg[78] >> 9) & 0x01;
LMX2595_VCO_CAPCTRL_STRT = (eeprom.reg[78] >> 1) & 0xFF;

}

void LMX2595Update(void)
{
  LMX2595Send(0x2);           //Reg 0 + reset bit
  LMX2595Send(0x0);           //Reg 0 no reset
  for(int r = eeprom.numberOfRegs - 1; r >= 0; r--)
  {
      LMX2595Send(eeprom.reg[r]);
  }
  delay(10);
  LMX2595Send(eeprom.reg[0] | 0x08);         //set FCAL_EN 
}


double LMX2595CalcPFD(double rpfd)
{
  double r = 0;
  double mult = 1;
  bool dub = 0;
  bool div = 0;



  //first try a simple division...
  r = eeprom.refOsc / rpfd;
  if(((double) int(r))  == r)       //check if this is an integer division
    {
      goto done;
    }

  //next try using the doubler

  r = (eeprom.refOsc * 2.0) / rpfd;
  if(((double) int(r)) == r)       //check if this is an integer division
    {
      dub = 1;
      goto done;
    }

   if(eeprom.refOsc < rpfd)                      //try the multiplier
     {
      mult = int(rpfd / eeprom.refOsc);
      r = (eeprom.refOsc * mult) / rpfd;
      if(((double) int(r)) == r)      //check if this is an integer division 
        {
          goto done;
        } 

     }

  Serial.print("Unable to acheive a PFD of ");
  Serial.print(rpfd, 6);
  Serial.print(" MHz With a Ref Oscillator of ");
  Serial.print(eeprom.refOsc, 6);
  Serial.println(" MHz");
  Serial.print("PFD set to ");
  Serial.print((eeprom.refOsc * (1+dub) * mult) / r, 6);
  Serial.println(" MHz");

  done:
  if(r < 1) r = 1;
  LMX2595_PLL_R_PRE = 1;          //we are not using the Pre Multiplier Divider
  LMX2595_MULT = mult;            //set the multipier 
  LMX2595_PLL_R = r;              //we will use the post multiplier divider
  LMX2595_OSC_2X = dub;           //Reference doubler if wee need it

//calculate the actual PFD set and adjust the HP and LP registers to suit
  rpfd = (eeprom.refOsc * (1 + dub) * mult) / r;

  if(rpfd <= 100.0) LMX2595_FCAL_HPFD_ADJ = 0;
  if((rpfd > 100.0) && (rpfd <= 150.0))  LMX2595_FCAL_HPFD_ADJ = 1; 
  if((rpfd > 150.0) && (rpfd <= 200.0))  LMX2595_FCAL_HPFD_ADJ = 2;
  if(rpfd > 200.0)  LMX2595_FCAL_HPFD_ADJ = 3;  
  if(rpfd >= 10.0) LMX2595_FCAL_LPFD_ADJ = 0;
  if((rpfd >= 5.0) && (rpfd < 10.0))  LMX2595_FCAL_LPFD_ADJ = 1; 
  if((rpfd >= 2.5) && (rpfd < 5.0))  LMX2595_FCAL_LPFD_ADJ = 2;
  if(rpfd < 2.5)  LMX2595_FCAL_LPFD_ADJ = 3; 

  return rpfd;
}

void LMX2595SetPfd(void)
{
  double pfd;
  bool freqOK;
  
  freqOK = false;
  
  while(!freqOK)
    {
       Serial.printf("\nEnter required PFD in MHz -->");
       pfd = inputFloat();
       if(pfd == 0) return;
       pfd = LMX2595CalcPFD(pfd);
      if(pfd <= eeprom.maxPfd)
        {
          freqOK = true;
        }
      else
        {
          Serial.print("\nPFD must be less than ");
          Serial.print(eeprom.maxPfd);
          Serial.println(" MHz");
        }
      
    }
   
}

double LMX2595GetPfd(void)
{
  double pfd = eeprom.refOsc;
  double r = (double) LMX2595_PLL_R * LMX2595_PLL_R_PRE ;
  if(LMX2595_OSC_2X == 1) pfd = pfd * 2.0;
  pfd = (pfd * LMX2595_MULT) / r;
  return pfd;
}

void LMX2595SetFrequency(double direct)
{
  bool freqOK = false;
  double pfd;
  double freq;
  double n;
  double part;
  double err;
  uint32_t num;
  uint32_t den;
  int chd = 0;
  int divide[] = {1,2,4,6,8,12,16,24,32,48,64,72,96,128,192,256,384,512,768};
  bool dub = 0;
  bool maxDivisor;
  char resp;

  if((eeprom.cwidEn) || (eeprom.jtMode))           //if we are using FSK keying then we must keep the divisor to the same value to avoid glitches
  {
    maxDivisor = true;
  }
  else
  {
    maxDivisor = false;
  }

  pfd = LMX2595GetPfd();

  freqOK = false;
  if(direct == 0)
   {
   resp = getSelection("Is there an external Multiplier chain? Y or N --->");
   if((resp == 'Y') || (resp == 'y'))
     {
        Serial.print("Enter External Multiplication Factor ---> ");
        eeprom.extMult = inputNumber();
     }
   else
     {
       eeprom.extMult = 1;
     }
    while(!freqOK)
    {
      Serial.print("\nEnter required Final Frequency in MHz -->");
      freq = inputFloat() / (double) eeprom.extMult;
      if((freq > 9.766) && (freq <= 20000.000))
        {
          freqOK = true;
        }
      else
        {
          Serial.println("Synthesiser Frequency must be between 9.766 and 20000 MHz");
        }
    }
   }
   else
   {
    freq = direct;
   }

if (freq > 15000.0)                 //use the doubler between 15 and 20 GHz
  {
    freq = freq /2.0;
    dub = 1;
  }

//Calculate the required output divider
  if((freq >= 7500.000) && (freq <= 15000.000))  chd = 0;
  if((freq >= 3750.000) && (freq < 7500.000))  chd = 1;
  if((freq >= 1875.000) && (freq < 3750.000))  chd = 2;
  if((freq >= 1250.000) && (freq < 1875.000))  chd = 3; 
  if((freq >= 950.0) && (freq < 1250.000))  chd = 4;
  if((freq >= 625.0) && (freq < 950.00))  chd = 5;
  if((freq >= 470.0) && (freq < 625.0))  chd = 6;
  if((freq >= 320.0) && (freq < 470.0))  chd = 7;
  if((freq >= 235.0) && (freq < 320.0))  chd = 8;
  if((freq >= 160.0) && (freq < 235.0))  chd = 9;
  if((freq >= 120.0) && (freq < 160.0))  chd = 10;
  if((freq >= 105.0) && (freq < 120.0))  chd = 11;
  if((freq >= 80.0) && (freq < 105.0))  chd = 12;
  if((freq >= 59.0) && (freq < 80.0))  chd = 13;
  if((freq >= 40.0) && (freq < 59.0))  chd = 14;
  if((freq >= 29.50) && (freq < 40.0))  chd = 15;
  if((freq >= 20.0) && (freq < 29.5))  chd = 16;
  if((freq >= 14.7) && (freq < 20.0))  chd = 17;
  if((freq >= 9.766) && (freq < 14.7))  chd = 18;  

  if(chd == 0)
    {
      LMX2595_OUTA_MUX = 1;
      LMX2595_VCO2X_EN = 0;  
    }
  else
    {
      LMX2595_OUTA_MUX = 0;
      LMX2595_VCO2X_EN = 0; 
      LMX2595_CHDIV = chd - 1;
      if(LMX2595_CHDIV == 2)
       {
        LMX2595_SEG1_EN = 0;
       }
      else
        {
         LMX2595_SEG1_EN = 1;   
        }
    }

  if(dub == 1)
    {
      LMX2595_OUTA_MUX = 2;
      LMX2595_VCO2X_EN = 1; 
    }


//adjust the vco frequency to allow for the output division
  freq = freq * (double) divide[chd];

  n = freq / pfd;

  LMX2595_PLL_N = (unsigned int) n;

  n = n - (unsigned int) n;

//n now has the desired fractional part. Now we need to work out the f/d to acheive this


 // first try to calculate the fraction up to a denominator of FFFF. This is an arbritary limit to keep the speed up. 
 err = 1.0;
 uint32_t  bestnum = 0;
 uint32_t  bestden = 1;

if(! maxDivisor)
{
   for(den = 2; den <= 0xFFFF ; den++)
     {
      num = int((den + 0.0000000001)  * n);
      if(num < 1) continue;

      part = (double) num / (double) den;
      if(abs(part-n) < 0.0000000001)
        {
          bestnum = num;
          bestden = den;
          err = 0;
          goto done;
        }
      if(abs((part - n)) < err)
        {
            bestnum = num;
            bestden = den;
            err = abs(part - n);
        }
     }

}
// finally try using the maximum value for the Denominator to see if that gives a smaller error than we already have. 

    den = 0xFFFFFFFF;                     //using the highest number to give the finest tuning steps. 
    num = (uint32_t) ((double) den * n);
    part = (double) num / (double) den;
       if(abs((part - n)) < err)
        {
            bestnum = num;
            bestden = den; 
        }
  
  done:
//program the fraction division
  LMX2595_PLL_DEN = bestden;
  LMX2595_PLL_NUM = bestnum;

  switch(LMX2595_MASH_ORDER)
  {
    case 0:
      if(freq >12500.0)
        {
          LMX2595_PFD_DLY_SEL = 2; 
        }
      else
        {
          LMX2595_PFD_DLY_SEL = 2;
        }
        break;
    
    case 1:
          if(freq <= 10000.0)
        {
          LMX2595_PFD_DLY_SEL = 1; 
        }
      else if(freq > 12250.0)
        {
          LMX2595_PFD_DLY_SEL = 3;
        }
      else
        {
          LMX2595_PFD_DLY_SEL = 2;
        }
      break;

    case 2:
      if(freq <= 10000.0)
        {
          LMX2595_PFD_DLY_SEL = 2; 
        }
      else
        {
          LMX2595_PFD_DLY_SEL = 3;
        }
        break;

      case 3:
      if(freq <= 10000.0)
        {
          LMX2595_PFD_DLY_SEL = 3; 
        }
      else
        {
          LMX2595_PFD_DLY_SEL = 4;
        }
        break;

      case 4:
      if(freq <= 10000.0)
        {
          LMX2595_PFD_DLY_SEL = 5; 
        }
      else
        {
          LMX2595_PFD_DLY_SEL = 6;
        }
        break;
  }

  LMX2595EncodeRegs();
}

void LMX2595CalcFreq(void)
{
  double pfd;
  double vco;
  double n;
  double nf;
  double fr;

  uint16_t diva;
  int divide[] = {2,4,6,8,12,16,24,32,48,64,72,96,128,192,256,384,512,768};

  if(LMX2595_OUTA_MUX == 0)
    {
      diva = divide[LMX2595_CHDIV];
    }
  else
    {
      diva = 1;
    }


  n = (double) LMX2595_PLL_N;

  Serial.println();
  Serial.print("Reference Oscillator = ");
  Serial.print(eeprom.refOsc,10);
  Serial.println(" MHz");

  pfd = LMX2595GetPfd();

  Serial.print("PFD = ");
  Serial.print(pfd , 10);
  Serial.println(" MHz");

  fr = (double) LMX2595_PLL_NUM / (double) LMX2595_PLL_DEN;

  nf = (n + fr);

  vco = pfd * nf;

  Serial.print("N = ");
  Serial.print(nf , 10);
  Serial.print(" ( ");
  Serial.print(LMX2595_PLL_N);
  Serial.print(" + (");
  Serial.print(LMX2595_PLL_NUM);
  Serial.print("/");
  Serial.print(LMX2595_PLL_DEN);
  Serial.println(") )");

  Serial.print("VCO = ");
  Serial.print(vco , 10);
  Serial.println(" MHz");

  Serial.print("O/P Divider = /");
  Serial.println(diva);

  Serial.print("O/P Doubler = ");
  if(LMX2595_VCO2X_EN)
   {
     Serial.println("On");
     vco = vco * 2.0;
   }
  else
   {
     Serial.println("Off");
   }


  Serial.print("Output Frequency = ");
  Serial.print(vco / diva , 10);
  Serial.println(" MHz");

  if(eeprom.extMult > 1)
    {
      Serial.print("Final Multiplied Frequency = ");
      Serial.print((vco / diva) * (double) eeprom.extMult, 10);
      Serial.println(" MHz"); 
    }
}

double LMX2595GetFrequency(void)
{
  double pfd;
  double vco;
  double n;
  double nf;
  double fr;

  uint16_t diva;
  int divide[] = {2,4,6,8,12,16,24,32,48,64,72,96,128,192,256,384,512,768};

  if(LMX2595_OUTA_MUX == 0)
    {
      diva = divide[LMX2595_CHDIV];
    }
  else
    {
      diva = 1;
    }

  n = (double) LMX2595_PLL_N;

  pfd = LMX2595GetPfd();

  fr = (double) LMX2595_PLL_NUM / (double) LMX2595_PLL_DEN;

  nf = (n + fr);

  vco = pfd * nf;

  if(LMX2595_VCO2X_EN)
   {
     vco = vco * 2.0;
   }

  return vco / diva;
}

void LMX2595jtShift(uint8_t val)
{
  static uint8_t lastval;

  if(val == lastval) return;
  lastval = val;
  
        LMX2595Send((34 << 16) | ((jtN[val] >> 16) & 0xFFFF));
        LMX2595Send((36 << 16) | (jtN[val] & 0xFFFF));     

        LMX2595Send((38 << 16) | ((jtDen[val] >> 16) & 0xFFFF));
        LMX2595Send((39 << 16) | (jtDen[val] & 0xFFFF));

        LMX2595Send((42 << 16) | ((jtNum[val] >> 16) & 0xFFFF));
        LMX2595Send((43 << 16) | (jtNum[val] & 0xFFFF));
}

void LMX2595FskKey(bool key)
{
  static bool lastkey;
  
  if(key == lastkey) return;
  lastkey = key;

  if(key)
    {

       LMX2595Send(eeprom.reg[34]);
       LMX2595Send(eeprom.reg[36]);       

        LMX2595Send(eeprom.reg[38]);
        LMX2595Send(eeprom.reg[39]);

       LMX2595Send(eeprom.reg[42]);
       LMX2595Send(eeprom.reg[43]);

    }
  else
    {

        LMX2595Send((34 << 16) | ((cwidKeyUpN >> 16) & 0xFFFF));
        LMX2595Send((36 << 16) | (cwidKeyUpN & 0xFFFF));     

        LMX2595Send((38 << 16) | ((cwidKeyUpDen >> 16) & 0xFFFF));
        LMX2595Send((39 << 16) | (cwidKeyUpDen & 0xFFFF));

        LMX2595Send((42 << 16) | ((cwidKeyUpNum >> 16) & 0xFFFF));
        LMX2595Send((43 << 16) | (cwidKeyUpNum & 0xFFFF));
    }
}

void LMX2595SaveFskShift(void)
{
  cwidKeyUpN = LMX2595_PLL_N;
  cwidKeyUpDen = LMX2595_PLL_DEN;
  cwidKeyUpNum = LMX2595_PLL_NUM;
}

void LMX2595SaveJt(uint8_t index)
{
  jtN[index] = LMX2595_PLL_N;
  jtDen[index] = LMX2595_PLL_DEN;
  jtNum[index] = LMX2595_PLL_NUM;
}
