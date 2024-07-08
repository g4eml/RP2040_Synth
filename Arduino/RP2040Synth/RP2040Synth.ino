// Synthesiser controller using RP2040
// Colin Durbridge G4EML 2024

//Global values...

enum chipType { NONE, MAX2780 , ADF4351 , LMX2595 };
String chipName[] = {"None","MAX2780", "ADF4351" , "LMX2595"};

uint8_t chip = MAX2780;                  //index to the current chip type
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

uint32_t cwidKeyUpN = 1;                  //key up value for the PLL N used to shift the frequency. Calculated by cwidInit()
uint32_t cwidKeyUpNum = 1;                //Key up value for the PLL numerator used to shift the frequency. Calculated by cwidInit()
uint32_t cwidKeyUpDen = 1;                //Key up value for the PLL denominator used to shift the frequency. Calculated by cwidInit()

void saveSettings(void);

#include <EEPROM.h>
#include <SPI.h>


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
      if(cwidEn == 0x73)
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
      chipDecodeRegs();
    }

  chipInit();
  if(cwidEn == 0x73)
    {
      cwidInit();
    }
}

void loop() 
{
  Serial.print("\n");
  Serial.print(chipName[chip]);
  Serial.println(" Synthesiser programmed, Sleeping");

  chipUpdate();
  
  unsigned long loopTimer = millis();
  while(1)
   {
     while(loopTimer == millis());          //hang waiting for the next 1mS timeslot
     loopTimer = millis();

      if(Serial.available() > 0 )          //test for USB command connected
     {
       mainMenu();
       chipUpdate();
       if(cwidEn == 0x73)
         {
           cwidInit();
         }
       Serial.print("\nSynthesiser programmed. Press any key for menu");
       delay(1000);
       flushInput();
     }
    
    if(cwidEn == 0x73)
      {
        cwidTick();
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
    EEPROM.put(0x230,cwidEn);       //test if CWID is enabled (magic number is 0x73)
      if(cwidEn == 0x73)
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
    EEPROM.commit();
}