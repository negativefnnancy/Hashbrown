# Hashbrown

A fantasy console! Codename: Hashbrown!
(I say fantasy, though I intend to build a real one at some point!)

There's not much to see here yet, but here are some fun ideas:

internal specs:
- possible cpu ideas
    - dual risc v cores (fpga) [second core with vector extensions]
    - dual arm cores
    - custom cpu ? ? ? (fpga)
- custom sound chip! FM + ring mod + nes-like 1-bit noise generator
- custom ds-like graphics (fpga)
- 8MB(?) ram shared by cores

ports and slots:
- cartridge/expansion bus (top, back)
    - stereo audio input for external sound hardware
    - dual address/data bus, one for each processor core [cartridge hardware is directly addressable via cpu bus]
- dual memory card buses (maybe sd card slots) (top)
- uart over 5 pin din connector (top center)
- jtag pin header on dev model (top left)
- special edition composite video out + pre-amp audio out over 4 pin 3.5mm jack (bottom)
- headphone jack (bottom)
- dc barrel connector for power (bottom)

external specs:
- switch style controller scheme and overral physical layout
    - volume wheel (top right)
    - power switch/sleep button (bottom)
- resistive 480x320 touch screen

example custom hardware for carts:
- neural chip??
- analog sound synthesis circutry
- pressure sensor (for blowing thru a straw)
- ray tracing core

exmalpe uart uses:
- midi controllers
- keyboard/mouse
- network adapter for netplay
- connect to pc
