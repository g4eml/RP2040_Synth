// SynthChip.h
// Common interface implemented by every supported RF synthesiser chip driver
// (MAX2870, ADF4351, LMX2595, CMT2119A, ADF5355, ...).
//
// To add support for a new chip:
//   1. Create a new class that inherits from SynthChip. In its constructor,
//      set "name" to the chip's display name (see MAX2870.ino for a simple
//      example). Implement the pure virtual methods below; the optional ones
//      only need overriding if they apply to the new chip.
//   2. Instantiate one global object of the new class (e.g. "MyChip myChip;"
//      at the bottom of the file, outside the class).
//   3. Add one line for it to CHIP_LIST in ChipList.h.
//
// That's it - see ChipList.h for details. enum chipType, chipTable[], the
// chip count, and the chip-selection menu are all generated automatically
// from that one list, so there are no switch statements, name lists, or
// hardcoded chip counts left to update by hand.

#ifndef SYNTHCHIP_H
#define SYNTHCHIP_H

#include <Arduino.h>

class SynthChip
{
public:

  //Display name for this chip, set once in the derived class's constructor.
  const char* name = "Unknown";

  //Capability / limits for the currently selected chip. Set by init().
  int   numberOfRegs = 6;      //number of registers in this chip type
  int   numberOfBits = 32;     //number of bits in each register (informational - not currently read elsewhere)
  float maxPfd = 105.0;        //maximum PFD frequency
  float minPfd = 0;            //minimum PFD frequency
  float maxOsc = 100;          //maximum reference oscillator frequency
  float minOsc = 0;            //minimum reference oscillator frequency
  bool  jt4Only = true;        //lower spec chips only support JT4 (limited fractional register size)
  bool  jtDisable = false;     //lowest spec chips can not do JT modes (limited frequency resolution)

  //--- Required for every chip ---
  virtual void   init(void) = 0;                    //one-time setup for this chip type
  virtual void   update(void) = 0;                  //write the current registers out to the chip
  virtual void   setFrequency(double f) = 0;         //calculate registers for frequency f (MHz) and encode them
  virtual double getFrequency(void) = 0;             //return the frequency (MHz) encoded in the current registers
  virtual void   calcFreq(void) = 0;                 //recalculate/display frequency from the current registers
  virtual void   setDefault(void) = 0;               //reset to this chip's default register values
  virtual void   fskKey(bool key) = 0;                //key up/down for CW ID FSK shift
  virtual void   extKey(bool key) = 0;                //key up/down for external key FSK shift
  virtual void   jtShift(uint8_t val) = 0;            //shift to JT tone number val (0 = nominal)
  virtual void   saveFskShift(void) = 0;             //calculate/store the CW ID FSK shift values
  virtual void   saveKeyShift(void) = 0;             //calculate/store the external key FSK shift values
  virtual void   saveJt(uint8_t index) = 0;           //calculate/store the JT tone shift for index
  virtual double getPfd(void) = 0;                   //return the current PFD frequency

  //--- Optional: default implementation is used unless a chip overrides it ---
  virtual void   decodeRegs(void) {}                 //decode chanData registers into local variables (not all chips need this)
  virtual double calcPfd(double pfd) { return 0; }    //recalculate a valid PFD close to the requested value
  virtual void   setParameters(void)                 //interactive "view/enter variables" menu
  {
    Serial.println();
    Serial.println("Variables are not available for this chip type.");
    Serial.println();
  }
  virtual bool   hasEepromBurn(void) { return false; }//does this chip support burning settings to its own EEPROM?
  virtual void   eepromBurn(void) {}                  //burn settings to the chip's own EEPROM, if supported

  virtual ~SynthChip() {}
};

#endif
