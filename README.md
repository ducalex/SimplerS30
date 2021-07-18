# SimplerS30

This is a RetroArch firmware for the PocketGo S30 aiming to provide a pick-up-and-play experience.

Simpler30 respects RetroArch's default settings, `base_s30.cfg` contains only what is required to run correctly on the S30. 
Playlists are generated automatically at boot for a true pick-up-and-play experience!


# Installation
Copy the content of the sdcard folder to the root of your FAT32 SD Card.


# Future Plans

- Periodically keep up with new emulator versions
- Port some non-libretro games (add a way to start PPSSPP for example)


# Technical information

## Buttons mapping

The buttons on the S30 map to keyboard keys as follows:

| S30 btn | Retroarch  | Scancode |
|---------|------------|----------|
| Power   | escape     | 
| Vol +   | add        | 
| Vol -   | subtract   | 
| L1      | lbrace     | 
| L2      | leftparen  | 
| R1      | rbrace     | 
| R2      | rightparen | 
| A       | ctrl       | 
| B       | alt        | 
| X       | rctrl      | 
| Y       | ralt       | 
| Start   | enter      | 
| Select  | select     | 
| Up      | up         | 
| Down    | down       | 
| Left    | left       | 
| Right   | right      | 



# Acknowledgements

- While this project is no longer based on [SimpleS30 by retrogamecorps](https://retrogamecorps.com), it was an invaluable resource!
- bkacjios's [RetroArch](https://github.com/bkacjios/RetroArch), which I believe is the one I use (I took it from Simple30).
- The toolchain was shared by [slaminger](https://github.com/slaminger).
