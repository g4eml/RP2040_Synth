//Chip specific functions for ADF5355 device

#define ADF5355CEPin 3         //GPO 3 Connect to ADF5355 CE Pin
#define ADF5355MUXPin 4         //GPO 4 Connect to ADF5355 MUX Pin
#define ADF5355LEPin 5         //GPO 5 Connect to ADF5355 LE Pin
#define ADF5355CKPin 6         //GPO 6 Connect to ADF5355 CLK Pin
#define ADF5355DATPin 7         //GPO 7 Connect to ADF5355 DAT Pin



//ADF5355 Register bits.
//REG 0
bool ADF5355_AUTOCAL;
bool ADF5355_PRESCALER;
unsigned int ADF5355_INT;

//REG 1
uint32_t ADF5355_FRAC1;

//REG 2
unsigned int ADF5355_FRAC2;
unsigned int ADF5355_MOD2;

//REG 3
bool ADF5355_SD;
bool ADF5355_PR;
bool ADF5355_PA;
uint32_t ADF5355_PHASE;

//REG 4
uint8_t ADF5355_MUX;
bool ADF5355_DBR;
bool ADF5355_RDIV2;
unsigned int ADF5355_R;
bool ADF5355_DOUBLEBUFF;
uint8_t ADF5355_CP;
bool ADF5355_REFMODE;
bool ADF5355_MUXLOGIC;
bool ADF5355_PDP;
bool ADF5355_PD;
bool ADF5355_CPTS;
bool ADF5355_CR;

//REG 5
uint32_t ADF5355_R5_RESERVED;   //fixed pattern 0x 00800025

//REG 6
bool ADF5355_GB;
bool ADF5355_NB;
uint8_t ADF5355_R6_RESERVED;
bool ADF5355_FBS;
uint8_t ADF5355_RFDIV;
uint8_t ADF5355_CPBL;
bool ADF5355_MTLD;
bool ADF5355_RFBDIS;
bool ADF5355_RFAEN;
uint8_t ADF5355_RFPWR;

//REG7
uint8_t ADF5355_R7_RESERVED;
bool ADF5355_LES;
uint8_t ADF5355_LDCC;
bool ADF5355_LOLM;
uint8_t ADF5355_FRACNLDP;
bool ADF5355_LDM;

//REG8
uint32_t ADF5355_R8_RESERVED;   //fixed pattern 0x 102D0428

//REG9
uint8_t ADF5355_VCOBD;
unsigned int ADF5355_TIMEOUT;
uint8_t ADF5355_ALTIMEOUT;
uint8_t ADF5355_SYNTIMEOUT;

//REG10
uint32_t ADF5355_R10_RESERVED;   //fixed pattern 0x 00300 in top 18 bits
uint8_t ADF5355_ADCCLK;
bool ADF5355_ADCCONV;
bool ADF5355_ADCEN;

//REG11
uint32_t ADF5355_R11_RESERVED;   //fixed pattern 0x 0061300B

