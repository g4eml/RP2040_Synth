//Chip specific functions for MAX2780 device

#define MAX2870CEPin 3         //GPO 3 Connect to Max2870  CE Pin
#define MAX2870MUXPin 4         //GPO 4 Connect to Max2870  MUX_OUT Pin         
#define MAX2870LEPin 5         //GPO 5 Connect to Max2870  LE Pin
#define MAX2870CKPin 6         //GPO 6 Connect to Max2870  CLK Pin 
#define MAX2870DATPin 7         //GPO 7 Connect to Max2870  DATA Pin



//MAX2870 Register bits. Default settings give output at 435.100 MHz with 100MHz reference clock 4MHz PFD.

//Reg 0
bool Max2870_INT;
unsigned int Max2870_N;
unsigned int Max2870_FRAC;

//Reg 1
bool Max2870_CPOC;
byte Max2870_CPL;
byte Max2870_CPT;
unsigned int Max2870_P;
unsigned int Max2870_M;

//Reg 2
bool Max2870_LDS;
byte Max2870_SDN;
byte Max2870_MUX;
bool Max2870_DBR;
bool Max2870_RDIV2;
unsigned int Max2870_R;
bool Max2870_REG4DB;
byte Max2870_CP;
bool Max2870_LDF;
bool Max2870_LDP;
bool Max2870_PDP;
bool Max2870_SHDN;
bool Max2870_TRI;
bool Max2870_RST;

//Reg 3
byte Max2870_VCO;
bool Max2870_VAS_SHDN;
bool Max2870_RETUNE;
byte Max2870_CDM;
unsigned int Max2870_CDIV;

//Reg 4
byte Max2870_BS_MSBS;
bool Max2870_FB;
byte Max2870_DIVA;
byte Max2870_BS;
bool Max2870_BDIV;
bool Max2870_RFB_EN;
byte Max2870_BPWR;
bool Max2870_RFA_EN;
byte Max2870_APWR;

//Reg 5
bool Max2870_F01;
byte Max2870_LD;
bool Max2870_MUX_MSB;

void Max2870SetDefault(void)
{
    //Reg 0
  Max2870_INT = 0;
  Max2870_N = 348;
  Max2870_FRAC = 8;

  //Reg 1
  Max2870_CPOC = 0;
  Max2870_CPL = 0;
  Max2870_CPT = 0;
  Max2870_P = 1;
  Max2870_M = 100;

  //Reg 2
  Max2870_LDS = 0;
  Max2870_SDN = 0;
  Max2870_MUX = 6;
  Max2870_DBR = 0;
  Max2870_RDIV2 = 0;
  Max2870_R = 10;
  Max2870_REG4DB = 0;
  Max2870_CP = 15;
  Max2870_LDF = 0;
  Max2870_LDP = 0;
  Max2870_PDP = 1;
  Max2870_SHDN = 0;
  Max2870_TRI = 0;
  Max2870_RST = 0;

  //Reg 3
  Max2870_VCO = 0;
  Max2870_VAS_SHDN = 0;
  Max2870_RETUNE = 0;
  Max2870_CDM = 0;
  Max2870_CDIV = 1;

  //Reg 4
  Max2870_BS_MSBS = 0;
  Max2870_FB = 1;
  Max2870_DIVA = 3;
  Max2870_BS = 80;
  Max2870_BDIV = 0;
  Max2870_RFB_EN = 0;
  Max2870_BPWR = 0;
  Max2870_RFA_EN = 1;
  Max2870_APWR = 3;

  //Reg 5
  Max2870_F01 = 0;
  Max2870_LD = 1;
  Max2870_MUX_MSB = 0;

  Max2870SetFrequency(0);

  Max2870EncodeRegs();
  Max2870Update();
}

