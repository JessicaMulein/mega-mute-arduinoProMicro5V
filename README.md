# arduinoProMicro5V

MegaMute is an over-engineered mute button with an inspection / SCADA style monitoring only (via bluetooth or usb serial) of a relay latch lock-out key/remote/master and XLR jacks that are connected/disconnected physically.

Designed for Sparkfun Arduino Pro Micro 5V/16Mhz and a BlueSmirf bluetooth module. (TODO: links)
4 5V Logic IO lines for:
 # 0. Power Good at chain root = PG
 # 1. !Power Good through key interlock = !PGK
 # 2. !Power Good through remote relay / second key override = !PGR
 # 3. !Power Good through final master switch, connected to relay trigger line. Jacks are LIVE/ON-AIR. = !PGMASTER
 
 
