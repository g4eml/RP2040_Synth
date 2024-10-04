// Synthesiser controller using RP2040
// Colin Durbridge G4EML 2024


#define NUMBEROFCHANNELS 10

//Global values...

enum chipType { NONE, MAX2870 , ADF4351 , LMX2595 };
String chipName[] = {"None","MAX2870", "ADF4351" , "LMX2595"};


//These values are saved to the eeprom for recall on statup. 
//these values alpply to all channels

uint8_t chip = MAX2870;                 //index to the current chip type (applies to all channels)
uint8_t selChan = 0;                    //FF if channel is externally selected by switches. Channel number to force one channel. 
double refOsc = 100.000 ;               //reference oscillator frequency in MHz (applies to all channels)

//Bit definitions for fskMode flags

#define CWIDBIT 0x01
#define NOTCWIDBIT 0xFE
#define EXTKEYBIT 0x02
#define NOTEXTKEYBIT 0xFD

//this structure is repeated for each channel

struct chanstruct
{
uint32_t reg[80];                       // allow for up to 80 32 bit registers. 
uint8_t fskMode = 0;                    // 8 flag bits to indicate enabled special FSK modes.  Bit 0 = CWID   Bit 1 = External Key  
uint8_t cwidLen = 1;                    //number of characters in the CWID
uint8_t cwidSpeed = 10;                 //CWID speed in words per minute
uint8_t cwidInterval = 60;              //CWID Inteval in seconds.
float cwidShift = 0;                    //CW ID FSK Shift in MHz 
float keyShift = 0;                     //external key FSK Shift in MHz 
char cwid[32] = " ";                    // up to 32 CWID characters
uint8_t jtMode = 0;                     //JT mode
char jtid[13] = " ";                    //JT Message
uint8_t extMult = 1;                    //Multiplcation factor for external frequency multiplier. (used to calculate the correct FSK Shifts.)
};

struct chanstruct chanData[NUMBEROFCHANNELS];


//End of saved values

int numberOfRegs = 6;                     //number of registers in the current chip type
int numberOfBits = 32;                    //number of bits in each register
float maxPfd = 105.0;                     //maximum PFD frequency
bool jt4Only = true;                      //lower spec chips only support JT4 due to limited fractional register size.  
uint8_t channel = 0;                      //currently active channel.

uint32_t cwidKeyUpN = 1;                  //key up value for the PLL N used to shift the frequency for CWID. Calculated by cwidInit()
uint32_t cwidKeyUpNum = 1;                //Key up value for the PLL numerator used to shift the frequency for CWID. Calculated by cwidInit()
uint32_t cwidKeyUpDen = 1;                //Key up value for the PLL denominator used to shift the frequency for CWID. Calculated by cwidInit()

uint32_t ExtKeyUpN = 1;                  //key up value for the PLL N used to shift the frequency for External Key. Calculated by ExtKeyInit()
uint32_t ExtKeyUpNum = 1;                //Key up value for the PLL numerator used to shift the frequency for External Key. Calculated by ExtKeyInit()
uint32_t ExtKeyUpDen = 1;                //Key up value for the PLL denominator used to shift the frequency for External Key. Calculated by ExtKeyInit()

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

int seconds = 0;                         //seconds counter.  counts up to 120 seconds.  0-60 is even minute. 60-120 is odd minute. 
int milliseconds = 0;                   //millisecond counter used to increment seconds counter. 

bool cwidActive = false;                //flag to start sending CW ID
int nextcwidTime = 60;                   //trigger time for next CWID

bool jtActive = false;                  //flag to start Jt Sending
int  jtTime = 1;                       //trigger time for next JT sequence

bool lastKeyState = 1;                 //external key state last pass 1 = key up 0 = key down

#define GPSTXPin 0                      //Serial data to GPS module 
#define GPSRXPin 1                      //SeriaL data from GPS module

#define CHANSEL0Pin 10                  //External channel select pins. Pulled up to 3V3. High is Logic 0 Low is Logic 1
#define CHANSEL1Pin 11
#define CHANSEL2Pin 12
#define CHANSEL3Pin 13

#define EXTKEYPin 14                    //External Key input Pulled upp to 3V3 ground to key.

char gpsBuffer[256];                     //GPS data buffer
int gpsPointer;                          //GPS buffer pointer. 
char gpsCh;
int gpsSec = -1;                         //gps Seconds counter 0-59 is even minute, 60-119 is odd minute. -1 is GPS Invalid 
bool gpsActive = false;                    