void Max2870SetParameters(void)
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
        Serial.println("Reg0 = INT N FRAC");
        Serial.println("Reg1 = CPOC CPL CPT P M"); 
        Serial.println("Reg2 = LDS SDN MUX DBR RDIV2 R REG4DB CP LDF LDP PDP SHDN TRI RST"); 
        Serial.println("Reg3 = VCO VAS_SHDN RETUNE CDM CDIV");
        Serial.println("Reg4 = BS_MSBS FB DIVA BS BDIV RFB_EN BPWR RFA_EN APWR");
        Serial.println("Reg5 = F01 LD MUX_MSB");
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
      if(paramBool(param , "INT" , &Max2870_INT , value)) goto handled;
      if(paramUint(param , "N" , &Max2870_N , value , (Max2870_INT == 0) ? 19:16 , (Max2870_INT == 0) ? 4091:65535)) goto handled;
      if(paramUint(param , "FRAC" , &Max2870_FRAC , value , 0 , 4095)) goto handled;

//REG1
      if(paramBool(param , "CPOC" , &Max2870_CPOC , value)) goto handled;
      if(paramByte(param , "CPL" , &Max2870_CPL , value , 0 , 3)) goto handled;
      if(paramByte(param , "CPT" , &Max2870_CPT , value , 0 , 3)) goto handled;
      if(paramUint(param , "P" , &Max2870_P , value , 0 , 4095)) goto handled;
      if(paramUint(param , "M" , &Max2870_M , value , 2 , 4095)) goto handled;

//REG2
      if(paramBool(param , "LDS" , &Max2870_LDS , value)) goto handled;
      if(paramByte(param , "SDN" , &Max2870_SDN , value , 0 , 3)) goto handled;
      if(paramByte(param , "MUX" , &Max2870_MUX , value , 0 , 15)) goto handled;
      if(paramBool(param , "DBR" , &Max2870_DBR , value)) goto handled;
      if(paramBool(param , "RDIV2" , &Max2870_RDIV2 , value)) goto handled;
      if(paramUint(param , "R" , &Max2870_R , value , 1 , 1023)) goto handled;
      if(paramBool(param , "REG4DB" , &Max2870_REG4DB , value)) goto handled;
      if(paramByte(param , "CP" , &Max2870_CP , value , 0 , 15)) goto handled;
      if(paramBool(param , "LDF" , &Max2870_LDF , value)) goto handled;
      if(paramBool(param , "LDP" , &Max2870_LDP , value)) goto handled;
      if(paramBool(param , "PDP" , &Max2870_PDP , value)) goto handled;
      if(paramBool(param , "SHDN" , &Max2870_SHDN , value)) goto handled;
      if(paramBool(param , "TRI" , &Max2870_TRI , value)) goto handled;
      if(paramBool(param , "RST" , &Max2870_RST , value)) goto handled;

//REG3
      if(paramByte(param , "VCO" , &Max2870_VCO , value , 0 , 63)) goto handled;
      if(paramBool(param , "VAS_SHDN" , &Max2870_VAS_SHDN , value)) goto handled;
      if(paramBool(param , "RETUNE" , &Max2870_RETUNE , value)) goto handled;
      if(paramByte(param , "CDM" , &Max2870_CDM , value , 0 , 3)) goto handled;
      if(paramUint(param , "CDIV" , &Max2870_CDIV , value , 1 , 4095)) goto handled;

//REG4
      if(paramByte(param , "BS_MSBS" , &Max2870_BS_MSBS , value , 0 , 3)) goto handled;
      if(paramBool(param , "FB" , &Max2870_FB , value)) goto handled;
      if(paramByte(param , "DIVA" , &Max2870_DIVA , value , 0 , 7)) goto handled;
      if(paramByte(param , "BS" , &Max2870_BS , value , 1 , 255)) goto handled;
      if(paramBool(param , "BDIV" , &Max2870_BDIV , value)) goto handled;
      if(paramBool(param , "RFB_EN" , &Max2870_RFB_EN , value)) goto handled;
      if(paramByte(param , "BPWR" , &Max2870_BPWR , value , 0 , 3)) goto handled;
      if(paramBool(param , "RFA_EN" , &Max2870_RFA_EN , value)) goto handled;
      if(paramByte(param , "APWR" , &Max2870_APWR , value , 0 , 3)) goto handled;

//REG5
      if(paramBool(param , "F01" , &Max2870_F01 , value)) goto handled;
      if(paramByte(param , "LD" , &Max2870_LD , value , 0 , 3)) goto handled;
      if(paramBool(param , "MUX_MSB" , &Max2870_MUX_MSB , value)) goto handled;


    handled:
    Max2870EncodeRegs();
    Max2870Update();

  }
}

