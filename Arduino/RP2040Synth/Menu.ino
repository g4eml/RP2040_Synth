// Menu routines for programming

void flushInput(void)
{
  char dummy;
   while(Serial.available())
   {
    dummy = Serial.read();
   }
}

int32_t inputNumber(void)
{
  int n = -1;
  bool done = false;
  char ch;
  bool minus = false;

  flushInput();
  while(!done)
    {
      while(Serial.available() == 0);
      ch = Serial.read();
      Serial.print(ch);
      switch(ch)
        {
          case 13:
          Serial.println();
          done = true;
          break;
          
          case '0' ... '9':
          if(n == -1) n = 0;
          n = (n * 10) + (ch - '0');
          break;

          case '-':
          minus = true;
          break;

        }

    }

  if(minus)
  {
    return 0-n;
  }
  else
  {
    return n;
  }
}

double inputFloat(void)
{
  bool done = false;
  String s;
  char ch;
  flushInput();
  s = "";
    while(!done)
    {
      while(Serial.available() == 0);
      ch = Serial.read();
      if(ch == 13)
      {
        Serial.println();
        done = true;
      }
      else
      {
      Serial.print(ch);
      s += ch;

      if(((ch == 8) || (ch == 127)) && (s.length() >1))
        {       
          s.remove(s.length() -2);
        }
      }
    }

  return s.toDouble();
}

String inputString(bool uppercase)
{
  bool done = false;
  char ch;
  String s;
  flushInput();
  s = "";
    while(!done)
    {
      while(Serial.available() == 0);
      ch = Serial.read();
      if(uppercase)
        {
          if((ch >=97) && (ch <= 122))
          {
            ch = ch - 32;
          }
        }
      if(ch == 13)
      {
        Serial.println();
        done = true;
      }
      else
      {
      Serial.print(ch);
      s += ch;

      if(((ch == 8) || (ch == 127)) && (s.length() >1))
        {       
          s.remove(s.length() -2);
        }
      }
    }
  return s;
}

void showMenu(String *list)
{

 int i = 0;

 Serial.print("\n");
 Serial.print("Chip type is ");
 Serial.println(chipName[chip]);
 Serial.print("Ref Osc =  ");
 Serial.print(refOsc , 10);
 Serial.println(" MHz");
 Serial.print("Channel Number ");
 if(selChan == 255)
  {
    Serial.print(" (Externally Selected) = ");
    channel = readChannelInputs();
  }
else
  {
    Serial.print(" (Fixed) = "); 
  }
 Serial.println(channel);
 Serial.println();
  while(list[i] != "$$$")
  {
    Serial.println(list[i++]);
  }
}

char getSelection(String p)
{
 char resp;

  Serial.println();
  Serial.print(p);

  while(Serial.available() == 0);
  resp = Serial.read();
  Serial.println(resp);

  return resp;
}

void enterOsc(void)
{
  double oscFreq = 0;
  Serial.print("\nCurrent Reference Oscillator is ");
  Serial.print(refOsc , 10);
  Serial.print(" MHz\nEnter New Reference Oscillator Frequency in MHz --> ");
  oscFreq = inputFloat();
  if ((oscFreq >= minOsc ) && (oscFreq <= maxOsc))
    {
      refOsc = oscFreq;
    }
}

