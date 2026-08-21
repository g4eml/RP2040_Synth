// ChipList.h
// The single master list of supported chip types.
//
// enum chipType (in RP2040Synth.ino) and chipTable[] (in redirects.ino) are
// both generated from this one list, so they can never drift out of sync
// with each other, and the number of supported chips never needs to be
// hardcoded anywhere.
//
// IMPORTANT: this list must stay in the same order as it is today (MAX2870,
// ADF4351, LMX2595, CMT2119A, ADF5355, ...) - the resulting enum values are
// what gets saved as "chip" in the RP2040's EEPROM, so re-ordering it would
// change the meaning of settings already saved by units in the field. New
// chip types must always be appended at the end.
//
// To add a new chip type:
//   1. Write its SynthChip-derived class (see SynthChip.h) and instantiate
//      one global object of it, e.g. "MyChipClass myChipInstance;".
//   2. Add one line below: CHIP_ENTRY(EnumName, myChipInstance)
// That's it - the enum value, chipTable[] entry, and chip count are all
// generated automatically from this one line.

#ifndef CHIPLIST_H
#define CHIPLIST_H

#define CHIP_LIST                    \
  CHIP_ENTRY(MAX2870,  max2870Chip)  \
  CHIP_ENTRY(ADF4351,  adf4351Chip)  \
  CHIP_ENTRY(LMX2595,  lmx2595Chip)  \
  CHIP_ENTRY(CMT2119A, cmt2119aChip) \
  CHIP_ENTRY(ADF5355,  adf5355Chip)

#endif
