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
  if (oscFreq > 0 )
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
                  reg[regno] = (uint32_t) strtol(buf,0,16);
               }
             Serial.print("\nR");
             Serial.print(regno);
             Serial.print(" = 0x");
             Serial.println(reg[regno],HEX);
           }
        }
      if(param[0] == '*')
        {
          Serial.println();
          for(int i = numberOfRegs -1; i>=0; i--)
          {
             Serial.print("R");
             Serial.print(i);
             Serial.print("\t0x");
             Serial.println(reg[i],HEX);
          }
        }
  }
}

void setCwIdent(void)
{
  char resp;
  String cws;
  Serial.println();
  resp = getSelection("Enable CW Ident? Y or N --->");
  if((resp == 'N') || (resp == 'n'))
   {
     cwidEn = 0;
     Serial.println();
     return;
   }

  cwidEn = 0x73;
  Serial.print("Enter CW Ident (max 255 characters) --->");
  cws = inputString(true);
  cws.toCharArray(&cwid[1], 255 );
  cwidLen = cws.length();

  Serial.print("Enter CW speed (5 - 30 Words per Minute) ---> ");
  cwidSpeed = inputNumber();
  if(cwidSpeed < 5) cwidSpeed = 5;
  if(cwidSpeed > 30) cwidSpeed = 30;

  Serial.println("Enter ident interval (Seconds)");
  Serial.print("Note:- This value will not be used if JT Modes are also active. ---> ");
  cwidInterval = inputNumber();
  if(cwidInterval < 10) cwidInterval = 5;
  if(cwidInterval > 120) cwidInterval = 120;

  Serial.print("Enter FSK Shift in Hz ---> ");
  cwidShift = inputNumber();
  cwidShift = cwidShift / 1000000.0;      //convert to MHz
  double nominal = chipGetFrequency();
  chipSetFrequency(nominal + cwidShift);
  chipSaveFskShift();
  Serial.println("Actual frequencies achievable with the current PFD will be :-");
  Serial.print("Key Up Frequency = ");
  double up = chipGetFrequency();
  Serial.print(up,10);
  Serial.println(" MHz");
  chipSetFrequency(nominal); 
  Serial.print("Key Down Frequency = ");
  double down = chipGetFrequency();
  Serial.print(down,10);
  Serial.println(" MHz");
  Serial.print("FSK Shift = ");
  Serial.print((down - up) * 1000000 , 3);
  Serial.println(" Hz");
  Serial.println("\nDon't forget to save the settings to EEPROM if you are happy with them.");
  Serial.println("CW ID will only run when not in the menu. Type X to exit menu.");

}

void setJTMode(void)
{
  String jtmodes[] = {"0 = None" , "1 = JT4G" , "2 = JT65B" , "$$$"};
  char resp;
  String jts;
  showMenu(jtmodes);
  jtMode = getSelection("Select JT Mode --->") - '0';

  Serial.print("Enter JT Message (Max 13 characters) --->");
  jts = inputString(true) + "             ";                 //make sure there are at least 13 chars available
  jts.toCharArray(&jtid[0], 13 );


  double worsterr = jtInit();

   if((worsterr * 1000000.0)> 0.009)
   {
    Serial.print("The current chip settings can produce the required tones with a maximum error of ");
    Serial.print(worsterr * 1000000.0);
    Serial.println(" Hz");
   }

  Serial.println("\nDon't forget to save the settings to EEPROM if you are happy with them.");
  Serial.println("JT Encoding will only run when not in the menu. Type X to exit menu.");

}

void mainMenu(void)
{
  char resp;
  double temp;
  String menuList[] = {"T = Select Chip Type" , "D = Set Default Register Values for chip" , "O = Set Reference Oscillator Frequency" , "P = Enter PFD Frequency" , "F = Enter Output Frequency" , "C = Calculate and display frequency from current settings" , "V = View / Enter Variables for Registers", "R = View / Enter Registers Directly in Hex" , "I = Configure CW Ident" ,"J = Configure JT Mode" , "S = Save Registers to EEPROM" , "X = Exit Menu" , "$$$"};
  String chipList[] = {"1 = MAX2870" , "2 = ADF4351" , "3 = LMX2595" , "$$$"};

   Serial.println("");
   Serial.println("G4EML Synthesiser Controller");
   flushInput();
   showMenu(menuList);
   do
    {
      resp = getSelection("Enter Command -->");

      switch(resp)
      {
        case 'S':
        case 's':
        saveSettings();
        Serial.println("\nRegisters saved to EEPROM");
        break;

        case 'F':
        case 'f':
        Serial.print("Current Frequency is ");
        Serial.print(chipGetFrequency(),10);
        Serial.println(" MHz");
        chipSetFrequency(0);
        chipUpdate();
        Serial.println("\nFrequency Updated");
        chipCalcFreq();
        break;

        case 'V':
        case 'v':
        chipSetParameters();
        chipCalcFreq();
        break;

        case 'T':
        case 't':
        showMenu(chipList);
        resp = getSelection("Enter Chip Type -->");
        if((resp > '0') && (resp < '4'))
        {
        chip = resp - '0';
        }
        Serial.print("Chip type is now ");
        Serial.println(chipName[chip]);
        break;

        case 'R':
        case 'r':
        enterRegs();
        chipDecodeRegs();
        chipCalcFreq();
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
        chipSetDefault();
        chipCalcFreq();
        break;

        case 'I':
        case 'i':
        setCwIdent();
        break;

        case 'J':
        case 'j':
        setJTMode();
        break;

        case 'P':
        case 'p':
        Serial.print("Current PFD is ");
        Serial.print(chipGetPfd() , 10);
        Serial.println(" MHz");
        temp = chipGetFrequency();
        chipSetPfd();
        Serial.println("Recalculating frequency with new PFD.");
        chipSetFrequency(temp);
        chipUpdate();
        break;

        default:
        showMenu(menuList);
      }

    }
    while((resp != 'X')&&(resp != 'x'));
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



