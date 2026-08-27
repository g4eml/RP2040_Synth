// SynthChip.h
// Common interface implemented by every supported RF synthesiser chip driver
// (MAX2870, ADF4351, LMX2595, CMT2119A, ADF5355, ...).
//
// This file (and ChipList.h, redirects.ino, and each chip's own .ino) is shared,
// byte-for-byte identical, between the two sketches that use this chip architecture:
//   - RP2040Synth        (serial-menu only)
//   - RP2040_LCD_Synth   (adds a touchscreen UI)
// They are separate Arduino sketches (the IDE requires each sketch's main .ino to
// match its folder name, and the two boards have different pins/hardware), so
// there is no way to share these files automatically - after following the steps
// below, copy the same updated files into both sketch folders by hand.
//
// To add support for a new chip:
//   1. Create a new class that inherits from SynthChip. In its constructor,
//      set "name" to the chip's display name (see MAX2870.ino for a simple
//      example). Implement the pure virtual methods below; the optional ones
//      only need overriding if they apply to the new chip. If the chip has an
//      adjustable output power or a software output-enable bit, also implement
//      getPower()/setPower()/getOutput()/enableOutput() - these are only used
//      by the touchscreen project, but costs nothing to include in the shared
//      file. If it doesn't have either, override hasPowerControl()/
//      hasOutputControl() to return false instead of guessing at register bits
//      that aren't in the datasheet (see CMT2119A.ino for an example) - the
//      touchscreen UI will grey those controls out rather than fake them.
//   2. Instantiate one global object of the new class (e.g. "MyChip myChip;"
//      at the bottom of the file, outside the class).
//   3. Add one line for it to CHIP_LIST in ChipList.h. Always append to the end,
//      never insert in the middle or reorder - the resulting enum values are
//      saved to EEPROM, so reordering would scramble settings already saved on
//      units in the field.
//   4. Copy the new chip's .ino and the updated ChipList.h into BOTH sketch
//      folders (see note above). At that point enum chipType, chipTable[], the
//      chip count, and the chip-selection menu are all generated automatically
//      from that one list in BOTH projects - nothing else needs to change for
//      the serial-menu (RP2040Synth) project to fully support the new chip.
//   5. Touchscreen project only: the chip-selector row in ConfigScreen.ino is a
//      hand-drawn row of on-screen buttons, so it isn't auto-generated. Add a
//      new #define block for the button's screen position, a drawLabel()/
//      drawOnOff() call in configScreenUpdate(), and a touchZone() handler in
//      doConfigScreen(), following the same pattern used for the other chips
//      (e.g. the CMT2119A button) in that file.
//   6. Verify the new driver's frequency output on real hardware (spectrum
//      analyser/frequency counter) - compiling cleanly only confirms the code
//      is well-formed, not that the register math matches the datasheet.

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
  virtual void   encodeRegs(void) {}                 //encode local variables back into chanData registers (not all chips need this)
  virtual double calcPfd(double pfd) { return 0; }    //recalculate a valid PFD close to the requested value
  virtual void   setParameters(void)                 //interactive "view/enter variables" menu
  {
    Serial.println();
    Serial.println("Variables are not available for this chip type.");
    Serial.println();
  }
  virtual bool   hasEepromBurn(void) { return false; }//does this chip support burning settings to its own EEPROM?
  virtual void   eepromBurn(void) {}                  //burn settings to the chip's own EEPROM, if supported

  //--- Optional: RF output power/enable control, used by the touchscreen UI ---
  //Not every chip exposes an adjustable output power or a software output-enable bit
  //(e.g. CMT2119A's power level is fixed by the RFPDK-programmed EEPROM image, not a
  //documented on-line register, so it can't be controlled here). hasPowerControl() and
  //hasOutputControl() let the UI grey out those controls instead of pretending they work.
  virtual bool    hasPowerControl(void)  { return true; }
  virtual bool    hasOutputControl(void) { return true; }
  virtual uint8_t getPower(void)         { return 0; }      //current output power setting (chip-specific units/range)
  virtual void    setPower(uint8_t p)    {}                  //set output power (chip-specific units/range)
  virtual bool    getOutput(void)        { return true; }    //is RF output currently enabled?
  virtual void    enableOutput(bool o)   {}                  //enable/disable RF output

  virtual ~SynthChip() {}
};

#endif
