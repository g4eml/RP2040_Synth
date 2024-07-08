// generates a CW ident 

unsigned int cwidTicks;
unsigned int cwidBitInterval;
uint32_t cwidCycleTimer;
uint8_t cwidBitCount;
int cwidByteIndex;
bool cwidKey;
uint32_t cwidPattern;
bool cwidSending = false;

void cwidInit(void)
{
  cwidBitCount = 0;
  cwidByteIndex = 0;
  cwidTicks = 0;
  cwidBitInterval = 1200 / cwidSpeed;
  cwidCycleTimer = 10000;
  cwid[0] = 255;             // add an initial 1 second of silence. 
  cwid[cwidLen + 1] = 255;    // and also at the end if the ident
  cwidEncode(0);

// calculate the required f/d value for the CWID FSK Offset
  double nominal = chipGetFrequency();
  chipSetFrequency(nominal + cwidShift);
  chipSaveFskShift();
  chipSetFrequency(nominal); 
}

void cwidTick(void)
{
  cwidCycleTimer--;
  if(cwidCycleTimer == 0)   //trigger the next ident 
    {
      cwidCycleTimer = cwidInterval * 1000;
      cwidSending = true;
      cwidByteIndex = 0;                     //get ready to send the ident 
      cwidPattern = 0;                        //set up 1 second of key up before sending the ident
    }

  if(cwidSending)
    {
      cwidTicks++;
      if(cwidTicks == cwidBitInterval)       //ready for the next bit
        {
          cwidTicks = 0;
          cwidBitCount--;
          cwidKey = cwidPattern & (1 << cwidBitCount);
          chipFskKey(cwidKey);
          if(cwidBitCount == 0)
            {
              cwidByteIndex++;
              if(cwidByteIndex == cwidLen + 2)
                {
                  cwidSending = false;
                }
                  cwidEncode(cwidByteIndex);
            }
       }

    } 
    else
    {
      chipFskKey(true);
    }
  
}

void cwidEncode(int index)
{
  uint8_t morseBits[] = {8,12,14,10,4,12,12,10,6,16,12,12,10,8,14,14,16,10,8,6,10,12,12,14,16,14,22,20,18,16,14,12,14,16,18,20,16,4};
  // A B C D E F G H I
  uint32_t morsePattern[] = {0b10111000 , 0b111010101000 , 0b11101011101000 , 0b1110101000 , 0b1000, 0b101011101000 , 0b111011101000 , 0b1010101000, 0b101000 ,
  // J K L M N O P Q R
                            0b1011101110111000 , 0b111010111000 , 0b101110101000 , 0b1110111000 , 0b11101000 , 0b11101110111000 , 0b10111011101000 , 0b1110111010111000 , 0b1011101000 ,
  // S T U V W X Y Z
                            0b10101000 , 0b111000 , 0b1010111000 , 0b101010111000 , 0b101110111000 , 0b11101010111000 , 0b1110101110111000 , 0b11101110101000 ,
  // 0 1 2 3 4 5
                            0b1110111011101110111000 , 0b10111011101110111000 , 0b101011101110111000 , 0b1010101110111000 , 0b10101010111000 , 0b101010101000 ,
  // 6 7 8 9 / space
                            0b11101010101000 , 0b1110111010101000 , 0b111011101110101000 , 0b11101110111011101000 , 0b1110101011101000 , 0b0000 };
  
  uint8_t charIndex;

  charIndex = cwid[index];
  
  switch(charIndex)
    {
      case 255:                 //special case for char 255 return a 1 second silence. Used at the start and end of the ident.
      cwidPattern = 0;
      cwidBitCount = 1000/cwidBitInterval;
      return;

      case 'A' ... 'Z':
      charIndex = charIndex - 'A';
      break; 

      case '0' ... '9':
      charIndex = charIndex - '0' + 26;
      break;

      case '/':
      charIndex = 36;
      break;

      default:
      charIndex = 37;
      break;
    }


  cwidBitCount = morseBits[charIndex];
  cwidPattern = morsePattern[charIndex];
}