void enterRegs(void)
{
  int regno = 0;
  int64_t regval = 0;
  String resp;
  String param;
  String value;
  bool done = false;
  int equals;

  while(!done)
  {
    Serial.print("\nEnter Register or ? for help--> ");
    resp = inputString(true);
    if (resp.length() == 0)
      {
         done = true;
         continue;
      } 
    if(resp.indexOf("?") != -1)
      {
        Serial.println();
        Serial.println("Enter 'Rn' to view current Register value.");
        Serial.println("Enter 'Rn = HHHHHHHH' to set Register to Hex value.");
        Serial.println("Enter * to dump all registers.");
        Serial.println("Enter blank line to exit");
        Serial.println("Valid Register Numbers (See Chip Data Sheet) are:-");
        Serial.print("R0 to R");
        Serial.println(numberOfRegs -1);
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

      if(param[0] == 'R')
        {
           param = param.substring(1);   //remove the R character
           param.trim();
           regno = param.toInt();
           if(regno < numberOfRegs)
             {
             if(value.length() >0)
               {
                  char buf[16];
                  value.toCharArray(buf,16);
                  chanData[channel].reg[regno] = (uint32_t) strtoll(buf,0,16);
               }
             Serial.print("\nR");
             Serial.print(regno);
             Serial.print(" = ");
             printHex(chanData[channel].reg[regno],8);
             Serial.println("");

           }
        }
      if(param[0] == '*')
        {
          Serial.println();
          for(int i = 0 ;i < numberOfRegs; i++)
          {
             Serial.print("R");
             Serial.print(i);
             Serial.print("\t");
             printHex(chanData[channel].reg[i],8);
             Serial.println("");
          }
        }
  }
}

void printHex(uint32_t num, int precision)
{
  char tmp[16];
  char format[128];
  sprintf(format, "0x%%0%dlX", precision);
  sprintf(tmp, format, num);
  Serial.print(tmp);
}

void setCwIdent(void)
{
  char resp;
  String cws;
  Serial.println();
  resp = getSelection("Enable CW Ident? Y or N --->");
  if((resp == 'N') || (resp == 'n'))
   {
     chanData[channel].fskMode &= NOTCWIDBIT;
     Serial.println();
     return;
   }

  chanData[channel].fskMode |= CWIDBIT;
  Serial.print("Enter CW Ident (max 30 characters) --->");
  cws = inputString(true);
  cws.toCharArray(&chanData[channel].cwid[1], 30 );
  chanData[channel].cwidLen = cws.length();

  Serial.print("Enter CW speed (5 - 30 Words per Minute) ---> ");
  chanData[channel].cwidSpeed = inputNumber();
  if(chanData[channel].cwidSpeed < 5) chanData[channel].cwidSpeed = 5;
  if(chanData[channel].cwidSpeed > 30) chanData[channel].cwidSpeed = 30;

  if(chanData[channel].jtMode==0)
    {
    Serial.print("Enter ident interval (Seconds) -->");
    chanData[channel].cwidInterval = inputNumber();
    if(chanData[channel].cwidInterval < 10) chanData[channel].cwidInterval = 10;
    if(chanData[channel].cwidInterval > 120) chanData[channel].cwidInterval = 120;
    }

  if(chanData[channel].extMult > 1)
     {
        Serial.print("Enter Final Multiplied Frequency FSK Shift in Hz ---> ");
     }
  else
     {
       Serial.print("Enter FSK Shift in Hz ---> ");
     }

  chanData[channel].cwidShift = inputNumber() / (double) chanData[channel].extMult ;
  chanData[channel].cwidShift = chanData[channel].cwidShift / 1000000.0;      //convert to MHz
  double nominal = chipGetFrequency();
  chipSetFrequency(nominal + (double) chanData[channel].cwidShift);
  chipSaveFskShift();
  Serial.println("\nActual Final Multiplied frequencies achievable with the current PFD will be :-");
  Serial.print("Key Up Frequency = ");
  double up = chipGetFrequency() * (double) chanData[channel].extMult;
  Serial.print(up,10);
  Serial.println(" MHz");
  chipSetFrequency(nominal); 
  Serial.print("Key Down Frequency = ");
  double down = chipGetFrequency() * (double) chanData[channel].extMult;
  Serial.print(down,10);
  Serial.println(" MHz");
  Serial.print("FSK Shift = ");
  Serial.print((down - up) * 1000000 , 3);
  Serial.println(" Hz");
  Serial.println("\nDon't forget to save the settings to EEPROM if you are happy with them.");
  Serial.println("CW ID will only run when not in the menu. Type X to exit menu.");

}

void setjtMode(void)
{
  String jtModes[] = {"0 = None" , "1 = JT4G" , "2 = JT65B" , "3 = JT65C", "$$$"};
  String jtModesReduced[] = {"0 = None" , "1 = JT4G" , "$$$"};
  char resp;
  char maxresp;
  String jts;
  if(jt4Only)
    {
      showMenu(jtModesReduced);
      maxresp = '1';
    }
  else
    {
      showMenu(jtModes);
      maxresp = '3';
    }
  resp = getSelection("Select JT Mode --->");
  if((resp >='0') && (resp <= maxresp))
    {
      chanData[channel].jtMode =  resp - '0';
    }
  else
    {
      chanData[channel].jtMode = 0;
    }


  if(chanData[channel].jtMode != 0)
    {
      Serial.print("Enter Tone 1 Offset from Carrier Frequency (Hz) --->");
      chanData[channel].jtTone1 = inputNumber() / (double) chanData[channel].extMult ;
      chanData[channel].jtTone1 = chanData[channel].jtTone1 / 1000000.0;      //convert to MHz
      char temp[14];
      Serial.print("Enter JT Message (Max 13 characters) --->");
      jts = inputString(true) + "             ";                 //make sure there are at least 13 chars available
      jts.toCharArray(temp,14);                                  //copy the chars to a temporary char array (include trailing zero terminator)
      for(int c = 0;c<13;c++)                                    //now copy the  13 chars excluding the trailing zero
      {
        chanData[channel].jtid[c] = temp[c];
      }
      
      double worsterr = jtInit();

      if((worsterr * 1000000.0)> 0.009)
      {
        Serial.println("\nThe current Synthesiser settings can produce the required tones");
        Serial.print("with a maximum error of ");
        Serial.print(worsterr * 1000000.0);
        Serial.println(" Hz");
      }
      if((worsterr * 1000000.0) > (jtToneSpacing / 100))
      {
        Serial.println("\nWARNING! This error may be too large for accurate decoding.");
        Serial.println("Try changing the Tone 1 Offset or the PFD for a smaller error.");
      }
    }
  Serial.println("\nDon't forget to save the settings to EEPROM if you are happy with them.");
  Serial.println("JT Encoding will only run when not in the menu. Type X to exit menu.");

}

void setKeyMode(void)
{
  char resp;
  Serial.println();
  resp = getSelection("Enable External CW Key? Y or N --->");
  if((resp == 'N') || (resp == 'n'))
   {
     chanData[channel].fskMode &= NOTEXTKEYBIT;
     Serial.println();
     return;
   }

  chanData[channel].fskMode |= EXTKEYBIT;

  if(chanData[channel].extMult > 1)
     {
        Serial.print("Enter Final Multiplied Frequency FSK Shift in Hz ---> ");
     }
  else
     {
       Serial.print("Enter FSK Shift in Hz ---> ");
     }

  chanData[channel].keyShift = inputNumber() / (double) chanData[channel].extMult ;
  chanData[channel].keyShift = chanData[channel].keyShift / 1000000.0;      //convert to MHz
  double nominal = chipGetFrequency();
  chipSetFrequency(nominal + (double) chanData[channel].keyShift);
  chipSaveKeyShift();
  Serial.println("\nActual Final Multiplied Frequencies achievable with the current PFD will be :-");
  Serial.print("Key Down Frequency = ");
  double down = chipGetFrequency() * (double) chanData[channel].extMult;
  Serial.print(down,10);
  Serial.println(" MHz");
  chipSetFrequency(nominal); 
  Serial.print("Key Up Frequency = ");
  double up = chipGetFrequency() * (double) chanData[channel].extMult;
  Serial.print(up,10);
  Serial.println(" MHz");
  Serial.print("FSK Shift = ");
  Serial.print((down - up) * 1000000 , 3);
  Serial.println(" Hz");
  Serial.println("\nDon't forget to save the settings to EEPROM if you are happy with them.");
  Serial.println("CW ID will only run when not in the menu. Type X to exit menu.");
}



void viewNMEA(void)
{
  Serial.println("Looking for NMEA Data. Press any key to exit.");
  flushInput();
  while(Serial.available() == 0)
    {
      if(Serial1.available() > 0)           //data received from GPS module
      {
        while(Serial1.available() >0)
          {
            Serial.write(Serial1.read());
          }
      }
    }
}

void mainMenu(void)
{
  char resp;
  double temp;
  String menuList[] = {"T = Select Chip Type" , "O = Set Reference Oscillator Frequency" , "N = Set Channel Number" ,"     ", "D = Set Default Register Values for chip"  , "P = Enter PFD Frequency" ,"M = Set External Multiplier", "F = Enter Output Frequency" , "C = Calculate and display frequency from current settings" , "V = View / Enter Variables for Registers", "R = View / Enter Registers Directly in Hex" , "I = Configure CW Ident" ,"J = Configure JT Mode" , "K = Configure External Key", "G = View GPS NMEA data", "S = Save to EEPROM" , "X = Exit Menu" , "$$$"};
  String chipList[] = {"1 = MAX2870" , "2 = ADF4351" , "3 = LMX2595" , "$$$"};

   Serial.println("");
   Serial.print("G4EML Synthesiser Controller Version ");
   Serial.println(VERSION);
   flushInput();
   showMenu(menuList);
   do
    {
      resp = getSelection("Enter Command (? for menu) -->");

      switch(resp)
      {
        case 'N':
        case 'n':
        Serial.print("Enter Channel Number 0-");
        Serial.print(NUMBEROFCHANNELS -1);
        Serial.print(" or 255 for Externally Selected -->");
        selChan = inputNumber();
        if(selChan < 0) selChan = 0;
        if((selChan > NUMBEROFCHANNELS -1)&(selChan !=255)) selChan=NUMBEROFCHANNELS -1;
        if(selChan < NUMBEROFCHANNELS)
         {
           channel=selChan;
         }
        else
         {
          channel=readChannelInputs();
         } 
        initChannel();
        chipUpdate();
        break;

        case 'S':
        case 's':
        saveSettings();
        Serial.println("\nSettings saved to EEPROM");
        break;

        case 'F':
        case 'f':
        Serial.print("Current Frequency is ");
        Serial.print(chipGetFrequency(),10);        
        Serial.println(" MHz");
        if(chanData[channel].extMult >1)
          {
            Serial.print("Current Final Multiplied Frequency is ");
            Serial.print(chipGetFrequency() * (double) chanData[channel].extMult,10);        
            Serial.println(" MHz");
          }

        chipSetFrequency(0);
        chipUpdate();
        Serial.println("\nFrequency Updated");
        break;

        case 'M':
        case 'm':
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
        break;

        case 'V':
        case 'v':
        chipSetParameters();
        break;

        case 'T':
        case 't':
        Serial.println("Warning:- Changing Chip Type will reset all channels to default.");
        resp=getSelection("Do you want to continue? -->");
        if ((resp != 'Y') & (resp != 'y')) break;
        showMenu(chipList);
        resp = getSelection("Enter Chip Type -->");
        if((resp > '0') && (resp < '4'))
        {
        chip = resp - '0';
        }
        Serial.print("Chip type is now ");
        Serial.println(chipName[chip]);
        channel = 0;
        selChan = 0;
        chanData[channel].fskMode = 0;
        chanData[channel].jtMode = 0;
        chipInit();
        enterOsc();
        chipSetDefault();
        for(int c=1 ; c < NUMBEROFCHANNELS;c++)
          {
            chanData[c] = chanData[0];
          }
        break;

        case 'R':
        case 'r':
        enterRegs();
        chipDecodeRegs();
        break;

        case 'O':
        case 'o':
        temp = chipGetFrequency();
        enterOsc();
        Serial.println("Recalculating frequency with new Ref Osc.");
        chipSetFrequency(temp);
        chipUpdate();
        break;

        case 'C':
        case 'c':
        chipCalcFreq();
 
        break;

        case 'D':
        case 'd':
        Serial.println("Default register values loaded.");
        chipInit();
        chipSetDefault();
        break;

        case 'I':
        case 'i':
        setCwIdent();
        break;

        case 'J':
        case 'j':
        setjtMode();
        break;

        case 'K':
        case 'k':
        setKeyMode();
        break;

        case 'G':
        case 'g':
        viewNMEA();
        break;


        case 'P':
        case 'p':
        enterPfd();
        break;

        default:
        showMenu(menuList);
      }

    }
    while((resp != 'X')&&(resp != 'x'));
}

void enterPfd(void)
 {
  double pfd;
  double rpfd;
  double temp;
  bool freqOK;
  double oldpfd;
  
  freqOK = false;

  oldpfd = chipGetPfd();

  Serial.print("Current PFD is ");
  Serial.print(oldpfd , 10);
  Serial.println(" MHz");
  temp = chipGetFrequency();
 
  while(!freqOK)
    {
       Serial.printf("\nEnter required PFD in MHz -->");
       pfd = inputFloat();
       if(pfd == 0) return;
      if((pfd <= maxPfd) && (pfd >= minPfd))
        {
          freqOK = true;
        }
      else
        {
          Serial.print("\nPFD must be between ");
          Serial.print(minPfd);
          Serial.print(" MHz");
          Serial.print(" and ");
          Serial.print(maxPfd);
          Serial.println(" MHz");
        }    
    }
   
    rpfd = chipCalcPfd(pfd);

    if(rpfd != oldpfd)
    {
     Serial.println("Recalculating frequency with new PFD");
    chipSetFrequency(temp);
    chipUpdate();     
    }
 }

bool paramBool(String param , String name, bool* var , String value)
{
  bool ret = false;

         if(param == name)
        {
          if(value.length() >0)
          {
            if((value.toInt() >= 0) && (value.toInt() <= 1))
            {
              *var = value.toInt();
            }
          }
          Serial.print("\n");
          Serial.print(param);
          Serial.print(" = ");
          Serial.println(*var);
          ret = true;
        }
  return ret;
}

bool paramByte(String param , String name, byte* var , String value , byte min , byte max)
{
  bool ret = false;

         if(param == name)
        {
          if(value.length() >0)
          {
            if((value.toInt() >= min) && (value.toInt() <= max))
            {
              *var = value.toInt();
            }
          }
          Serial.print("\n");
          Serial.print(param);
          Serial.print(" = ");
          Serial.println(*var);
          ret = true;
        }
  return ret;
}

bool paramUint(String param , String name, unsigned int* var , String value , uint16_t min , uint16_t max)
{
  bool ret = false;

         if(param == name)
        {
          if(value.length() >0)
          {
            if((value.toInt() >= min) && (value.toInt() <= max))
            {
              *var = value.toInt();
            }
          }
          Serial.print("\n");
          Serial.print(param);
          Serial.print(" = ");
          Serial.println(*var);
          ret = true;
        }
  return ret;
}

bool paramUint32(String param , String name, uint32_t * var , String value , uint32_t min , uint32_t max)
{
  bool ret = false;

         if(param == name)
        {
          if(value.length() >0)
          {
            if((value.toInt() >= min) && (value.toInt() <= max))
            {
              *var = value.toInt();
            }
          }
          Serial.print("\n");
          Serial.print(param);
          Serial.print(" = ");
          Serial.println(*var);
          ret = true;
        }
  return ret;
}



