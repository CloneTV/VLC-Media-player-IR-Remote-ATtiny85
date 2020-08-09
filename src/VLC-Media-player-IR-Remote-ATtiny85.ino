/*
  VLC/Media player IR Remote for PC (Windows 10, Linux, Android, Mac)
  Project for ATtiny85 USB stick + TL1838 IR Receiver Module
  TrinketHidCombo library: https://github.com/adafruit/Adafruit-Trinket-USB/tree/master/TrinketHidCombo

  Origin: 10.03.2016 Pawel A. Hernik
  Modify by ClontTv 05.08.2020
  Changed to work with VLC player and removed bugs 
  https://clonetv.github.io/VLC-Media-player-IR-Remote-ATtiny85/

  Features and Capabilities:

  - rewind in both directions,
  - previous/next track,
  - play/pause/stop,
  - PAD support: right, left, up, down, ok
  - change volume, mute,
  - menu - Menu button,
  - change player type: VLC or standard Windows 10 Media player - BLUE button (VLC default),
  - print screen - YELLOW button or On/Off IR receive command,
  - PC sleep - RED button,
  - PC wakeup - GREEN button,
  - exit full screen mode (ESC),
  - keeps state On/Off IR receiver and player type.

  If you notice braking when quickly sequentially pressing keys - comment out the line BUILD_LED_BLINK
*/

/* Comment next line for not use sound volume and mute on PC */
#define BUILD_SOUND_CTRL 1
/* Comment next line for not use blinking LED on key press */
#define BUILD_LED_BLINK 1
/* Comment next line for not use On/Off on press YELLOW button.
 * This will activate the ScreenShot function.
 */
// #define BUILD_ONOFF_IR 1

#include <EEPROM.h>
#include <TrinketHidCombo.h>
#include "IRCodeSkylineRemote.h"
// #include "IRCodeChinaNoNameRemote.h" /* special things :) */

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

#define EEP_ENABLE (byte)3
#define EEP_DISABLE (byte)4
#define EEP_ADDR_ONOF 0
#define EEP_ADDR_PLAYER 1

uint16_t pulse = 0,
         pulses[NUMPULSES][2];
uint32_t irCode = 0U,
         irCodeLast = 0U;
unsigned long lastPress = 0UL;
bool isVlcPlayer;

#if defined(BUILD_ONOFF_IR)
#  define IR_ISENABLE() if ((!isIrOn) && (irCode != IR_YELLOW)) return
#  define IR_SET_LAST() isIrOn = epprom_get(EEP_ADDR_ONOF, true)
#  define IR_ENABLE_LED() digitalWrite(LEDpin, isIrOn)
   bool isIrOn;
#else
#  define IR_ISENABLE()
#  define IR_SET_LAST()
#  define IR_ENABLE_LED()
#endif

#if defined(BUILD_LED_BLINK)
#  define LED_ON() isLedLight = true
#  define LED_OFF() if (digitalRead(LEDpin)) isLedLight = false
#  define LED_CHECK() ledCheck()
#  define LED_TRIGGER() digitalWrite(LEDpin, isLedLight)
   bool isLedLight = false;
#else
#  define LED_ON()
#  define LED_OFF()
#  define LED_CHECK()
#  define LED_TRIGGER()
#endif

#define PLAYER_SET_LAST() isVlcPlayer = epprom_get(EEP_ADDR_PLAYER, true)

void setup() {
  pinMode(LEDpin, OUTPUT);
  pinMode(IRpin, INPUT);
  TrinketHidCombo.begin();
  PLAYER_SET_LAST();
  IR_SET_LAST();
  IR_ENABLE_LED();
}

void loop()
{
  TrinketHidCombo.poll();
  LED_ON();
  
  uint16_t numpulse = listenForIR();
  for (int i = 0; i < 32; i++) {
    irCode = irCode << 1;
    if (((pulses[i][0] * RESOLUTION) > 0) && ((pulses[i][0] * RESOLUTION) < 500)) {
      irCode |= 0;
    } else {
      irCode |= 1;
    }
  }
  
  IR_ISENABLE();
  int repeat = ((irCode != irCodeLast) || ((millis() - lastPress) > 200));
  switch (irCode) {
#   if defined(BUILD_SOUND_CTRL)
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
#   endif
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
    case IR_BLUE: {
      if(repeat) {
        isVlcPlayer = !isVlcPlayer;
        epprom_set(EEP_ADDR_PLAYER, isVlcPlayer);
      }
      break;
    }
    case IR_YELLOW: {
      if(repeat) {
#       if defined(BUILD_ONOFF_IR)
        isIrOn = !isIrOn;
        digitalWrite(LEDpin, isIrOn);
        epprom_set(EEP_ADDR_ONOF, isIrOn);
#       else
        TrinketHidCombo.pressKey(0,KEYCODE_PRINTSCREEN);
        TrinketHidCombo.pressKey(0,0); 
#       endif
      }
      break;
    }
    case IR_RED: {
      if(repeat) {
        TrinketHidCombo.pressKey(0,SYSCTRLKEY_SLEEP);
        TrinketHidCombo.pressKey(0,0); 
      }
      break;
    }
    case IR_GREEN: {
      if(repeat) {
        TrinketHidCombo.pressKey(0,SYSCTRLKEY_WAKE);
        TrinketHidCombo.pressKey(0,0); 
      }
      break;
    }
    default: {
      LED_OFF();
      break;
    }
  }
  LED_TRIGGER();
  irCodeLast = irCode;
  lastPress = millis();
}

// ----------------------

uint16_t listenForIR() {
  pulse = 0;
  
  while (true) {
    uint32_t hp = 0U, lp = 0U;
    while (IRpin_PIN & _BV(IRpin)) {
      LED_CHECK();
      hp++;
      delayMicroseconds(RESOLUTION);
      if (((hp >= MAXPULSE) && (pulse != 0)) || (pulse == NUMPULSES))
        return pulse;
      if (hp >= MAXPULSE)
        TrinketHidCombo.poll();
    }
    pulses[pulse][0] = hp;

    while (!(IRpin_PIN & _BV(IRpin))) {
      LED_CHECK();
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

static inline bool epprom_get(int addr, bool b) {
  switch(EEPROM.read(addr)) {
    case EEP_ENABLE: return true;
    case EEP_DISABLE: return false;
    default: return b;
  }
}
static inline void epprom_set(int addr, bool b) {
  EEPROM.write(addr, ((b) ? EEP_ENABLE : EEP_DISABLE));
}

#if defined(BUILD_LED_BLINK)
static inline void ledCheck() {
    if (isLedLight && ((millis() - lastPress) > 500)) {
      digitalWrite(LEDpin, false);
      isLedLight = false;
    }
}
#endif