void Max2870Init(void)
{
  int chip = MAX2780;                  //index to the current chip type
  numberOfRegs = 6;                   //number of registers in the current chip type
  numberOfBits = 32;                   //number of bits in each register
  maxPfd = 105.0;                      //maximum PFD frequency
  pinMode(MAX2870CEPin,OUTPUT);
  digitalWrite(MAX2870CEPin,HIGH); 
  pinMode(MAX2870LEPin,OUTPUT);
  digitalWrite(MAX2870LEPin,HIGH);
  SPI.setRX(MAX2870MUXPin);
  SPI.setTX(MAX2870DATPin);
  SPI.setSCK(MAX2870CKPin);
  SPI.beginTransaction(SPISettings(400000,MSBFIRST,SPI_MODE0));
  SPI.begin();
}

void Max2870Send(int32_t val)
{
  int a=(val >> 24) & 0xFF;
  int b=(val >> 16) & 0xFF;
  int c=(val >> 8) & 0xFF;
  int d=val & 0xFF;
  
  digitalWrite(MAX2870LEPin,LOW);
  delayMicroseconds(10);
  SPI.transfer((byte)a);
  SPI.transfer((byte)b);
  SPI.transfer((byte)c);
  SPI.transfer((byte)d);
  delayMicroseconds(10);
  digitalWrite(MAX2870LEPin,HIGH);
  delayMicroseconds(10);  
}


void Max2870EncodeRegs(void)
{
  reg[0] = (Max2870_INT << 31) | (Max2870_N << 15) | (Max2870_FRAC << 3);
  reg[1] = (Max2870_CPOC << 31) | (Max2870_CPL << 29) | (Max2870_CPT << 27) | (Max2870_P << 15) | (Max2870_M << 3) | 1 ;
  reg[2] = (Max2870_LDS << 31) | (Max2870_SDN << 29) | (Max2870_MUX << 26) | (Max2870_DBR << 25) | (Max2870_RDIV2 << 24) | (Max2870_R << 14) | (Max2870_REG4DB <<13) | (Max2870_CP << 9) | (Max2870_LDF << 8) | (Max2870_LDP << 7) | (Max2870_PDP << 6) | (Max2870_SHDN << 5) | (Max2870_TRI << 4) | (Max2870_RST << 3) | 2;
  reg[3] = (Max2870_VCO << 26) | (Max2870_VAS_SHDN << 25) | (Max2870_RETUNE << 24) | (Max2870_CDM << 15) | (Max2870_CDIV << 3) | 3;
  reg[4] = (6 << 28) | (Max2870_BS_MSBS << 24) | (Max2870_FB << 23) | (Max2870_DIVA << 20) | (Max2870_BS << 12) | (Max2870_BDIV << 9) | (Max2870_RFB_EN << 8) | (Max2870_BPWR << 6) | (Max2870_RFA_EN << 5) | (Max2870_APWR << 3) | 4;
  reg[5] = (Max2870_F01 << 24) | (Max2870_LD << 22) | (Max2870_MUX_MSB << 18) | 5; 
}

