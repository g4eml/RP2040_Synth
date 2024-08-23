// Synthesiser controller using RP2040
// Colin Durbridge G4EML 2024

//Global values...

enum chipType { NONE, MAX2870 , ADF4351 , LMX2595 };
String chipName[] = {"None","MAX2870", "ADF4351" , "LMX2595"};


//These values are saved to the EEPROM for recall on statup. 

uint8_t chip = MAX2870;                  //index to the current chip type
uint32_t reg[128];                    // allow for up to 128 32 bit registers. 
int numberOfRegs = 6;                  //number of registers in the current chip type
int numberOfBits = 32;                 //number of bits in each register
float maxPfd = 105.0;                 //maximum PFD frequency
double refOsc = 100.000 ;             //reference oscillator frequency in MHz
uint8_t cwidEn = 0;                      //magic number to indicate if CWID is enabled 0x73 = enabled. 
uint8_t cwidLen = 1;                    //number of characters in the CWID
uint8_t cwidSpeed = 10;                  //CWID speed in words per minute
uint8_t cwidInterval = 60;               //CWID Inteval in seconds.
double cwidShift = 0;                       //CW ID FSK Shift in MHz  
char cwid[257] = " ";                      //CWID characters
uint8_t jtMode = 0;                        //JT mode
char jtid[13] = " ";                       //JT Message


//End of saved values



uint32_t cwidKeyUpN = 1;                  //key up value for the PLL N used to shift the frequency. Calculated by cwidInit()
uint32_t cwidKeyUpNum = 1;                //Key up value for the PLL numerator used to shift the frequency. Calculated by cwidInit()
uint32_t cwidKeyUpDen = 1;                //Key up value for the PLL denominator used to shift the frequency. Calculated by cwidInit()

void saveSettings(void);

#include <EEPROM.h>
#include <SPI.h>
#include <JTEncode.h>

#define JT65B_TONE_SPACING       5.4         // 2 * 2.69 Hz
#define JT65C_TONE_SPACING       10.8        // 4 * 2.69 Hz
#define JT4G_TONE_SPACING        315         // 72 * 4.37 Hz
#define JT65_DELAY               372         // Delay in ms for JT65
#define JT4_DELAY                229         // Delay value for JT4


uint8_t jtBuffer[256];
uint8_t jtSymbolCount;
uint16_t jtToneDelay;
double jtToneSpacing;
uint8_t jtNumberOfTones;

uint32_t jtN[65];
uint32_t jtNum[65];
uint32_t jtDen[65];


JTEncode jtEncode;

int seconds =0;                         //seconds counter.  counts up to 120 seconds.  0-60 is even minute. 60-120 is odd minute. 
int milliseconds = 0;                   //millisecond counter used to increment seconds counter. 

bool cwidActive = false;                //flag to start sending CW ID
int nextcwidTime = 60;                   //trigger time for next CWID

bool jtActive = false;                  //flag to start Jt Sending
int  jtTime = 1;                       //trigger time for next JT sequence

void setup() 
{
  Serial.begin();
  delay(1000);
  EEPROM.begin(1024);
  if(EEPROM.read(0) == 0x55)        //magic number to indcate EEPROM is valid
    {
      EEPROM.get(1,refOsc);         //get reference oscillator frequency for use in calculations. 
      EEPROM.get(0x10,chip);        //read the chip type
      for(int i=0;i<128;i++)        //read the saved register settings in 0x020 to 0x220
      {
        EEPROM.get(0x20 + i*4,reg[i]);
      }

      EEPROM.get(0x230,cwidEn);       //test if CWID is enabled (magic number is 0x73)
      if(cwidEn)
          {
            EEPROM.get(0x231,cwidLen);        //number of characters in CWID
            EEPROM.get(0x232,cwidSpeed);       //speed in words per minute 
            EEPROM.get(0x233,cwidInterval);    //interval in seconds.
            EEPROM.get(0x234 , cwidShift);      //Desired CWID shift in MHz  (may not be the actual shift achieved)
            for(int i = 0 ; i< cwidLen ; i++)
              {
                EEPROM.get(0x240 + i , cwid[i+1]);    //read in the characters
              }
          }
       EEPROM.get(0x350,jtMode);                //test if Jt mode is enabled
       if(jtMode)
         {
          for(int i = 0 ; i< 13 ; i++)
              {
                EEPROM.get(0x360 + i , jtid[i]);    //read in the characters
              }
         }

      chipDecodeRegs();
    }

  chipInit();
  if(cwidEn)
    {
      cwidInit();
    }
   
   if(jtMode)
     {
       jtInit();
     }

    seconds = -1;
}

void loop() 
{
  Serial.print("\n");
  Serial.print(chipName[chip]);
  Serial.println(" Synthesiser programmed, Sleeping");

  chipUpdate();
  delay(100);
  chipUpdate();
  
  unsigned long loopTimer = millis();
  while(1)
   {
     while(loopTimer == millis());          //hang waiting for the next 1mS timeslot
     loopTimer = millis();
     milliseconds++;
     if(milliseconds == 1000)
      {
        seconds++;
        milliseconds = 0;
        if(seconds == 120)
          {
            seconds = 0;
          }
      }

     if((cwidEn) & (seconds == nextcwidTime))
       {
        cwidActive = true;                                        //start this CW ID
        nextcwidTime = (seconds + cwidInterval) % 120;            //schedule the next CW ID
       }

     if((jtMode != 0) & (seconds == jtTime))
       {
        jtActive = true;                                        //start the JT Sequence
       }

    if(cwidEn)
      {
        cwidTick();
      }

    if(jtMode != 0)
      {
        jtTick();
      }

      if(Serial.available() > 0 )          //test for USB command connected
     {
       mainMenu();                         //timing loop stops while the menu system is running.
       chipUpdate();
       if(cwidEn)
         {
           cwidInit();
         }
       if(jtMode != 0)
         {
           jtInit();
         }      
       Serial.print("\nSynthesiser programmed. Press any key for menu");
       delay(1000);
       flushInput();
     }
    


   }
  

}

void saveSettings(void)
{
    EEPROM.write(0, 0x55);         //magic number to indcate EEPROM is valid
    EEPROM.put(1,refOsc);
    EEPROM.put(0x10,chip);        //read the chip type
    for(int i=0;i<128;i++)         //all register settings
      {
        EEPROM.put(0x20 + i*4,reg[i]);
      }
    EEPROM.put(0x230,cwidEn);       //CWID 
      if(cwidEn)
          {
            EEPROM.put(0x231,cwidLen);        //number of characters in CWID
            EEPROM.put(0x232,cwidSpeed);       //speed in words per minute 
            EEPROM.put(0x233,cwidInterval);    //interval in seconds.
            EEPROM.put(0x234, cwidShift);        //Desired Shift in Hz. 
            for(int i = 0 ; i< cwidLen ; i++)
              {
                EEPROM.put(0x240 + i , cwid[i + 1]);    //save the characters
              }
          }
    EEPROM.put(0x350,jtMode);
      if(jtMode)
         {
            for(int i = 0 ; i< 13 ; i++)
              {
                EEPROM.put(0x360 + i , jtid[i]);    //save the characters
              }
         }
    EEPROM.commit();
}