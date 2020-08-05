/*
  Scanner IR code part for : VLC/Media player IR Remote for PC (Windows 10)
  Project for ATtiny85 USB stick + TL1838 IR Receiver Module
  TrinketKeyboard library: https://github.com/adafruit/Adafruit-Trinket-USB/tree/master/TrinketKeyboard

  ClontTv 05.08.2020
  https://clonetv.github.io/VLC-Media-player-IR-Remote-ATtiny85-/
*/

#include <TrinketKeyboard.h>

#define IRpin_PIN PINB
#define IRpin     2
#define LEDpin    1

#define MAXPULSE    5000
#define NUMPULSES   50
#define RESOLUTION  2

uint16_t pulse = 0,
         pulses[NUMPULSES][2];
uint32_t irCode = 0U,
         irCodeLast = 0U;
unsigned long lastPress = 0UL;

void setup()
{
  pinMode(LEDpin, OUTPUT);
  pinMode(IRpin, INPUT);
  TrinketKeyboard.begin();
}

void loop()
{
  TrinketKeyboard.poll();
  digitalWrite(LEDpin, false);

  uint16_t numpulse = listenForIR();
  for (int i = 0; i < 32; i++) {
    irCode = irCode << 1;
    if ((pulses[i][0] * RESOLUTION) > 0 && (pulses[i][0] * RESOLUTION) < 500) {
      irCode |= 0;
    } else {
      irCode |= 1;
    }
  }

  int repeat = ((irCode != irCodeLast) || ((millis() - lastPress) > 200));
  TrinketKeyboard.print("code: ");
  TrinketKeyboard.println(irCode, HEX);
  digitalWrite(LEDpin, true);

  irCodeLast = irCode;
  lastPress = millis();
}

uint16_t listenForIR() {
  pulse = 0;
  
  while (true) {
    uint32_t hp = 0U, lp = 0U;
    while (IRpin_PIN & _BV(IRpin)) {
      hp++;
      delayMicroseconds(RESOLUTION);
      if (((hp >= MAXPULSE) && (pulse != 0)) || (pulse == NUMPULSES))
        return pulse;
      if (hp >= MAXPULSE)
        TrinketKeyboard.poll();
    }
    pulses[pulse][0] = hp;

    while (!(IRpin_PIN & _BV(IRpin))) {
      lp++;
      delayMicroseconds(RESOLUTION);
      if (((lp >= MAXPULSE) && (pulse != 0)) || (pulse == NUMPULSES))
        return pulse;
      if (lp >= MAXPULSE)
        TrinketKeyboard.poll();
    }
    pulses[pulse][1] = lp;
    pulse++;
  }
}