void Max2870DecodeRegs(void)
{
  Max2870_INT = (reg[0] >> 31) & 0x01;
  Max2870_N = (reg[0] >> 15) & 0xFFFF;
  Max2870_FRAC = (reg[0] >> 3) & 0xFFF;

  Max2870_CPOC = (reg[1] >> 31) & 0x01;
  Max2870_CPL = (reg[1] >> 29) & 0x03;
  Max2870_CPT = (reg[1] >> 27) & 0x03;
  Max2870_P = (reg[1] >> 15) & 0x0FFF; 
  Max2870_M = (reg[1] >> 3) & 0x0FFF;

  Max2870_LDS = (reg[2] >> 31) & 0x01;
  Max2870_SDN = (reg[2] >> 29) & 0x03;
  Max2870_MUX = (reg[2] >> 26) & 0x0F;
  Max2870_DBR = (reg[2] >> 25) & 0x01; 
  Max2870_RDIV2 = (reg[2] >> 24) & 0x01; 
  Max2870_R = (reg[2] >> 14) & 0x3FF; 
  Max2870_REG4DB = (reg[2] >> 13) & 0x01;
  Max2870_CP = (reg[2] >> 9) & 0x0F;
  Max2870_LDF = (reg[2] >> 8) & 0x01;
  Max2870_LDP = (reg[2] >> 7) & 0x01;
  Max2870_PDP = (reg[2] >> 6) & 0x01;
  Max2870_SHDN = (reg[2] >> 5) & 0x01; 
  Max2870_TRI = (reg[2] >> 4) & 0x01;
  Max2870_RST = (reg[2] >> 3) & 0x01;

  Max2870_VCO = (reg[3] >> 26) & 0x3F;
  Max2870_VAS_SHDN = (reg[3] >> 25) & 0x01;
  Max2870_RETUNE = (reg[3] >> 24) & 0x01;
  Max2870_CDM = (reg[3] >> 15) & 0x03;
  Max2870_CDIV = (reg[3] >> 3) & 0xFFF;

  Max2870_BS_MSBS = (reg[4] >> 24) & 0x03;
  Max2870_FB = (reg[4] >> 23) & 0x01;
  Max2870_DIVA = (reg[4] >> 20) & 0x07;
  Max2870_BS = (reg[4] >> 12) & 0xFF;
  Max2870_BDIV = (reg[4] >> 9) & 0x01;
  Max2870_RFB_EN = (reg[4] >> 8) & 0x01;
  Max2870_BPWR = (reg[4] >> 6) & 0x03;
  Max2870_RFA_EN = (reg[4] >> 5) & 0x01; 
  Max2870_APWR = (reg[4] >> 3) & 0x03;

  Max2870_F01 = (reg[5] >> 24) & 0x01;
  Max2870_LD = (reg[5] >> 22) & 0x03;
  Max2870_MUX_MSB = (reg[5] >> 18) & 0x01;

}

void Max2870Update(void)
{
  Max2870Send(reg[5]);
  Max2870Send(reg[4]);
  Max2870Send(reg[3]);
  Max2870Send(reg[2]);
  Max2870Send(reg[1]);
  Max2870Send(reg[0]);
}


double Max2870CalcPFD(double rpfd)
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

  if(r < 1)  r = 1;

  Serial.print("Unable to acheive a PFD of ");
  Serial.print(rpfd, 6);
  Serial.print(" MHz With a Ref Oscillator of ");
  Serial.print(refOsc, 6);
  Serial.println(" MHz");
  Serial.print("PFD set to ");
  Serial.print((refOsc * 2.0) / r, 6);
  Serial.println(" MHz");

  done:
  if(r < 1)  r = 1;
  Max2870_R = r;
  Max2870_DBR = dub;
  Max2870_RDIV2 = div;

  return ((refOsc * (1+dub)) / r);
}

