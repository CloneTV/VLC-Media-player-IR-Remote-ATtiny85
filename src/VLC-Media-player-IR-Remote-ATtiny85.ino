/*
  VLC/Media player IR Remote for PC (Windows 10)
  Project for ATtiny85 USB stick + TL1838 IR Receiver Module
  TrinketHidCombo library: https://github.com/adafruit/Adafruit-Trinket-USB/tree/master/TrinketHidCombo

  Origin: 10.03.2016 Pawel A. Hernik
  Modify by ClontTv 05.08.2020
  Changed to work with VLC player and removed bugs 
  https://clonetv.github.io/VLC-Media-player-IR-Remote-ATtiny85-/
  
  To use the standard Windows 10 media player, press BLUE button: 
*/

#include <TrinketHidCombo.h>

#define IRpin_PIN PINB
#define IRpin 2
#define LEDpin 1

#define MAXPULSE    5000
#define NUMPULSES    50
#define RESOLUTION    2

#define MMKEY_REV         0xB5
#define MMKEY_FWD         0xB6
#define MMKEY_PREV_TRACK  0xB5
#define MMKEY_NEXT_TRACK  0xB6

uint16_t pulse = 0,
         pulses[NUMPULSES][2];
uint32_t irCode = 0U,
         irCodeLast = 0U;
unsigned long lastPress = 0UL;
bool isLedLight = false,
     isVlcPlayer = true;

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
#define IR_RED   0xC13EE1
#define IR_GREEN 0xC13EE2
#define IR_YELLOW 0xC13EE3
#define IR_BLUE  0xC13EE4


void setup() {
  pinMode(LEDpin, OUTPUT);
  pinMode(IRpin, INPUT);
  TrinketHidCombo.begin();
}

void loop()
{
  TrinketHidCombo.poll();
  isLedLight = true;

  uint16_t numpulse = listenForIR();
  for (int i = 0; i < 32; i++) {
    irCode = irCode << 1;
    if (((pulses[i][0] * RESOLUTION) > 0) && ((pulses[i][0] * RESOLUTION) < 500)) {
      irCode |= 0;
    } else {
      irCode |= 1;
    }
  }
  int repeat = ((irCode != irCodeLast) || ((millis() - lastPress) > 200));
  switch (irCode) {
    case IR_MUTE: {
      if(repeat)
        TrinketHidCombo.pressMultimediaKey(MMKEY_MUTE);
      break;
    }
    case IR_VOLUP: {
      TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_UP);
      break;
    }
    case IR_VOLDN: {
      TrinketHidCombo.pressMultimediaKey(MMKEY_VOL_DOWN);
      break;
    }
    case IR_PLAY: {
      if(repeat)
        TrinketHidCombo.pressMultimediaKey(MMKEY_PLAYPAUSE);
      break;
    }
    case IR_STOP: {
      if(repeat)
        TrinketHidCombo.pressMultimediaKey(MMKEY_STOP);
      break;
    }
    case IR_PREV: {
      if(repeat)
        TrinketHidCombo.pressMultimediaKey(MMKEY_PREV_TRACK);
      break;
    }
    case IR_NEXT: {
      if(repeat)
        TrinketHidCombo.pressMultimediaKey(MMKEY_NEXT_TRACK);
      break;
    }
    case IR_REW: {
      if (isVlcPlayer) {
        TrinketHidCombo.pressKey(0,KEYCODE_ARROW_LEFT);
        TrinketHidCombo.pressKey(0,0); 
      } else {
        TrinketHidCombo.pressMultimediaKey(MMKEY_FWD);
      }
      break;
    }
    case IR_FFWD: {
      if (isVlcPlayer) {
        TrinketHidCombo.pressKey(0,KEYCODE_ARROW_RIGHT);
        TrinketHidCombo.pressKey(0,0); 
      } else {
        TrinketHidCombo.pressMultimediaKey(MMKEY_REV);
      }
      break;
    }
    case IR_MENU: {
      if (repeat)
        TrinketHidCombo.pressMultimediaKey(MMKEY_KB_MENU);
      break;
    }
    case IR_EXIT: {
      TrinketHidCombo.pressKey(0,KEYCODE_ESC);
      TrinketHidCombo.pressKey(0,0); 
      break;
    }
    case IR_LEFT: {
      TrinketHidCombo.pressKey(0,KEYCODE_ARROW_LEFT);
      TrinketHidCombo.pressKey(0,0); 
      break;
    }
    case IR_RIGHT: {
      TrinketHidCombo.pressKey(0,KEYCODE_ARROW_RIGHT);
      TrinketHidCombo.pressKey(0,0); 
      break;
    }
    case IR_UP: {
      if (repeat){
        TrinketHidCombo.pressKey(0,KEYCODE_ARROW_UP);
        TrinketHidCombo.pressKey(0,0); 
      }
      break;
    }
    case IR_DOWN: {
      if (repeat){
        TrinketHidCombo.pressKey(0,KEYCODE_ARROW_DOWN);
        TrinketHidCombo.pressKey(0,0); 
      }      
      break;
    }
    case IR_OK: {
      if(repeat) {
        TrinketHidCombo.pressKey(0,KEYCODE_SPACE);
        TrinketHidCombo.pressKey(0,0); 
      }
      break;
    }
    default: {
      if (digitalRead(LEDpin))
        isLedLight = false;
      break;
    }
  }
  if (isLedLight)
    digitalWrite(LEDpin, true);
  else
    digitalWrite(LEDpin, false);

  irCodeLast = irCode;
  lastPress = millis();
}

// ----------------------

uint16_t listenForIR() {
  pulse = 0;
  
  while (true) {
    uint32_t hp = 0U, lp = 0U;
    while (IRpin_PIN & _BV(IRpin)) {
      ledOff();
      hp++;
      delayMicroseconds(RESOLUTION);
      if (((hp >= MAXPULSE) && (pulse != 0)) || (pulse == NUMPULSES))
        return pulse;
      if (hp >= MAXPULSE)
        TrinketHidCombo.poll();
    }
    pulses[pulse][0] = hp;

    while (!(IRpin_PIN & _BV(IRpin))) {
      ledOff();
      lp++;
      delayMicroseconds(RESOLUTION);
      if (((lp >= MAXPULSE) && (pulse != 0)) || (pulse == NUMPULSES))
        return pulse;
      if (lp >= MAXPULSE)
        TrinketHidCombo.poll();
    }
    pulses[pulse][1] = lp;
    pulse++;
  }
}

static inline void ledOff() {
    if (isLedLight && ((millis() - lastPress) > 500)) {
      digitalWrite(LEDpin, false);
      isLedLight = false;
    }
}
