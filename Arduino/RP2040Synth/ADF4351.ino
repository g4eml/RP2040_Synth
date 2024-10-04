//Chip specific functions for ADF4351 device

#define ADF4351CEPin 3         //GPO 3 Connect to ADf4351 CE Pin
#define ADF4351MUXPin 4         //GPO 4 Connect to ADF4351 MUX Pin
#define ADF4351LEPin 5         //GPO 5 Connect to ADF4351 LE Pin
#define ADF4351CKPin 6         //GPO 6 Connect to ADF4351 CLK Pin
#define ADF4351DATPin 7         //GPO 7 Connect to ADF4351 DAT Pin



//ADF4351 Register bits.
//REG 0
unsigned int ADF4351_INT;
unsigned int ADF4351_FRAC;

//REG 1
bool ADF4351_PH;
bool ADF4351_PR;
unsigned int ADF4351_P;
unsigned int ADF4351_M;

//REG 2
byte ADF4351_NM;
byte ADF4351_MUX;
bool ADF4351_DBR;
bool ADF4351_RDIV2;
unsigned int ADF4351_R;
bool ADF4351_REG4DB;
byte ADF4351_CP;
bool ADF4351_LDF;
bool ADF4351_LDP;
bool ADF4351_PDP;
bool ADF4351_PWDN;
bool ADF4351_TRI;
bool ADF4351_RST;

//REG 3
bool ADF4351_BSM;
bool ADF4351_ABP;
bool ADF4351_CC;
bool ADF4351_CSR;
byte ADF4351_CDM;
unsigned int ADF4351_CDIV;

//REG 4
bool ADF4351_FB;
byte ADF4351_RFDIV;
byte ADF4351_BS;
bool ADF4351_VCOPD;
bool ADF4351_MTLD;
bool ADF4351_AUXSEL;
bool ADF4351_AUXEN;
byte ADF4351_AUXPWR;
bool ADF4351_RFEN;
byte ADF4351_RFPWR;

//REG 5
byte ADF4351_LD = 1;

void ADF4351SetDefault(void)
{
  //ADF4351 Register bits. Default settings give output at 435.100 MHz with 10MHz PFD.
  //REG 0
  ADF4351_INT = 348;
  ADF4351_FRAC = 8;

  //REG 1
  ADF4351_PH = 0;
  ADF4351_PR = 0;
  ADF4351_P = 1;
  ADF4351_M = 100;

  //REG 2
  ADF4351_NM = 0;
  ADF4351_MUX = 6;
  ADF4351_DBR = 1;
  ADF4351_RDIV2 = 0;
  ADF4351_R = 5;
  ADF4351_REG4DB = 0;
  ADF4351_CP = 7;
  ADF4351_LDF = 0;
  ADF4351_LDP = 0;
  ADF4351_PDP = 1;
  ADF4351_PWDN = 0;
  ADF4351_TRI = 0;
  ADF4351_RST = 0;

  //REG 3
  ADF4351_BSM = 1;
  ADF4351_ABP = 0;
  ADF4351_CC = 0;
  ADF4351_CSR = 0;
  ADF4351_CDM = 0;
  ADF4351_CDIV = 0;

  //REG 4
  ADF4351_FB = 1;
  ADF4351_RFDIV = 3;
  ADF4351_BS = 80;
  ADF4351_VCOPD = 0;
  ADF4351_MTLD = 0;
  ADF4351_AUXSEL = 0;
  ADF4351_AUXEN = 0;
  ADF4351_AUXPWR = 0;
  ADF4351_RFEN = 1;
  ADF4351_RFPWR = 3;

  //REG 5
  ADF4351_LD = 1;

  ADF4351SetFrequency(0);
  ADF4351EncodeRegs();
  ADF4351Update();

}