//REG12
unsigned int ADF5355_RESYNC_CLOCK;
unsigned int ADF5355_R12_RESERVED;  //fixed pattern 0x 041 in bits 15 - 4



 void ADF5355SetDefault(void)
 {
    //ADF5355 Register bits. Default settings give output at 435.100 MHz with 10MHz PFD.
//REG 0
ADF5355_AUTOCAL = 1;
ADF5355_PRESCALER = 1;
ADF5355_INT = 348;

//REG 1
unsigned int ADF5355_FRAC1 = 1342177;

//REG 2
ADF5355_FRAC2 = 7;
ADF5355_MOD2 = 25;

//REG 3
ADF5355_SD = 0;
ADF5355_PR = 0;
ADF5355_PA = 0;
ADF5355_PHASE =0;

//REG 4
ADF5355_MUX = 0;
ADF5355_DBR = 0;
ADF5355_RDIV2 = 0;
ADF5355_R = 1;
ADF5355_DOUBLEBUFF = 0;
ADF5355_CP = 2;
ADF5355_REFMODE = 0;
ADF5355_MUXLOGIC = 1;
ADF5355_PDP = 1;
ADF5355_PD = 0;
ADF5355_CPTS = 0;
ADF5355_CR = 0;

//REG 5
ADF5355_R5_RESERVED = 0x0080002;

//REG 6
ADF5355_GB = 1;
ADF5355_NB = 1;
ADF5355_R6_RESERVED = 0x0A;
ADF5355_FBS = 1;
ADF5355_RFDIV = 3;
ADF5355_CPBL = 1;
ADF5355_MTLD = 1;
ADF5355_RFBDIS = 1;
ADF5355_RFAEN = 1;
ADF5355_RFPWR = 3;

//REG7
ADF5355_R7_RESERVED = 4;  
ADF5355_LES = 1;
ADF5355_LDCC = 0;
ADF5355_LOLM = 1;
ADF5355_FRACNLDP = 3;
ADF5355_LDM = 0;

//REG8
ADF5355_R8_RESERVED = 0x102D042; 

//REG9
ADF5355_VCOBD = 5;
ADF5355_TIMEOUT = 512;
ADF5355_ALTIMEOUT = 31;
ADF5355_SYNTIMEOUT = 31;

//REG10
ADF5355_R10_RESERVED = 0x00300;  
ADF5355_ADCCLK = 25;
ADF5355_ADCCONV = 1;
ADF5355_ADCEN = 1;

//REG11
ADF5355_R11_RESERVED = 0x0061300;

//REG12
ADF5355_RESYNC_CLOCK = 1;
ADF5355_R12_RESERVED = 0x41;

   ADF5355SetFrequency(0);
   ADF5355EncodeRegs();
   ADF5355Update();

 }

 void ADF5355SetParameters(void)
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
         Serial.println("Valid Parameter Names (See Data Sheet) are:-");
         Serial.println("Reg0 = AUTOCAL PRESCALER INT");
         Serial.println("Reg1 = FRAC1"); 
         Serial.println("Reg2 = FRAC2 MOD2"); 
         Serial.println("Reg3 = SD PR PA PHASE");
         Serial.println("Reg4 = MUX DBR RDIV2 R DOUBLEBUFF CP REFMODE MUXLOGIC PDP PD CPTS CR");
         Serial.println("Reg5 = (No User Settings)");
         Serial.println("Reg6 = GB NB FBS RFDIV CPBL MTLD RFBDIS RFAEN RFPWR");
         Serial.println("Reg7 = LES LDCC LOLM FRACNLDP LDM");
         Serial.println("Reg8 = (No User Settings)");
         Serial.println("Reg9 = VCOBD TIMEOUT ALTIMEOUT SYNTIMEOUT");
         Serial.println("Reg10 = ADCCLK ADCCONV ADCEN");
         Serial.println("Reg11 = (No User Settings)");
         Serial.println("Reg12 = RESYNC_CLOCK");
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

 //Reg0
      if(paramBool(param , "AUTOCAL" , &ADF5355_AUTOCAL , value)) goto handled;
      if(paramBool(param , "PRESCALER" , &ADF5355_PRESCALER , value)) goto handled;
      if(paramUint(param , "INT" , &ADF5355_INT , value , 23 , 65535)) goto handled;


 //REG1
      if(paramUint32(param , "FRAC1" , &ADF5355_FRAC1 , value , 0 , 16777215)) goto handled;


 //REG2
      if(paramUint(param , "FRAC2" , &ADF5355_FRAC2 , value , 0 , 16383)) goto handled;
      if(paramUint(param , "MOD2" , &ADF5355_MOD2 , value , 2 , 16383)) goto handled;


 //REG3
      if(paramBool(param , "SD" , &ADF5355_SD , value)) goto handled;
      if(paramBool(param , "PR" , &ADF5355_PR , value)) goto handled;
      if(paramBool(param , "PA" , &ADF5355_PA , value)) goto handled;
      if(paramUint32(param , "PHASE" , &ADF5355_PHASE , value , 0 , 16777215)) goto handled;


 //REG4
      if(paramByte(param , "MUX" , &ADF5355_MUX , value , 0 , 7)) goto handled;
      if(paramBool(param , "DBR" , &ADF5355_DBR , value)) goto handled;
      if(paramBool(param , "RDIV2" , &ADF5355_RDIV2 , value)) goto handled;
      if(paramUint(param , "R" , &ADF5355_R , value , 1 , 1023)) goto handled;
      if(paramBool(param , "DOUBLEBUFF" , &ADF5355_DOUBLEBUFF , value)) goto handled;
      if(paramByte(param , "CP" , &ADF5355_CP , value , 0 , 15)) goto handled;
      if(paramBool(param , "REFMODE" , &ADF5355_REFMODE , value)) goto handled;
      if(paramBool(param , "MUXLOGIC" , &ADF5355_MUXLOGIC , value)) goto handled;
      if(paramBool(param , "PDP" , &ADF5355_PDP , value)) goto handled;
      if(paramBool(param , "PD" , &ADF5355_PD , value)) goto handled;
      if(paramBool(param , "CPTS" , &ADF5355_CPTS , value)) goto handled;
      if(paramBool(param , "CR" , &ADF5355_CR , value)) goto handled;      

 //REG5
 //No User variables

 //REG6
      if(paramBool(param , "GB" , &ADF5355_GB , value)) goto handled;
      if(paramBool(param , "NB" , &ADF5355_NB , value)) goto handled;
      if(paramBool(param , "FBS" , &ADF5355_FBS , value)) goto handled;
      if(paramByte(param , "RFDIV" , &ADF5355_RFDIV , value , 0 , 6)) goto handled;
      if(paramByte(param , "CPBL" , &ADF5355_CPBL , value , 1 , 255)) goto handled;
      if(paramBool(param , "MTLD" , &ADF5355_MTLD , value)) goto handled;
      if(paramBool(param , "RFBDIS" , &ADF5355_RFBDIS , value)) goto handled;
      if(paramBool(param , "RFAEN" , &ADF5355_RFAEN , value)) goto handled;
      if(paramByte(param , "RFPWR" , &ADF5355_RFPWR , value , 0 , 3)) goto handled;