void setup() 
{
  Serial.begin();                       //USB serial port
  Serial1.setRX(GPSRXPin);              //Configure the GPIO pins for the GPS module
  Serial1.setTX(GPSTXPin);

  pinMode(CHANSEL0Pin, INPUT_PULLUP);   //configure the channel select pins. 
  pinMode(CHANSEL1Pin, INPUT_PULLUP);   //Inverted logic to allow switches to ground. 
  pinMode(CHANSEL2Pin, INPUT_PULLUP);
  pinMode(CHANSEL3Pin, INPUT_PULLUP);

  pinMode(EXTKEYPin, INPUT_PULLUP);     //configure the external Key Input

  Serial1.begin(9600);                  //start GPS port comms
  gpsPointer = 0;
  delay(1000);
  EEPROM.begin(4096);
  if(EEPROM.read(0) == 0x57)        //magic number to indcate EEPROM is valid
    {
      EEPROM.get(1,chip);              //chip type for all channels
      EEPROM.get(2,selChan);           //read the selected channel. 0xFF if externally switched.
      EEPROM.get(3,refOsc);           //reference oscillator for all channels. 
      EEPROM.get(12,chanData);         //get channel data structure.
      if(selChan == 255)
        {
          channel = readChannelInputs();      //read the channel select bits. 
        }
      else
        {
          channel = selChan;          //fix the channel number. 
        } 
    }
   chipInit();
   initChannel();
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

//synchronise the local clock to the GPS clock if available

      if((milliseconds == 500) && (gpsSec != -1) && (gpsActive))
         {
          seconds = gpsSec;
          gpsActive = false;
         }

     if(selChan == 255)             //external chaannel selection
       {
         uint8_t newChan=readChannelInputs();
         if(newChan != channel)
           {
            channel=newChan;
            seconds = -1;                       //reset the timing after changing channel.
            milliseconds = 0;
            chipUpdate();
            initChannel();
           }

       }

     if((chanData[channel].fskMode & CWIDBIT) & (seconds == nextcwidTime))
       {
        cwidActive = true;                                        //start this CW ID
        nextcwidTime = (seconds + chanData[channel].cwidInterval) % 120;            //schedule the next CW ID
       }

     if((chanData[channel].jtMode != 0) & (seconds == jtTime))
       {
        jtActive = true;                                        //start the JT Sequence
       }

    if(chanData[channel].fskMode & CWIDBIT)
      {
        cwidTick();
      }

    if(chanData[channel].jtMode != 0)
      {
        jtTick();
      }

    if(chanData[channel].fskMode & EXTKEYBIT)
      {
        keyTick();
      }

      if(Serial.available() > 0 )          //test for USB command connected
     {
       mainMenu();                         //timing loop stops while the menu system is running.
       seconds = -1;                       //reset the timing after using the menu.
       milliseconds = 0;
       initChannel();  
       chipUpdate();    
       Serial.print("\nSynthesiser programmed. Press any key for menu");
       delay(500);
       flushInput();
     }
    
    if(Serial1.available() > 0)           //data received from GPS module
      {
        while(Serial1.available() >0)
          {
            gpsCh=Serial1.read();
            if(gpsCh > 31) gpsBuffer[gpsPointer++] = gpsCh;
            if((gpsCh == 13) || (gpsPointer > 255))
              {
                gpsBuffer[gpsPointer] = 0;
                processNMEA();
                gpsPointer = 0;
              }
          }

      }


   }
  

}

void initChannel()
{
  chipDecodeRegs();
  if(chanData[channel].fskMode & CWIDBIT)
    {
      cwidInit();
    }
   
  if(chanData[channel].fskMode & EXTKEYBIT)
    {
      keyInit();
    }

   if(chanData[channel].jtMode)
     {
       jtInit();
     }


}

void processNMEA(void)
{
  float gpsTime;

 gpsActive = true;
 if(strstr(gpsBuffer , "RMC") != NULL)                         //is this the RMC sentence?
  {
    if(strstr(gpsBuffer , "A") != NULL)                       // is the data valid?
      {
       int p=strcspn(gpsBuffer , ",");                         // find the first comma
       gpsTime = strtof(gpsBuffer+p+1 , NULL);                 //copy the time to a floating point number
       gpsSec = 60 * ((int(gpsTime) / 100) % 2);                 // count odd and even minutes
       gpsSec = gpsSec + (int(gpsTime) % 100);  
      }
    else
     {
       gpsSec = -1;                                            //GPS time not valid
     }
  }


}

uint8_t readChannelInputs(void)
{
  uint8_t ch;
  ch = (digitalRead(CHANSEL3Pin) << 3) | (digitalRead(CHANSEL2Pin) << 2) | (digitalRead(CHANSEL1Pin) << 1) | (digitalRead(CHANSEL0Pin));
  ch = ~ch & 0x0F;           //invert the logic and limit to 4 bits. 
  if(ch > (NUMBEROFCHANNELS -1)) ch = NUMBEROFCHANNELS-1;
  return ch;
}

void saveSettings(void)
{
    EEPROM.write(0, 0x57);         //magic number to indcate EEPROM is valid
    EEPROM.put(1,chip);            //save the chip type
    EEPROM.put(2,selChan);         //Save the currently selected channel
    EEPROM.put(3,refOsc);          //reference oscillator for all channels.
    EEPROM.put(12,chanData);         //get channel data structure. 
    EEPROM.commit();
}