void ADF4351SetParameters(void)
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
        Serial.println("Valid Parameter Names (See Max 2870 Data Sheet) are:-");
        Serial.println("Reg0 = INT FRAC");
        Serial.println("Reg1 = PH PR P M"); 
        Serial.println("Reg2 = NM MUX DBR RDIV2 R REG4DB CP LDF LDP PDP PWDN TRI RST"); 
        Serial.println("Reg3 = BSM ABP CC CSR CDM CDIV");
        Serial.println("Reg4 = FB RFDIV BS VCOPD MTLD AUXSEL AUXEN AUXPWR RFEN RFPWR");
        Serial.println("Reg5 = LD");
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
      if(paramUint(param , "INT" , &ADF4351_INT , value , 23 , 65535)) goto handled;
      if(paramUint(param , "FRAC" , &ADF4351_FRAC , value , 0 , 4095)) goto handled;

//REG1
      if(paramBool(param , "PH" , &ADF4351_PH , value)) goto handled;
      if(paramBool(param , "PR" , &ADF4351_PR , value)) goto handled;
      if(paramUint(param , "P" , &ADF4351_P , value , 0 , 4095)) goto handled;
      if(paramUint(param , "M" , &ADF4351_M , value , 2 , 4095)) goto handled;

//REG2
      if(paramByte(param , "NM" , &ADF4351_NM, value , 0 , 3)) goto handled;
      if(paramByte(param , "MUX" , &ADF4351_MUX , value , 0 , 7)) goto handled;
      if(paramBool(param , "DBR" , &ADF4351_DBR , value)) goto handled;
      if(paramBool(param , "RDIV2" , &ADF4351_RDIV2 , value)) goto handled;
      if(paramUint(param , "R" , &ADF4351_R , value , 1 , 1023)) goto handled;
      if(paramBool(param , "REG4DB" , &ADF4351_REG4DB , value)) goto handled;
      if(paramByte(param , "CP" , &ADF4351_CP , value , 0 , 15)) goto handled;
      if(paramBool(param , "LDF" , &ADF4351_LDF , value)) goto handled;
      if(paramBool(param , "LDP" , &ADF4351_LDP , value)) goto handled;
      if(paramBool(param , "PDP" , &ADF4351_PDP , value)) goto handled;
      if(paramBool(param , "PWDN" , &ADF4351_PWDN , value)) goto handled;
      if(paramBool(param , "TRI" , &ADF4351_TRI , value)) goto handled;
      if(paramBool(param , "RST" , &ADF4351_RST , value)) goto handled;

//REG3
      if(paramBool(param , "BSM" , &ADF4351_BSM , value)) goto handled;
      if(paramBool(param , "ABP" , &ADF4351_ABP , value)) goto handled;
      if(paramBool(param , "CC" , &ADF4351_CC , value)) goto handled;
      if(paramBool(param , "CSR" , &ADF4351_CSR , value)) goto handled;
      if(paramByte(param , "CDM" , &ADF4351_CDM , value , 0 , 3)) goto handled;
      if(paramUint(param , "CDIV" , &ADF4351_CDIV , value , 0 , 4095)) goto handled;

//REG4
      if(paramBool(param , "FB" , &ADF4351_FB , value)) goto handled;
      if(paramByte(param , "RFDIV" , &ADF4351_RFDIV , value , 0 , 6)) goto handled;
      if(paramByte(param , "BS" , &ADF4351_BS , value , 1 , 255)) goto handled;
      if(paramBool(param , "VCOPD" , &ADF4351_VCOPD , value)) goto handled;
      if(paramBool(param , "MTLD" , &ADF4351_MTLD , value)) goto handled;
      if(paramBool(param , "AUXSEL" , &ADF4351_AUXSEL , value)) goto handled;
      if(paramBool(param , "AUXEN" , &ADF4351_AUXEN , value)) goto handled;
      if(paramByte(param , "AUXPWR" , &ADF4351_AUXPWR , value , 0 , 3)) goto handled;
      if(paramBool(param , "RFEN" , &ADF4351_RFEN , value)) goto handled;
      if(paramByte(param , "RFPWR" , &ADF4351_RFPWR , value , 0 , 3)) goto handled;