//REG7
      if(paramBool(param , "LES" , &ADF5355_LES , value)) goto handled;
      if(paramByte(param , "LDCC" , &ADF5355_LDCC , value , 0 , 3)) goto handled;
      if(paramBool(param , "LOLM" , &ADF5355_LOLM , value)) goto handled;
      if(paramByte(param , "FRACNLDP" , &ADF5355_FRACNLDP , value , 0 , 3)) goto handled;
      if(paramBool(param , "LDM" , &ADF5355_LDM , value)) goto handled;
 
 //REG8
 //No User variables

 //REG9
      if(paramByte(param , "VCOBD" , &ADF5355_VCOBD , value , 1 , 255)) goto handled;
      if(paramUint(param , "TIMEOUT" , &ADF5355_TIMEOUT , value , 1 , 1023)) goto handled;     
      if(paramByte(param , "ALTIMEOUT" , &ADF5355_ALTIMEOUT , value , 1 , 31)) goto handled;
      if(paramByte(param , "SYNTIMEOUT" , &ADF5355_SYNTIMEOUT , value , 1 , 31)) goto handled;

//REG10
      if(paramByte(param , "ADCCLK" , &ADF5355_ADCCLK , value , 1 , 255)) goto handled;
      if(paramBool(param , "ADCCONV" , &ADF5355_ADCCONV , value)) goto handled;
      if(paramBool(param , "ADCEN" , &ADF5355_ADCEN , value)) goto handled;

//REG11
 //No User variables

