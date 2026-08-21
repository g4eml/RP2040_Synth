//This module redirects the generic chipXxx() functions to the currently selected
//chip driver object (activeChip). It replaces what used to be ~16 separate
//switch(chip) statements, one per function, each of which had to be extended
//by hand every time a new chip type was added.
//
//To add a new chip type: see the instructions at the top of ChipList.h. That
//is the only list that needs to change - chipTable[] below and enum chipType
//in RP2040Synth.ino are both generated from it automatically.

#include "ChipList.h"

SynthChip* chipTable[] =
{
  nullptr,          //NONE
  #define CHIP_ENTRY(name, instance) &instance,
  CHIP_LIST
  #undef CHIP_ENTRY
};

void chipInit(void)
{
  activeChip = chipTable[chip];
  activeChip->init();
}

//Returns the display name for chip type "index" (an enum chipType value).
//Used instead of a separately-maintained name array so there is only one
//place (each chip's own constructor) where its name is defined.
const char* chipTypeName(uint8_t index)
{
  if (chipTable[index] == nullptr) return "None";
  return chipTable[index]->name;
}

void chipUpdate(void)
{
  activeChip->update();
}

void chipSetParameters(void)
{
  activeChip->setParameters();
}

void chipSetFrequency(double direct)
{
  activeChip->setFrequency(direct);
}

double chipGetFrequency(void)
{
  return activeChip->getFrequency();
}

void chipCalcFreq(void)
{
  activeChip->calcFreq();
}

void chipDecodeRegs(void)
{
  activeChip->decodeRegs();
}

void chipSetDefault(void)
{
  activeChip->setDefault();
}

void chipFskKey(bool key)
{
  activeChip->fskKey(key);
}

void chipExtKey(bool key)
{
  activeChip->extKey(key);
}

void chipJtShift(uint8_t val)
{
  activeChip->jtShift(val);
}

void chipSaveFskShift(void)
{
  activeChip->saveFskShift();
}

void chipSaveKeyShift(void)
{
  activeChip->saveKeyShift();
}

void chipSaveJt(uint8_t index)
{
  activeChip->saveJt(index);
}

double chipGetPfd(void)
{
  return activeChip->getPfd();
}

double chipCalcPfd(double pfd)
{
  return activeChip->calcPfd(pfd);
}