//REG5
      if(paramByte(param , "LD" , &ADF4351_LD , value , 0 , 3)) goto handled;


    handled:
    ADF4351EncodeRegs();
    ADF4351Update();

  }
}


void ADF4351Init(void)
{
  numberOfRegs = 6;                   //number of registers in the current chip type
  numberOfBits = 32;                   //number of bits in each register
  maxPfd = 35.0;
  jt4Only = true;
  pinMode(ADF4351CEPin,OUTPUT);
  digitalWrite(ADF4351CEPin,HIGH); 
  pinMode(ADF4351LEPin,OUTPUT);
  digitalWrite(ADF4351LEPin,HIGH);
  SPI.setRX(ADF4351MUXPin);
  SPI.setTX(ADF4351DATPin);
  SPI.setSCK(ADF4351CKPin);
  SPI.beginTransaction(SPISettings(40000,MSBFIRST,SPI_MODE0));
  SPI.begin();
}

void ADF4351Send(int32_t val)
{
  int a=(val >> 24) & 0xFF;
  int b=(val >> 16) & 0xFF;
  int c=(val >> 8) & 0xFF;
  int d=val & 0xFF;
  
  digitalWrite(ADF4351LEPin,LOW);
  delayMicroseconds(10);
  SPI.transfer((byte)a);
  SPI.transfer((byte)b);
  SPI.transfer((byte)c);
  SPI.transfer((byte)d);
  delayMicroseconds(10);
  digitalWrite(ADF4351LEPin,HIGH);
  delayMicroseconds(10);  
}


void ADF4351EncodeRegs(void)
{
  chanData[channel].reg[0] = (ADF4351_INT << 15) | (ADF4351_FRAC << 3);
  chanData[channel].reg[1] = (ADF4351_PH << 28) | (ADF4351_PR << 27) | (ADF4351_P << 15) | (ADF4351_M << 3) | 1 ;
  chanData[channel].reg[2] = (ADF4351_NM << 29) | (ADF4351_MUX << 26) | (ADF4351_DBR << 25) | (ADF4351_RDIV2 << 24) | (ADF4351_R << 14) | (ADF4351_REG4DB <<13) | (ADF4351_CP << 9) | (ADF4351_LDF << 8) | (ADF4351_LDP << 7) | (ADF4351_PDP << 6) | (ADF4351_PWDN << 5) | (ADF4351_TRI << 4) | (ADF4351_RST << 3) | 2;
  chanData[channel].reg[3] = (ADF4351_BSM << 23) | (ADF4351_ABP << 22) | (ADF4351_CC << 21) | (ADF4351_CSR << 18) | (ADF4351_CDM << 15) | (ADF4351_CDIV << 3) | 3;
  chanData[channel].reg[4] = (ADF4351_FB << 23) | (ADF4351_RFDIV << 20) | (ADF4351_BS << 12) | (ADF4351_VCOPD << 11) | (ADF4351_MTLD << 10) | (ADF4351_AUXSEL << 9) | (ADF4351_AUXEN << 8) | (ADF4351_AUXPWR << 6) | (ADF4351_RFEN << 5) | (ADF4351_RFPWR << 3) | 4;
  chanData[channel].reg[5] = (ADF4351_LD << 22) | (3 << 19) | 5; 
}