//REG12
      if(paramUint(param , "RESYNC_CLOCK" , &ADF5355_RESYNC_CLOCK , value , 1 , 65535)) goto handled;


     handled:
     ADF5355EncodeRegs();
     ADF5355Update();

   }
 }


 void ADF5355Init(void)
 {
   numberOfRegs = 13;                   //number of registers in the current chip type
   numberOfBits = 32;                   //number of bits in each register
   maxPfd = 125.0;
   minPfd = 0;
   maxOsc = 251;
   minOsc = 9;
   jt4Only = false;
   jtDisable = false;
   pinMode(ADF5355CEPin,OUTPUT);
   digitalWrite(ADF5355CEPin,HIGH); 
   pinMode(ADF5355LEPin,OUTPUT);
   digitalWrite(ADF5355LEPin,HIGH);
   SPI.setRX(ADF5355MUXPin);
   SPI.setTX(ADF5355DATPin);
   SPI.setSCK(ADF5355CKPin);
   SPI.beginTransaction(SPISettings(40000,MSBFIRST,SPI_MODE0));
   SPI.begin();
 }

 void ADF5355Send(int32_t val)
 {
   int a=(val >> 24) & 0xFF;
   int b=(val >> 16) & 0xFF;
   int c=(val >> 8) & 0xFF;
   int d=val & 0xFF;
  
   digitalWrite(ADF5355LEPin,LOW);
   delayMicroseconds(10);
   SPI.transfer((uint8_t)a);
   SPI.transfer((uint8_t)b);
   SPI.transfer((uint8_t)c);
   SPI.transfer((uint8_t)d);
   delayMicroseconds(10);
   digitalWrite(ADF5355LEPin,HIGH);
   delayMicroseconds(10);  
 }


 void ADF5355EncodeRegs(void)
 {
   chanData[channel].reg[0] = (ADF5355_AUTOCAL << 21) | (ADF5355_PRESCALER << 20) | (ADF5355_INT << 4);
   chanData[channel].reg[1] = (ADF5355_FRAC1 << 4) | 1 ;
   chanData[channel].reg[2] = (ADF5355_FRAC2 << 18)  | (ADF5355_MOD2 << 4) | 2;
   chanData[channel].reg[3] = (ADF5355_SD << 30) | (ADF5355_PR << 29) | (ADF5355_PA << 28) | (ADF5355_PHASE << 4) | 3;
   chanData[channel].reg[4] = (ADF5355_MUX << 27) | (ADF5355_DBR << 26) | (ADF5355_RDIV2 << 25) | (ADF5355_R << 15) | (ADF5355_DOUBLEBUFF << 14) | (ADF5355_CP << 10) | (ADF5355_REFMODE << 9) | (ADF5355_MUXLOGIC << 8) | (ADF5355_PDP << 7) | (ADF5355_PD << 6)| (ADF5355_CPTS << 5)| (ADF5355_CR << 4) | 4;
   chanData[channel].reg[5] = (ADF5355_R5_RESERVED << 4) | 5; 
   chanData[channel].reg[6] = (ADF5355_GB << 30) | (ADF5355_NB << 29) | (ADF5355_R6_RESERVED << 25) | (ADF5355_FBS << 24) | (ADF5355_RFDIV << 21) | (ADF5355_CPBL << 13) | (ADF5355_MTLD << 11) | (ADF5355_RFBDIS << 10) | (ADF5355_RFAEN << 6) | (ADF5355_RFPWR << 4) | 6;
   chanData[channel].reg[7] = (ADF5355_R7_RESERVED << 26) | (ADF5355_LES << 25) | (ADF5355_LDCC << 8) | (ADF5355_LOLM << 7) | (ADF5355_FRACNLDP << 5) | (ADF5355_LDM << 4) | 7;
   chanData[channel].reg[8] = (ADF5355_R8_RESERVED << 4) | 8;
   chanData[channel].reg[9] = (ADF5355_VCOBD << 24) | (ADF5355_TIMEOUT << 14) | (ADF5355_ALTIMEOUT << 9) | (ADF5355_SYNTIMEOUT << 4) | 9;
   chanData[channel].reg[10] = (ADF5355_R10_RESERVED << 14) |(ADF5355_ADCCLK << 6) |(ADF5355_ADCCONV << 5) |(ADF5355_ADCEN << 4) | 10;
   chanData[channel].reg[11] = (ADF5355_R11_RESERVED << 4) | 11;
   chanData[channel].reg[12] = (ADF5355_RESYNC_CLOCK << 16) | (ADF5355_R12_RESERVED << 4) | 12;
 }

 void ADF5355DecodeRegs(void)
 {
   ADF5355_AUTOCAL = (chanData[channel].reg[0] >> 21) & 0x01;
   ADF5355_PRESCALER = (chanData[channel].reg[0] >> 20) & 0x01;
   ADF5355_INT = (chanData[channel].reg[0] >> 4) & 0xFFFF;

   ADF5355_FRAC1 = (chanData[channel].reg[1] >> 4) & 0xFFFFFF;

   ADF5355_FRAC2 = (chanData[channel].reg[2] >> 18) & 0x3FFF;
   ADF5355_MOD2 = (chanData[channel].reg[2] >> 4) & 0x3FFF;

   ADF5355_SD = (chanData[channel].reg[3] >> 30) & 0x01;
   ADF5355_PR = (chanData[channel].reg[3] >> 29) & 0x01;
   ADF5355_PA = (chanData[channel].reg[3] >> 28) & 0x01;
   ADF5355_PHASE = (chanData[channel].reg[3] >> 4) & 0xFFFFFF;

   ADF5355_MUX = (chanData[channel].reg[4] >> 27) & 0x07;
   ADF5355_DBR = (chanData[channel].reg[4] >> 26) & 0x01;
   ADF5355_RDIV2 = (chanData[channel].reg[4] >> 25) & 0x01;
   ADF5355_R = (chanData[channel].reg[4] >> 15) & 0x3FF;
   ADF5355_DOUBLEBUFF = (chanData[channel].reg[4] >> 14) & 0x01;
   ADF5355_CP = (chanData[channel].reg[4] >> 10) & 0x0F;
   ADF5355_REFMODE = (chanData[channel].reg[4] >> 9) & 0x01;   
   ADF5355_MUXLOGIC = (chanData[channel].reg[4] >> 8) & 0x01;
   ADF5355_PDP = (chanData[channel].reg[4] >> 7) & 0x01;
   ADF5355_PD = (chanData[channel].reg[4] >> 6) & 0x01;
   ADF5355_CPTS = (chanData[channel].reg[4] >> 5) & 0x01;
   ADF5355_CR = (chanData[channel].reg[4] >> 4) & 0x01;

   ADF5355_R5_RESERVED =(chanData[channel].reg[5] >> 4) & 0xFFFFFFF; 

   ADF5355_GB = (chanData[channel].reg[6] >> 30) & 0x01; 
   ADF5355_NB = (chanData[channel].reg[6] >> 29) & 0x01;
   ADF5355_R6_RESERVED = (chanData[channel].reg[6] >> 25) & 0x0F;
   ADF5355_FBS = (chanData[channel].reg[6] >> 24) & 0x01;
   ADF5355_RFDIV = (chanData[channel].reg[6] >> 21) & 0x07;
   ADF5355_CPBL = (chanData[channel].reg[6] >> 13) & 0xFF;
   ADF5355_MTLD = (chanData[channel].reg[6] >> 11) & 0x01;
   ADF5355_RFBDIS = (chanData[channel].reg[6] >> 10) & 0x01;   
   ADF5355_RFAEN = (chanData[channel].reg[6] >> 6) & 0x01;
   ADF5355_RFPWR = (chanData[channel].reg[6] >> 4) & 0x03;

   ADF5355_R7_RESERVED = (chanData[channel].reg[7] >> 26) & 0x3F;
   ADF5355_LES = (chanData[channel].reg[7] >> 25) & 0x01;
   ADF5355_LDCC = (chanData[channel].reg[7] >> 8) & 0x03;
   ADF5355_LOLM = (chanData[channel].reg[7] >> 7) & 0x01;
   ADF5355_FRACNLDP = (chanData[channel].reg[7] >> 5) & 0x03;
   ADF5355_LDM = (chanData[channel].reg[7] >> 4) & 0x01; 

   ADF5355_R8_RESERVED =(chanData[channel].reg[8] >> 4) & 0xFFFFFFF;  

   ADF5355_VCOBD = (chanData[channel].reg[9] >> 24) & 0xFF;
   ADF5355_TIMEOUT = (chanData[channel].reg[9] >> 14) & 0x3FF;
   ADF5355_ALTIMEOUT = (chanData[channel].reg[9] >> 9) & 0x1F;
   ADF5355_SYNTIMEOUT = (chanData[channel].reg[9] >> 4) & 0x1F;

   ADF5355_R10_RESERVED = (chanData[channel].reg[10] >> 14) & 0x3FFFF;
   ADF5355_ADCCLK = (chanData[channel].reg[10] >> 6) & 0xFF;
   ADF5355_ADCCONV = (chanData[channel].reg[10] >> 5) & 0x01; 
   ADF5355_ADCEN = (chanData[channel].reg[10] >> 4) & 0x01;   

   ADF5355_R11_RESERVED = (chanData[channel].reg[11] >> 4) & 0xFFFFFFF; 

   ADF5355_RESYNC_CLOCK = (chanData[channel].reg[12] >> 16) & 0xFFFF;
   ADF5355_R12_RESERVED = (chanData[channel].reg[12] >> 4) & 0xFFF;  
 }

 void ADF5355Update(void)
 {
   ADF5355Send(chanData[channel].reg[12]);
   ADF5355Send(chanData[channel].reg[11]);
   ADF5355Send(chanData[channel].reg[10]);
   ADF5355Send(chanData[channel].reg[9]);
   ADF5355Send(chanData[channel].reg[8]);
   ADF5355Send(chanData[channel].reg[7]);
   ADF5355Send(chanData[channel].reg[6]);
   ADF5355Send(chanData[channel].reg[5]);
   ADF5355Send(chanData[channel].reg[4]);
   ADF5355Send(chanData[channel].reg[3]);
   ADF5355Send(chanData[channel].reg[2]);
   ADF5355Send(chanData[channel].reg[1]);
   delayMicroseconds(256);                   // must be greater than 16 ADC Clocks. This value should be OK.      
   ADF5355Send(chanData[channel].reg[0]);
 }