void Max2870SetPfd(void)
{
  double pfd;
  bool freqOK;
  
  freqOK = false;
  
   while(!freqOK)
    {
       Serial.printf("Enter required PFD in MHz -->");
       pfd = inputFloat();
       if(pfd == 0) return;
       pfd = Max2870CalcPFD(pfd);
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

double Max2870GetPfd(void)
{
  double pfd = refOsc;
  double r = (double) Max2870_R;
  if(Max2870_DBR == 1) pfd = pfd * 2.0;
  pfd = pfd / r;
  if (Max2870_RDIV2 == 1) pfd = pfd / 2.0;
  return pfd;
}

void Max2870SetFrequency(double direct)
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

  pfd = Max2870GetPfd();

  freqOK = false;
  if(direct ==0)
  {
   while(!freqOK)
    {
      Serial.print("\nEnter required Frequency in MHz -->");
      freq = inputFloat();
      if((freq > 23.500) && (freq <= 6000.000))
        {
          freqOK = true;
        }
      else
        {
          Serial.println("Frequency must be between 23.5 and 6000 MHz");
        }
    }
  }
  else
  {
    freq = direct;
  }
//Calculate the required output divider
  if((freq >= 3000.000) && (freq <= 6000.000))  Max2870_DIVA = 0;
  if((freq >= 1500.000) && (freq < 3000.000))  Max2870_DIVA = 1;
  if((freq >= 750.000) && (freq < 1500.000))  Max2870_DIVA = 2;
  if((freq >= 375.000) && (freq < 750.000))  Max2870_DIVA = 3; 
  if((freq >= 187.500) && (freq < 375.000))  Max2870_DIVA = 4;
  if((freq >= 93.750) && (freq < 187.500))  Max2870_DIVA = 5;
  if((freq >= 46.875) && (freq < 93.750))  Max2870_DIVA = 6;
  if((freq >= 23.500) && (freq < 46.875))  Max2870_DIVA = 7;

//adjust the frequency to allow for the output division
  freq = freq * (1 << Max2870_DIVA);

  n = freq / pfd;

  Max2870_N = (unsigned int) n;
  n = n - (unsigned int) n;

  //n now has the desired fractional part. Now we need to work put the best f/d to acheive this
   err = 1.0;
   bestnom = 0;
   bestden = 1;
   for(den = 2; den <= 4095 ; den++)
     {
      nom = int((den + 0.0000000001)  * n);
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
  Max2870_M = bestden;
  Max2870_FRAC = bestnom;

  Max2870EncodeRegs();
}


void Max2870CalcFreq(void)
{
  double pfd;
  double vco;
  double n;
  double m;
  double f;
  double nf;

  uint8_t diva;

  diva = 1 << Max2870_DIVA;

  n = (double) Max2870_N;
  m = (double) Max2870_M;
  f = (double) Max2870_FRAC;

  Serial.println();
  Serial.print("Reference Oscillator = ");
  Serial.print(refOsc,10);
  Serial.println(" MHz");


  pfd = Max2870GetPfd();


  Serial.print("PFD = ");
  Serial.print(pfd , 10);
  Serial.println(" MHz");

  if(Max2870_INT == 0)
  {
     if(Max2870_FB == 1)
       {
         nf = n + (f/m);
       }
      else
       {
          nf = (n + (f / m)) * diva;
       }
  }
  else
  {
         if(Max2870_FB == 1)
       {
         nf =  n ;
       }
      else
       {
          nf = n * diva;
       }

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
  Serial.println(") )");

  Serial.print("VCO = ");
  Serial.print(vco , 10);
  Serial.println(" MHz");

  Serial.print("O/P Divider = /");
  Serial.println(diva);

  Serial.print("Output Frequency = ");
  Serial.print(vco / diva , 10);
  Serial.println(" MHz");
}

double Max2870GetFrequency(void)
{
  double pfd;
  double vco;
  double n;
  double m;
  double f;
  double nf;

  uint8_t diva;

  diva = 1 << Max2870_DIVA;

  n = (double) Max2870_N;
  m = (double) Max2870_M;
  f = (double) Max2870_FRAC;

  pfd = Max2870GetPfd();

  if(Max2870_INT == 0)
  {
     if(Max2870_FB == 1)
       {
         nf = n + (f/m);
       }
      else
       {
          nf = (n + (f / m)) * diva;
       }
  }
  else
  {
         if(Max2870_FB == 1)
       {
         nf =  n ;
       }
      else
       {
          nf = n * diva;
       }

  }

  vco = pfd * nf;

  return vco / diva;
}


  void Max2870FskKey(bool key)
{
  static bool lastkey;
  
  if(key == lastkey) return;
  lastkey = key;

  if(key)
    {
      if(cwidKeyUpDen != Max2870_M)
      {
        Max2870Send(reg[1]);
      }

      if((cwidKeyUpN != Max2870_N) || (cwidKeyUpNum != Max2870_FRAC))
      {
         Max2870Send(reg[0]);
      }

    }
  else
    {
      if(cwidKeyUpDen != Max2870_M)
        {
          Max2870Send((reg[1] & 0xFFFF8007) | (cwidKeyUpDen << 3));
        }
      if((cwidKeyUpN != Max2870_N) || (cwidKeyUpNum != Max2870_FRAC))
      {
          Max2870Send((reg[0] & 0x80000007) | (cwidKeyUpN << 15) | (cwidKeyUpNum <<3));
      }
    }
}


void Max2870SaveFskShift(void)
{
  cwidKeyUpN = Max2870_N;
  cwidKeyUpDen = Max2870_M;
  cwidKeyUpNum = Max2870_FRAC;
}