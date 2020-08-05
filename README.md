# VLC/Media player IR Remote - homemade usb stick

 This project for `ATtiny85` USB stick + `TL1838` IR Receiver Module or compatible.  
 The stick can be controlled from the `Skyline TV` remote control and others with a similar remote control, VLC player or standard Media Player on Windows 10.  
 No software required for Windows 10 or special drivers.
 
## Features and Capabilities:

- change player type: `VLC` or `Media player`,
- `rewind` in both directions,
- `previous`/`next` track,
- `play`/`pause`/`stop`,
- PAD support: `right`, `left`, `up`, `down`, `ok`
- change volume,
- menu,
- exit full screen mode (`ESC`)

## Required:

- `ATtiny85` USB stick - 1 pcs.
- `TL1838` IR Receiver Module - 1 pcs.  
- [TrinketHidCombo Adafruit library](https://github.com/adafruit/Adafruit-Trinket-USB/tree/master/TrinketHidCombo)  
- [TrinketKeyboard Adafruit library](https://github.com/adafruit/Adafruit-Trinket-USB/tree/master/TrinketKeyboard)  
- `TrinketKeyboard` library required only for build [`IR Scanner`](/blob/master/src/IR-Scanner-ATtiny85.ino) part.
- IR Remote for **PC only**! tested on Windows 10.

## VLC advanced settings:

![screenshot VLC settings](https://clonetv.github.io/VLC-Media-player-IR-Remote-ATtiny85-/vlc-interfaces.png)

## LICENSE

MIT
