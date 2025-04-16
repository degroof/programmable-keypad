# programmable-keypad
Arduino code for a 2×3 programmable keypad

This Arduino code is intended for use with an Adafruit Trinket M0 and six key switches.
The key switches should be arranged in three rows and two columns. 
The keys should be wired to the five IO pins on the Trinket (3 for rows, 2 for columns). The order doesn't matter; you can reconfigure in the Arduino code (rowPins, colPins). 
No need for anti-ghosting diodes since you'll never need to press more than 2 keys at a time.

Pressing the bottom 2 keys similtaneously will toggle numlock.
Pressing the middle 2 keys simultaneously will put it into programming mode. You'll need to have it connected to a Windows PC and have a text editor open, preferably with a mono font set.

The code uses an eeprom emulator to store key settings to flash memory (the Trinket doesn't have eeprom on board).

2025-04-16 - Added keypad_interact_rpt.ino (allows key repetition)