double ADF5355CalcPFD(double rpfd)
{
  double r = 0;
  bool dub = 0;
  bool div = 0;

  //first try a simple division...
  r = refOsc / rpfd;

  if(fabs(((double) int(r)) - r) < 0.000001 )       //check if this is an integer division
    {
      goto done;
    }

  //next try using the doubler

  dub = 1;
  r = (refOsc * 2.0) / rpfd;
  if(fabs(((double) int(r)) - r) < 0.000001 )       //check if this is an integer division
    {
      goto done;
    }

  Serial.print("Unable to achieve a PFD of ");
  Serial.print(rpfd, 6);
  Serial.print(" MHz With a Ref Oscillator of ");
  Serial.print(refOsc, 6);
  Serial.println(" MHz");
  Serial.println("PFD has not been changed");
  
   return ADF5355GetPfd();

  done:
  if(r < 1) r = 1;
  ADF5355_R = r;
  ADF5355_DBR = dub;
  ADF5355_RDIV2 = div;

  Serial.print("PFD changed to ");
  Serial.print((refOsc * (1+dub)) / r , 6);
  Serial.println("MHz");
  return ((refOsc * (1+dub)) / r);
}


double ADF5355GetPfd(void)
{
  double pfd = refOsc;
  double r = (double) ADF5355_R; 
  if(ADF5355_DBR == 1) pfd = pfd * 2.0;
  pfd = pfd / r;
  if (ADF5355_RDIV2 == 1) pfd = pfd / 2.0;
  return pfd;
}

 void ADF5355SetFrequency(double direct)
 {
   bool freqOK = false;
   double freq;
   double pfd;
   double n;
   double part;
   double err;
   int nom;
   int den;
   int bestnom;
   int bestden;

   pfd = ADF5355GetPfd();

   freqOK = false;
   if(direct ==0)
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

       if((freq > 53.125) && (freq <= 13600.000))
         {
           freqOK = true;
           if(freq > 6800.000)
            {
               Serial.println("Frequencies above 6800 MHz will use Output B");
            }
         }
       else
         {
           Serial.println("Synthesiser Frequency must be between 53.125 and 13600 MHz");
         }
     }
    }
    else
    {
     freq = direct;
    }

 //Calculate the required output divider
   if((freq > 6800.000) && (freq <= 13600.000))  //use the built in doubler
    {
      ADF5355_RFDIV = 0;
      ADF5355_RFBDIS = 0; 
      ADF5355_RFAEN = 0; 
      freq=freq / 2.000;   
    }
   else
   {
    ADF5355_RFBDIS = 1; 
    ADF5355_RFAEN = 1;
     if((freq >= 3400.000) && (freq <= 6800.000))  ADF5355_RFDIV = 0;
     if((freq >= 1700.000) && (freq < 3400.000))  ADF5355_RFDIV = 1;
     if((freq >= 850.000) && (freq < 1700.000))  ADF5355_RFDIV = 2;
     if((freq >= 425.000) && (freq < 850.000))  ADF5355_RFDIV = 3; 
     if((freq >= 212.500) && (freq < 425.000))  ADF5355_RFDIV = 4;
     if((freq >= 106.250) && (freq < 212.500))  ADF5355_RFDIV = 5;
     if((freq >= 53.125) && (freq < 106.250))  ADF5355_RFDIV = 6;
   
    //adjust the frequency to allow for the output division
     freq = freq * (1 << ADF5355_RFDIV);
   }

   n = freq / pfd;

   ADF5355_INT = (unsigned int) n;
   n = n - (unsigned int) n;

