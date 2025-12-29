//sends a jt encoded message 

int jtTicks = 0;
int jtPointer = 0;

double jtInit(void)
{
  double nominal = chipGetFrequency();
  double nomf = 0;
  double thisf = 0;
  double worsterr = 0;
  
  switch(chanData[channel].jtMode)
    {
      case 0:
      return 0;
      break;

      case 1:
      jtEncode.jt4_encode(chanData[channel].jtid , jtBuffer);
      jtToneDelay = JT4_DELAY;
      jtNumberOfTones = 4;
      jtToneSpacing = JT4G_TONE_SPACING;
      jtSymbolCount = JT4_SYMBOL_COUNT;
      nextjtTime = 1;                           //JT message every even minute
      chanData[channel].cwidInterval = 120;            //override CW interval when Jt mode is active
      chanData[channel].jtInterval = 120;            //digi mode every 2 minute 
      nextcwidTime = 60;                    //CW ID every Odd minute
      break;

      case 2 ... 4: //Q65-15
      Q65Encode(chanData[channel].jtid , jtBuffer);
      jtToneDelay = Q65_DELAY;
      jtNumberOfTones = 65;
      jtToneSpacing = Q65_TONE_SPACING *2;

      if(chanData[channel].jtMode == 3)
       {
        jtToneSpacing = Q65_TONE_SPACING *4;
       }
      if(chanData[channel].jtMode == 4)
       {
        jtToneSpacing = Q65_TONE_SPACING *8;
       }

      jtSymbolCount = Q65_SYMBOL_COUNT;
      nextjtTime = 1;                           //Digi message every minute
      chanData[channel].cwidInterval = 60;            //override CW interval when Digi mode is active
      chanData[channel].jtInterval = 60;            //digi mode every 60 secs  
      nextcwidTime = 30;                        //CW ID at 30 seconds
      break;

      case 5 ... 8: //Q65-30
      Q65Encode(chanData[channel].jtid , jtBuffer);
      jtToneDelay = Q65_DELAY *2;
      jtNumberOfTones = 65;
      jtToneSpacing = Q65_TONE_SPACING;

      if(chanData[channel].jtMode == 6)
       {
        jtToneSpacing = Q65_TONE_SPACING *2;
       }
      if(chanData[channel].jtMode == 7)
       {
        jtToneSpacing = Q65_TONE_SPACING *4;
       }
      if(chanData[channel].jtMode == 8)
       {
        jtToneSpacing = Q65_TONE_SPACING *8;
       }

      jtSymbolCount = Q65_SYMBOL_COUNT;
      nextjtTime = 1;                           //Digi message every minute
      chanData[channel].cwidInterval = 60;            //override CW interval when Digi mode is active
      chanData[channel].jtInterval = 60;            //digi mode every 60 secs  
      nextcwidTime = 30;                        //CW ID at 30 seconds
      break;
 
    }

     chipSetFrequency(nominal);
     chipSaveJt(0);                           //save the nominal carrier frequency to JT Index 0
  for(int i = 0;i < jtNumberOfTones;i++)
   {
    nomf = nominal + (chanData[channel].jtTone1 / (double) chanData[channel].extMult) + i * ((jtToneSpacing/1000000.0) / (double) chanData[channel].extMult);
     chipSetFrequency(nomf);
     chipSaveJt(i+1);
     thisf = chipGetFrequency();
     if(abs(thisf - nomf) > worsterr )
     {
       worsterr = abs(thisf - nomf);   
     } 
   }

   chipSetFrequency(nominal);

  jtPointer = 0;
  jtTicks = 0;

  return worsterr * (double) chanData[channel].extMult;
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
            chipJtShift(0);                                 //Reset to nominal carrier frequency
          }
        else
        {
         chipJtShift(jtBuffer[jtPointer++] +1);
        }
      }
  }
}