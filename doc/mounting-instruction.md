# Mounting Instruction

## **Component Overview**

the device kit consists of this parts:

- Arduino Xiao Board
- Enocean TCM310 Module
- 220uF / 35V Elko
- Push Button
- KNX Pins
- SMA Antenna Connector (female)

<img src="pic/Bestueckung_Bot.png"  width="250" />
<img src="pic/Bestueckung_Top.png"  width="380" />

--------------------------------------------------

## **final assembly**

Note: the SMA connector is only needed in case you want to use an external Antenna. For more information see [link](https://gitlab.com/DIY-Smarthome-Projekt/knx-enocean-gateway/-/blob/main/doc/Antennen_Design.md)

<img src="pic/Bestueckung_Bot_final.png"  width="310" />
<img src="pic/Bestueckung_Top_final.png"  width="320" />

--------------------------------------------------

## **special solder points**

<img src="pic/SolderingReset.png"  width="310" />

It is only necessary to solder that pad with the yellow arrow. You can test the solder connection if you press the reset button and check if the board do a reset. The other three pads are only necessary in case you want to use a JTAG connection (green arrows)

<img src="pic/SolderingVCC.png"  width="320" />

This pads you do not need to solder (red arrows)
