/*
  VLC/Media player IR Remote for PC (Windows 10)
  Project for ATtiny85 USB stick + TL1838 IR Receiver Module
  TrinketHidCombo library: https://github.com/adafruit/Adafruit-Trinket-USB/tree/master/TrinketHidCombo

  Origin: 10.03.2016 Pawel A. Hernik
  Modify by ClontTv 05.08.2020
  Changed to work with VLC player and removed bugs 
  https://clonetv.github.io/VLC-Media-player-IR-Remote-ATtiny85/
*/


#define IR_MUTE  0xC03FE41B  // 9
#define IR_VOLUP 0xC03FEA15  // 8
#define IR_VOLDN 0xC03FE619  // 0
#define IR_PLAY  0xC03FE897  // 1
#define IR_STOP  0xC03FD8A7  // 2
#define IR_PREV  0xC03FC8B7  // 4
#define IR_NEXT  0xC03FC0BF  // 5
#define IR_EXIT  0xC0000001  // --
#define IR_MENU  0xC0000002  // --
#define IR_REW   0xC03FF887  // 3
#define IR_FFWD  0xC03FF08F  // 6
#define IR_OK    0xC03FCE31  // OK
#define IR_LEFT  0xC03FC43B  // left
#define IR_RIGHT 0xC03FD6A9  // right
#define IR_UP    0xC03FC639  // up
#define IR_DOWN  0xC03FD2AD  // down
#define IR_RED   0xC03FDA25  // *
#define IR_GREEN 0xC03FEC13  // #
#define IR_YELLOW 0xC03FF807 // 7
#define IR_BLUE  0xC0000003  // --