// n now has the desired fractional part. 
// first calculate the FRAC1 part which has a fixed Denumerator (MOD1) of 16777216
   
    part = n * 16777216.000;

    ADF5355_FRAC1 = (uint32_t) part;

    n = part - (uint32_t) part;

// n now has the remaining fractional part
// now calculate the best FRAC2 and MOD2 to try to achieve a zero frequency error

    bestnom = 0;
    bestden = 2;
    err = 99.00;

      for(den = 2; den <= 16383 ; den++)
      {
       nom = int((den * n) + 0.0000000001);
       if(nom < 1) continue;

       part = (double) nom / (double) den;
       if(abs(part-n) < 0.0000000001)
         {
           bestnom = nom;
           bestden = den;
           err = 0;
           goto done;
         }
       if(abs((part - n)) < err)
         {
             bestnom = nom;
             bestden = den;
             err = abs(part - n);
         }
      }

   done:
 //program the remaining fractional part
   ADF5355_MOD2 = bestden;
   ADF5355_FRAC2 = bestnom;

   ADF5355EncodeRegs();
 }


void ADF5355CalcFreq(void)
{
  double pfd;
  double vco;
  double n;
  double m1;
  double m2;
  double f1;
  double f2;
  double nf;

  uint8_t diva;

  diva = 1 << ADF5355_RFDIV;

  n = (double) ADF5355_INT;
  m1 = 16777216.000;
  f1 = (double) ADF5355_FRAC1;
  m2 = (double) ADF5355_MOD2;
  f2 = (double) ADF5355_FRAC2;

  Serial.println();
  Serial.print("Chip type is ");
  Serial.println(chipName[chip]);
  Serial.print("Reference Oscillator = ");
  Serial.print(refOsc,10);
  Serial.println(" MHz");

  pfd = ADF5355GetPfd();

  Serial.print("PFD = ");
  Serial.print(pfd , 10);
  Serial.println(" MHz");  

  if(ADF5355_FBS == 1)
    {
      nf = n + ((f1 + f2/m2)/m1);
    }
  else
    {
      nf = (n + ((f1 + f2/m2) / m1)) * diva;
    }


  vco = pfd * nf;

  Serial.print("N = ");
  Serial.print(nf , 10);
  Serial.print(" ( ");
  Serial.print(int(n));
  Serial.print(" + ((");
  Serial.print(int(f1));
  Serial.print(" + (");
  Serial.print(int(f2));
  Serial.print("/");
  Serial.print(int(m2));
  Serial.print(")) ");
  Serial.print("/");
  Serial.print(int(m1));
  Serial.println(") )");;

  Serial.print("VCO = ");
  Serial.print(vco , 10);
  Serial.println(" MHz");

  if(ADF5355_RFAEN == 1)
  {
   Serial.print("O/P Divider = /");
   Serial.println(diva);
   Serial.print("Output Frequency = ");
   Serial.print(vco / diva , 10);
   Serial.print(" MHz");
   Serial.println(" on O/P A");
   if(chanData[channel].extMult > 1)
    {
      Serial.print("External Multipler = X");
      Serial.println(chanData[channel].extMult);
      Serial.print("Final Multiplied Frequency = ");
      Serial.print((vco / diva) * (double) chanData[channel].extMult, 10);
      Serial.println(" MHz"); 
    }
  }

  if(ADF5355_RFBDIS == 0)
  {
   Serial.print("Output Frequency = ");
   Serial.print(vco * 2.00, 10);
   Serial.print(" MHz");
   Serial.println(" on O/P B");
   if(chanData[channel].extMult > 1)
    {
      Serial.print("External Multipler = X");
      Serial.println(chanData[channel].extMult);
      Serial.print("Final Multiplied Frequency = ");
      Serial.print((vco * 2.00) * (double) chanData[channel].extMult, 10);
      Serial.println(" MHz"); 
    }
  }



}
  
