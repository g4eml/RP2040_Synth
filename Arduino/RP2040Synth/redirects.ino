//This module redirects generic functions to the relevant chip specific version

void chipUpdate(void)
{
  switch(eeprom.chip)
  {
    case MAX2870:
    Max2870Update();
    break;

    case ADF4351:
    ADF4351Update();
    break;

    case LMX2595:
    LMX2595Update();
    break;
  }
}

void chipSetParameters(void)
{
  switch(eeprom.chip)
  {
    case MAX2870:
    Max2870SetParameters();
    break;

    case ADF4351:
    ADF4351SetParameters();
    break;

    case LMX2595:
    LMX2595SetParameters();
    break;
  }
}

void chipInit(void)
{
  switch(eeprom.chip)
  {
    case MAX2870:
    Max2870Init();
    break;

    case ADF4351:
    ADF4351Init();
    break;

    case LMX2595:
    LMX2595Init();
    break;
  }
}

void chipSetFrequency(double f)
{
  switch(eeprom.chip)
  {
    case MAX2870:
    Max2870SetFrequency(f);
    break;

    case ADF4351:
    ADF4351SetFrequency(f);
    break;

    case LMX2595:
    LMX2595SetFrequency(f);
    break;
  }
}

double chipGetFrequency(void)
{
  double freq;
  switch(eeprom.chip)
  {
    case MAX2870:
    freq = Max2870GetFrequency();
    break;

    case ADF4351:
    freq = ADF4351GetFrequency();
    break;

    case LMX2595:
    freq = LMX2595GetFrequency();
    break;
  }

  return freq;
}

void chipCalcFreq(void)
{
        switch(eeprom.chip)
        {
          case MAX2870:
          Max2870CalcFreq();
          break;

          case ADF4351:
          ADF4351CalcFreq();
          break;

          case LMX2595:
          LMX2595CalcFreq();
          break;
        }
}
  void chipDecodeRegs(void)
  {
      switch(eeprom.chip)
        {
          case MAX2870:
          Max2870DecodeRegs();
          break;

          case ADF4351:
          ADF4351DecodeRegs();
          break;

          case LMX2595:
          LMX2595DecodeRegs();
          break;
        }
  }

    void chipSetDefault(void)
  {
      switch(eeprom.chip)
        {
          case MAX2870:
          Max2870SetDefault();
          break;

          case ADF4351:
          ADF4351SetDefault();
          break;

          case LMX2595:
          LMX2595SetDefault();
          break;
        }
  }

    void chipFskKey(bool key)
  {

      switch(eeprom.chip)
        {
          case MAX2870:
          Max2870FskKey(key);
          break;

          case ADF4351:
          ADF4351FskKey(key);
          break;

          case LMX2595:
          LMX2595FskKey(key);
          break;
        }
  }

    void chipJtShift(uint8_t val)
  {

      switch(eeprom.chip)
        {
          case MAX2870:
          Max2870jtShift(val);
          break;

          case ADF4351:
          ADF4351jtShift(val);
          break;

          case LMX2595:
          LMX2595jtShift(val);
          break;
        }
  }


    void chipSaveFskShift(void)
  {

      switch(eeprom.chip)
        {
          case MAX2870:
          Max2870SaveFskShift();
          break;

          case ADF4351:
          ADF4351SaveFskShift();
          break;

          case LMX2595:
          LMX2595SaveFskShift();
          break;
        }
  }

    void chipSaveJt(uint8_t index)
  {

      switch(eeprom.chip)
        {
          case MAX2870:
          Max2870SaveJt(index);
          break;

          case ADF4351:
          ADF4351SaveJt(index);
          break;

          case LMX2595:
          LMX2595SaveJt(index);
          break;
        }
  }

    void chipSetPfd(void)
  {

      switch(eeprom.chip)
        {
          case MAX2870:
          Max2870SetPfd();
          break;

          case ADF4351:
          ADF4351SetPfd();
          break;

          case LMX2595:
          LMX2595SetPfd();
          break;
        }
  }

    double chipGetPfd(void)
  {
     double pfd;
      switch(eeprom.chip)
        {
          case MAX2870:
          pfd = Max2870GetPfd();
          break;

          case ADF4351:
          pfd = ADF4351GetPfd();
          break;

          case LMX2595:
          pfd = LMX2595GetPfd();
          break;
        }

    return pfd;
  }