void ADF4351DecodeRegs(void)
{
  ADF4351_INT = (chanData[channel].reg[0] >> 15) & 0xFFFF;
  ADF4351_FRAC = (chanData[channel].reg[0] >> 3) & 0x0FFF;

  ADF4351_PH = (chanData[channel].reg[1] >> 28) & 0x01;
  ADF4351_PR = (chanData[channel].reg[1] >> 27) & 0x01;
  ADF4351_P = (chanData[channel].reg[1] >> 15) & 0xFFF; 
  ADF4351_M = (chanData[channel].reg[1] >> 3) & 0xFFF;

  ADF4351_NM = (chanData[channel].reg[2] >> 29) & 0x03;
  ADF4351_MUX = (chanData[channel].reg[2] >> 26) & 0x07;
  ADF4351_DBR = (chanData[channel].reg[2] >> 25) & 0x01;
  ADF4351_RDIV2 = (chanData[channel].reg[2] >> 24) & 0x01;
  ADF4351_R = (chanData[channel].reg[2] >> 14) & 0x3FF;
  ADF4351_REG4DB = (chanData[channel].reg[2] >> 13) & 0x01;
  ADF4351_CP = (chanData[channel].reg[2] >> 9) & 0x0F;
  ADF4351_LDF = (chanData[channel].reg[2] >> 8) & 0x01;
  ADF4351_LDP = (chanData[channel].reg[2] >> 7) & 0x01;
  ADF4351_PDP = (chanData[channel].reg[2] >> 6) & 0x01;
  ADF4351_PWDN = (chanData[channel].reg[2] >> 5) & 0x01;
  ADF4351_TRI = (chanData[channel].reg[2] >> 4) & 0x01;
  ADF4351_RST = (chanData[channel].reg[2] >> 3) & 0x01;

  ADF4351_BSM = (chanData[channel].reg[3] >> 23) & 0x01;
  ADF4351_ABP = (chanData[channel].reg[3] >> 22) & 0x01;
  ADF4351_CC = (chanData[channel].reg[3] >> 21) & 0x01;
  ADF4351_CSR = (chanData[channel].reg[3] >> 18) & 0x01;
  ADF4351_CDM = (chanData[channel].reg[3] >> 15) & 0x03;
  ADF4351_CDIV = (chanData[channel].reg[3] >> 3) & 0xFFF;

  ADF4351_FB = (chanData[channel].reg[4] >> 23) & 0x01;
  ADF4351_RFDIV = (chanData[channel].reg[4] >> 20) & 0x07;
  ADF4351_BS = (chanData[channel].reg[4] >> 12) & 0xFF;
  ADF4351_VCOPD = (chanData[channel].reg[4] >> 11) & 0x01;
  ADF4351_MTLD = (chanData[channel].reg[4] >> 10) & 0x01;
  ADF4351_AUXSEL = (chanData[channel].reg[4] >> 9) & 0x01;
  ADF4351_AUXEN = (chanData[channel].reg[4] >> 8) & 0x01;
  ADF4351_AUXPWR = (chanData[channel].reg[4] >> 6) & 0x03;
  ADF4351_RFEN = (chanData[channel].reg[4] >> 5) & 0x01;
  ADF4351_RFPWR = (chanData[channel].reg[4] >> 3) & 0x03;

  ADF4351_LD = (chanData[channel].reg[5] >> 22) & 0x03;

}

void ADF4351Update(void)
{
  ADF4351Send(chanData[channel].reg[5]);
  ADF4351Send(chanData[channel].reg[4]);
  ADF4351Send(chanData[channel].reg[3]);
  ADF4351Send(chanData[channel].reg[2]);
  ADF4351Send(chanData[channel].reg[1]);
  ADF4351Send(chanData[channel].reg[0]);
}


double ADF4351CalcPFD(double rpfd)
{
  double r = 0;
  bool dub = 0;
  bool div = 0;

  //first try a simple division...
  r = refOsc / rpfd;

  if(((double) int(r))  == r)       //check if this is an integer division
    {
      goto done;
    }

  //next try using the doubler

  dub = 1;
  r = (refOsc * 2.0) / rpfd;
  if(((double) int(r)) == r)       //check if this is an integer division
    {
      goto done;
    }

  r = int(r);

  if(r < 1) r=1;

  Serial.print("Unable to acheive a PFD of ");
  Serial.print(rpfd, 6);
  Serial.print(" MHz With a Ref Oscillator of ");
  Serial.print(refOsc, 6);
  Serial.println(" MHz");
  Serial.print("PFD set to ");
  Serial.print((refOsc * 2.0) / r, 6);
  Serial.println(" MHz");

  done:
  if(r < 1) r = 1;
  ADF4351_R = r;
  ADF4351_DBR = dub;
  ADF4351_RDIV2 = div;

  return ((refOsc * (1+dub)) / r);
}

