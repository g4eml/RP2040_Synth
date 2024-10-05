void keyInit(void)
{
  lastKeyState = 1;
// calculate the required f/d value for the External Key FSK Offset
  double nominal = chipGetFrequency();
  chipSetFrequency(nominal + (double) chanData[channel].keyShift / (double) chanData[channel].extMult);
  chipSaveKeyShift();
  chipSetFrequency(nominal); 
}

void keyTick(void)
{
  if((!jtActive)&&(!cwidActive))                //dont allow external keying during idents. 
  {
    bool keyState = digitalRead(EXTKEYPin);
    if(lastKeyState != keyState)
    {
      chipExtKey(keyState);
      lastKeyState = keyState;
    }
  }

}