double ADF5355GetFrequency(void)
{
  double pfd;
  double vco;
  double n;
  double m1;
  double f1;
  double m2;
  double f2;
  double nf;

  uint8_t diva;

  diva = 1 << ADF5355_RFDIV;

  n = (double) ADF5355_INT;
  m1 = 16777216;
  f1 = (double) ADF5355_FRAC1;
  m2 = (double) ADF5355_MOD2;
  f2 = (double) ADF5355_FRAC2;

  pfd = ADF5355GetPfd();

  if(ADF5355_FBS == 1)
    {
      nf = n + ((f1 + f2/m2)/m1);
    }
  else
    {
      nf = (n + ((f1 + f2/m2) / m1)) * diva;
    }

  vco = pfd * nf;

  if(ADF5355_RFBDIS == 0)
  {
    return vco * 2.00;
  }
  else
  {
    return vco / diva;
  }

}

  void ADF5355jtShift(uint8_t val)
{
  static uint8_t lastval;
  
  if(val == lastval) return;
  lastval = val;

   ADF5355Send((chanData[channel].reg[2] & 0x0F) | ( jtDen[val] << 4));
   ADF5355Send((chanData[channel].reg[1] & 0xF000000F) | ( jtNum[val] << 4));
   ADF5355Send((chanData[channel].reg[0] & 0xFFF0000F) | ( jtN[val] << 4));
}


  void ADF5355FskKey(bool key)
{
  static bool lastkey;
  
  if(key == lastkey) return;
  lastkey = key;

  if(key)
    {
      ADF5355Send(chanData[channel].reg[2]);
      ADF5355Send(chanData[channel].reg[1]);
      ADF5355Send(chanData[channel].reg[0]);
    }
  else
    {
     ADF5355Send((chanData[channel].reg[2] & 0x0F) | ( cwidKeyUpDen << 4));
     ADF5355Send((chanData[channel].reg[1] & 0xF000000F) | ( cwidKeyUpNum << 4));
     ADF5355Send((chanData[channel].reg[0] & 0xFFF0000F) | ( cwidKeyUpN << 4));
    }
}