void ADF4351SetPfd(void)
{
  double pfd;
  bool freqOK;
  
  freqOK = false;
  
  while(!freqOK)
    {
       Serial.printf("\nEnter required PFD in MHz -->");
       pfd = inputFloat();
       if(pfd == 0) return;
       pfd = ADF4351CalcPFD(pfd);
      if(pfd <= maxPfd)
        {
          freqOK = true;
        }
      else
        {
          Serial.print("\nPFD must be less than ");
          Serial.print(maxPfd);
          Serial.println(" MHz");
        }
      
    }
   
}

double ADF4351GetPfd(void)
{
  double pfd = refOsc;
  double r = (double) ADF4351_R; 
  if(ADF4351_DBR == 1) pfd = pfd * 2.0;
  pfd = pfd / r;
  if (ADF4351_RDIV2 == 1) pfd = pfd / 2.0;
  return pfd;
}

void ADF4351SetFrequency(double direct)
{
  bool freqOK = false;
  char resp;

  double freq;
  double pfd;
  double n;
  double part;
  double err;
  int nom;
  int den;
  int bestnom;
  int bestden;

  pfd = ADF4351GetPfd();

  freqOK = false;
  if(direct ==0)
   {
   resp = getSelection("Is there an external Multiplier chain? Y or N --->");
   if((resp == 'Y') || (resp == 'y'))
     {
        Serial.print("Enter External Multiplication Factor ---> ");
        chanData[channel].extMult = inputNumber();
     }
   else
     {
       chanData[channel].extMult = 1;
     }
    while(!freqOK)
    {
      Serial.print("\nEnter required Final Frequency in MHz -->");
      freq = inputFloat() / (double) chanData[channel].extMult;
      if((freq > 34.375) && (freq <= 4400.000))
        {
          freqOK = true;
        }
      else
        {
          Serial.println("Synthesiser Frequency must be between 34.375 and 4400 MHz");
        }
    }
   }
   else
   {
    freq = direct;
   }

//Calculate the required output divider
  if((freq >= 2200.000) && (freq <= 4400.000))  ADF4351_RFDIV = 0;
  if((freq >= 1100.000) && (freq < 2200.000))  ADF4351_RFDIV = 1;
  if((freq >= 550.000) && (freq < 1100.000))  ADF4351_RFDIV = 2;
  if((freq >= 275.000) && (freq < 550.000))  ADF4351_RFDIV = 3; 
  if((freq >= 137.500) && (freq < 275.000))  ADF4351_RFDIV = 4;
  if((freq >= 68.750) && (freq < 13.7500))  ADF4351_RFDIV = 5;
  if((freq >= 34.375) && (freq < 68.750))  ADF4351_RFDIV = 6;
  

//adjust the frequency to allow for the output division
  freq = freq * (1 << ADF4351_RFDIV);

  n = freq / pfd;

  ADF4351_INT = (unsigned int) n;
  n = n - (unsigned int) n;

  //n now has the desired fractional part. Now we need to work put the best f/d to acheive this
   err = 1.0;
   bestnom = 0;
   bestden = 2;

     for(den = 2; den <= 4095 ; den++)
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
//program the fraction division
  ADF4351_M = bestden;
  ADF4351_FRAC = bestnom;

  ADF4351EncodeRegs();
}


