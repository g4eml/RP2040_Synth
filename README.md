# RP2040Synth

## Description

A Fractional N synthesiser controller based on the RP2040 processor.  This currently supports 3 different type of synthesiser chip, the ADF4351, the Max2870 and the LMX2595. 



## Features

Serial programming using the RP2040s built in USB serial port. 

Settings can be saved to EEPROM for automatic load on power on.

Support for FSK CW Identification for beacon use.

Default register settings can be loaded for all synthesiser types to produce some initial RF output. This is often the hardest thing to do with a new chip type. 

Direct output frequency and phase detector frequency entry. Register values are calculated automatically.  

Direct data sheet parameter entry for 'fine tuning' the default settings.

Direct register entry for loading a configuration from another program.



## Requirements

This code should work on any RP2040 based board. Recommended is the RP2040 Zero which is very small. The original Raspberry Pi Pico can also be used but this board is physically larger.

Suitable Synthesiser boards are available from Ebay, Amazon, Ali Express etc. 

## Programming or updating the RP2040 (quick method)

This can be done either before or after the Pico has been connected, it makes no difference. Updating to a new firmware version is done the same way.   

1. Locate the latest compiled firmware file 'RP2040_Synth.uf2' which will be found here https://github.com/g4eml/RP2040_Synth/releases and save it to your desktop. 

2. Hold down the BOOTSEL button on the RP2040 module while connecting it to your PC using its USB port. The RP2040 should appear as a USB disk drive on your PC.

3. Copy the .uf2 file onto the USB drive. The RP2040 will recognise the file and immediately update its firmware and reboot.

## Building your own version of the firmware (longer method and not normally required)

The RP2040 is programmed using the Arduino IDE with the Earl F. Philhower, III  RP2040 core. 

#### Installing the Arduino IDE

1. Download and Install the Arduino IDE 2.3.0 from here https://www.arduino.cc/en/software

2. Open the Arduino IDE and go to File/Preferences.

3. in the dialog enter the following URL in the 'Additional Boards Manager URLs' field: https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

4. Hit OK to close the Dialog.

5. Go to Tools->Board->Board Manager in the IDE.

6. Type “RP2040” in the search box.

7. Locate the entry for 'Raspberry Pi Pico/RP2040 by Earle F. Philhower, III' and click 'Install'

#### Downloading the Software.

1. Download the latest released source code .zip file from https://github.com/g4eml/RP2040_Synth/releases

2. Save it to a convenient location and then unzip it. 

#### Programming the RP2040

1. Open the Arduino IDE and click File/Open

2. Navigate to the File RP2404Synth/RP2040Synth.ino (downloaded in the previous step) and click Open.

3. Select Tools and make the following settings.

   Board: "Waveshare RP2040 Zero"        (or whatever board you are using)

   Debug Level: "None"

   Debug Port: "Disabled"

   C++ Exceptions: "Disabled"

   Flash Size: "2Mb (no FS)"

   CPU Speed: "133MHz"

   IP/Bluetooth Stack: "IPV4 Only"

   Optimise: "Small (-Os)(standard)"

   RTTI: "Disabled"

   Stack Protection: "Disabled"

   Upload Method: "Default (UF2)"

   USB Stack: "Pico SDK"  

5. Connect the RP2040 to the USB port while holding down the BOOTSEL button. 

6. Click Sketch/Upload.

The Sketch should compile and upload automatically to the Pico. If the upload fails you may need to disconnect the board and then hold down the BOOTSEL button while reconnecting. 

## Connections

The pinout of the various Synthesiser modules tends to vary so it is difficult to give exact wiring diagrams. The RP2040 modules also have different pinouts. 
The generic wiring instructions are as follows:-

RP2040      ADF4351      MAX2870      LMX2595
5V            N/C         N/C            5V
3V3           3V3         3V3            N/C
GND           GND         GND            GND
GPO3          CE          CE             CE  
GPO4          MUX         MUX            MUX
GPO5          LE          LE             LE
GPO6          CLK         CLK            CLK
GPO7          DAT         DATA           DAT


## Firmware description

By default on power up the firmware will immediately send the EEPROM saved register values to the sunthesiser chip.  If the CW ident feature is enabled the ident will begin. 

In normal use, for example as a Local oscillator or Beacon, that is all that is needed! 



To enter programming mode  (which you will need to do at least once) you need to connect to the USB serial port using a terminal program such as Putty or Terraterm. Pressing any key should result in the menu being displayed.
![Screenshot (106)](https://github.com/g4eml/RP2040_Synth/assets/1881884/8ec6a252-c24e-4df1-89dc-52bb52c28a04)

### Commands
Commands are entered by a single key press. Text and numbers require a carriage return to enter. Pressing Question Mark will usually give more detailed help. 
Changes to registers and parameters will be applied immediatey so you should be able to observe the result of the change in real time. 

T = Select Chip Type. Allows the Synthesiser chip type to be selected. Normally only needed the first time you configure the firmware. 

D = Set Default Register Values for chip. Sets default values to all registers. Tries to program a 10Mhz PFD and requests a frequency. After this your Synthesiser should start to output RF. 

O = Set Reference Oscillator Frequency. Sets the Reference Oscillator Frequency. Enter the actual frequency of your reference including any frequency error. The firmware will attempt to adjust for this. 

P = Enter PFD Frequency. Set the required PFD frequency. The firmware will attempt to get as close as possible to the requested value but not all values can be achieved. 

F = Enter Output Frequency. Set the required output frequency. The firmware will attempt to calculate the closest register values to achieve this. Changing the PFD may improve the result. 

C = Calculate and display frequency from current settings. The firmware will calculte the expected output frequency from the current register settings. Useful to check after youy make manual changes.

V = View / Enter Variables for Registers. Allows viewing or entry of parameters using the same names as defined in the chip datasheet. Press ? for a full list of the available parameter names. 

R = View / Enter Registers Directly in Hex. Allws direct entry of regiser values in Hexadecimal. Useful when transfering values calculated by another program. 

I = Configure CW Ident. Alows entry of CW Ident, CW Speed, Ident Period and FSK Shift.  A shift of -800Hz is a typical value. 

S = Save Registers to EEPROM. Saves the current Synthesiser settings to EEPROM. They will then be automatically loaded on the next power cycle. You must do this at least once. 

X = Exit Menu. Exits from the menu, re-initialises the Synthesiser and starts the CW Ident. 

## 