void ADF5355ExtKey(bool key)
{
  if(key)
    {
      ADF5355Send(chanData[channel].reg[2]);
      ADF5355Send(chanData[channel].reg[1]);
      ADF5355Send(chanData[channel].reg[0]);
    }
  else
    {
     ADF5355Send((chanData[channel].reg[2] & 0x0F) | (ExtKeyUpDen << 4));
     ADF5355Send((chanData[channel].reg[1] & 0xF000000F) | (ExtKeyUpNum << 4));
     ADF5355Send((chanData[channel].reg[0] & 0xFFF0000F) | (ExtKeyUpN << 4));
    }
}

void ADF5355SaveFskShift(void)
{
  cwidKeyUpN = ADF5355_INT;
  cwidKeyUpDen = ADF5355_FRAC2 << 14 | ADF5355_MOD2;
  cwidKeyUpNum = ADF5355_FRAC1;
}

void ADF5355SaveKeyShift(void)
{
  ExtKeyUpN = ADF5355_INT;
  ExtKeyUpDen = ADF5355_FRAC2 << 14 | ADF5355_MOD2;
  ExtKeyUpNum =  ADF5355_FRAC1;
}

void ADF5355SaveJt(uint8_t index)
{
  jtN[index] = ADF5355_INT;
  jtDen[index] = ADF5355_FRAC2 << 14 | ADF5355_MOD2;
  jtNum[index] = ADF5355_FRAC1;
}