void ADF4351CalcFreq(void)
{
  double pfd;
  double vco;
  double n;
  double m;
  double f;
  double nf;

  uint8_t diva;

  diva = 1 << ADF4351_RFDIV;

  n = (double) ADF4351_INT;
  m = (double) ADF4351_M;
  f = (double) ADF4351_FRAC;

  Serial.println();
  Serial.print("Reference Oscillator = ");
  Serial.print(refOsc,10);
  Serial.println(" MHz");

  pfd = ADF4351GetPfd();

  Serial.print("PFD = ");
  Serial.print(pfd , 10);
  Serial.println(" MHz");

  if(ADF4351_FB == 1)
    {
      nf = n + (f/m);
    }
  else
    {
      nf = (n + (f / m)) * diva;
    }


  vco = pfd * nf;

  Serial.print("N = ");
  Serial.print(nf , 10);
  Serial.print(" ( ");
  Serial.print(int(n));
  Serial.print(" + (");
  Serial.print(int(f));
  Serial.print("/");
  Serial.print(int(m));
  Serial.println(") )");;

  Serial.print("VCO = ");
  Serial.print(vco , 10);
  Serial.println(" MHz");

  Serial.print("O/P Divider = /");
  Serial.println(diva);

  Serial.print("Output Frequency = ");
  Serial.print(vco / diva , 10);
  Serial.println(" MHz");

  if(chanData[channel].extMult > 1)
    {
      Serial.print("Final Multiplied Frequency = ");
      Serial.print((vco / diva) * (double) chanData[channel].extMult, 10);
      Serial.println(" MHz"); 
    }
}
  
double ADF4351GetFrequency(void)
{
  double pfd;
  double vco;
  double n;
  double m;
  double f;
  double nf;

  uint8_t diva;

  diva = 1 << ADF4351_RFDIV;

  n = (double) ADF4351_INT;
  m = (double) ADF4351_M;
  f = (double) ADF4351_FRAC;

  pfd = ADF4351GetPfd();

  if(ADF4351_FB == 1)
    {
      nf = n + (f/m);
    }
  else
    {
      nf = (n + (f / m)) * diva;
    }

  vco = pfd * nf;

  return vco / diva;
}

  void ADF4351jtShift(uint8_t val)
{
  static uint8_t lastval;
  
  if(val == lastval) return;
  lastval = val;

  ADF4351Send((chanData[channel].reg[1] & 0xFFFF8007) | (jtDen[val] << 3));
  ADF4351Send((chanData[channel].reg[0] & 0x80000007) | (jtN[val] << 15) | (jtNum[val] <<3));
}


  void ADF4351FskKey(bool key)
{
  static bool lastkey;
  
  if(key == lastkey) return;
  lastkey = key;

  if(key)
    {
      ADF4351Send(chanData[channel].reg[1]);
      ADF4351Send(chanData[channel].reg[0]);
    }
  else
    {
     ADF4351Send((chanData[channel].reg[1] & 0xFFFF8007) | (cwidKeyUpDen << 3));
     ADF4351Send((chanData[channel].reg[0] & 0x80000007) | (cwidKeyUpN << 15) | (cwidKeyUpNum <<3));
    }
}

void ADF4351ExtKey(bool key)
{
  if(key)
    {
      ADF4351Send(chanData[channel].reg[1]);
      ADF4351Send(chanData[channel].reg[0]);
    }
  else
    {
     ADF4351Send((chanData[channel].reg[1] & 0xFFFF8007) | (ExtKeyUpDen << 3));
     ADF4351Send((chanData[channel].reg[0] & 0x80000007) | (ExtKeyUpN << 15) | (ExtKeyUpNum <<3));
    }
}

void ADF4351SaveFskShift(void)
{
  cwidKeyUpN = ADF4351_INT;
  cwidKeyUpDen = ADF4351_M;
  cwidKeyUpNum = ADF4351_FRAC;
}

void ADF4351SaveKeyShift(void)
{
  ExtKeyUpN = ADF4351_INT;
  ExtKeyUpDen = ADF4351_M;
  ExtKeyUpNum = ADF4351_FRAC;
}

void ADF4351SaveJt(uint8_t index)
{
  jtN[index] = ADF4351_INT;
  jtDen[index] = ADF4351_M;
  jtNum[index] = ADF4351_FRAC;
}
