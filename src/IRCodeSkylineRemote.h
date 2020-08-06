/*
  VLC/Media player IR Remote for PC (Windows 10)
  Project for ATtiny85 USB stick + TL1838 IR Receiver Module
  TrinketHidCombo library: https://github.com/adafruit/Adafruit-Trinket-USB/tree/master/TrinketHidCombo

  Origin: 10.03.2016 Pawel A. Hernik
  Modify by ClontTv 05.08.2020
  Changed to work with VLC player and removed bugs 
  https://clonetv.github.io/VLC-Media-player-IR-Remote-ATtiny85/
*/

#define IR_MUTE  0xC13EF28D
#define IR_VOLUP 0xC13EF00F
#define IR_VOLDN 0xC13EE09F
#define IR_PLAY  0xC13EE897
#define IR_STOP  0xC13EF689
#define IR_PREV  0xC13EC8B7
#define IR_NEXT  0xC13EC2BD
#define IR_EXIT  0xC13ED42B
#define IR_MENU  0xC13ED827
#define IR_REW   0xC13ED22D
#define IR_FFWD  0xC13EE29D
#define IR_OK    0xC13ED4AB
#define IR_LEFT  0xC13EE49B
#define IR_RIGHT 0xC13EF48B
#define IR_UP    0xC13EF887
#define IR_DOWN  0xC13EEC93
#define IR_RED   0xC13ECEB1
#define IR_GREEN 0xC13EEE91
#define IR_YELLOW 0xC13EDEA1
#define IR_BLUE  0xC13EFE81
