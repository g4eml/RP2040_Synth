//sends a jt encoded message 

int jtTicks = 0;
int jtPointer = 0;

double jtInit(void)
{
  double nominal = chipGetFrequency();
  double nomf = 0;
  double thisf = 0;
  double worsterr = 0;
  
  switch(jtMode)
    {
      case 0:
      return 0;
      break;

      case 1:
      jtEncode.jt4_encode(jtid , jtBuffer);
      jtToneDelay = JT4_DELAY;
      jtNumberOfTones = 4;
      jtToneSpacing = JT4G_TONE_SPACING;
      jtSymbolCount = JT4_SYMBOL_COUNT;
      jtTime = 1;                           //JT message every even minute
      cwidInterval = 120;                   //override CW interval when Jt mode is active
      nextcwidTime = 60;                    //CW ID every Odd minute
      break;

      case 2:
      jtEncode.jt65_encode(jtid , jtBuffer);
      jtToneDelay = JT65_DELAY;
      jtNumberOfTones = 65;
      jtToneSpacing = JT65B_TONE_SPACING;
      jtSymbolCount = JT65_SYMBOL_COUNT;
      jtTime = 1;                           //JT message every even minute
      cwidInterval = 120;                   //override CW interval when Jt mode is active
      nextcwidTime = 60;                    //CW ID every Odd minute
      break;    

      case 3:
      jtEncode.jt65_encode(jtid , jtBuffer);
      jtToneDelay = JT65_DELAY;
      jtNumberOfTones = 65;
      jtToneSpacing = JT65C_TONE_SPACING;
      jtSymbolCount = JT65_SYMBOL_COUNT;
      jtTime = 1;                           //JT message every even minute
      cwidInterval = 120;                   //override CW interval when Jt mode is active
      nextcwidTime = 60;                    //CW ID every Odd minute
      break; 
    }

  for(int i = 0;i < jtNumberOfTones;i++)
   {
    nomf = nominal + i * (jtToneSpacing/1000000.0);
     chipSetFrequency(nomf);
     chipSaveJt(i);
     thisf = chipGetFrequency();
     if(abs(thisf - nomf) > worsterr )
     {
       worsterr = abs(thisf - nomf);   
     } 
   }

   chipSetFrequency(nominal);


  jtPointer = 0;
  jtTicks = 0;

  return worsterr;
}

void jtTick(void)
{
  if(jtActive)
  {
    jtTicks++;
    if(jtTicks == jtToneDelay)
      {
        jtTicks = 0;
        if(jtPointer == jtSymbolCount) 
          {
            jtActive = false;
            jtPointer =  0;
            chipJtShift(0);
          }
        else
        {
         chipJtShift(jtBuffer[jtPointer++]);
        }
      }
  }
}