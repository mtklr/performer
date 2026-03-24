# Display mods

The PER|FORMER has been known for some noise associated with the display. Although this probably belongs in [performer-hardware](https://github.com/westlicht/performer-hardware/), here's some info about modifying the display power to potentially lower noise.

* Older display versions do not have these jumpers to change how the display is powered. This was the case with a white version I bought from Mouser a while back, I guess it was whatever they had in stock at the time.

You can buy direct from [New Haven Display](https://newhavendisplay.com/). I recently bought a yellow display and received the current version with jumpers.

## Display Jumper Settings

### Default Jumper Setting

According to [specs](https://newhavendisplay.com/content/specs/NHD-3.12-25664UCY2.pdf):

>OLED Logic Circuit + Boost converter + OLED panel are powered from VDD (pin #2). This allows the full module to be powered by a single low-voltage supply.
>
>Supply Current for Module
><br>
>IDD - VDD=3.3V, 100% ON, Min. -, Typ. 310, Max. 340 mA

This is how the display is powered by default, and how it works in the PER|FORMER. Simple, works fine, 340 mA current draw.

### Jumper Option #1

Independent supply voltage for boost converter (BC\_VDD)

>R14 = open, R15 = CLOSE, R18 = open, R1 = open
>
>Boost converter + OLED panel are powered from BC\_VDD (pin #3). OLED Logic Circuit is powered from VDD (pin #2). This allows for increased efficiency through the boost converter, by allowing a supply voltage up to +12V at its input, BC\_VDD (pin #3).
>
>Supply Current for Module
><br>
>IDD, VDD=3.3V, Min. -, Typ. 170, Max. 200 µA
>
>Supply Current for Boost Converter
><br>
>BC\_IDD, BC\_VDD=5.0V, 100% ON, Min. -, Typ. 150, Max. 170 mA
><br>
>BC\_IDD, BC\_VDD=12.0V, 100% ON, Min. -, Typ. 55, Max. 70 mA

This seemed a bit too complicated, and isn't as efficient as:

### Jumper Option #2

External supply voltage for OLED panel (VCC)

>R14 = open, R15 = open, R18 = CLOSE, R1 = open
>
>OLED panel is powered from VCC (pin #15) – boost converter is not used. OLED Logic Circuit is powered from VDD (pin #2). **This allows for maximum module efficiency, and drastically reduced total current consumption.**
>
>Supply Current for Module
><br>
>IDD, VDD=3.3V, Min. -, 170 Typ., 200 Max. µA
>
>Supply Voltage for OLED Panel
><br>
>VCC, 11.5 Min., 12 Typ., 12.5 Max. V
><br>
>ICC, VCC=12V, 100% ON, Min. -, Typ. 45, Max. 55 mA

Pretty easy to hook up:

1. Move jumper from R14 to R18.
2. Connect wire from input of DC converter (U16 pin 1, top of PCB) to pin 15 (NC) of the display. I removed pin 15 from the display header to break its connection to the PCB, and soldered this end of the wire in its place.

This adds ~55mA to the +12V current draw for the module, which Westlicht states as 100mA, so call it 12V @ ~160mA.

### Jumper Option #3

External supply voltage for internal regulator (G\_VDD)

This is mentioned in the specs, but never fully explained...

## DC Converter (U16)

Lower noise (i.e. more expensive) parts are available, see Recom Series R-78K-1.0, R-78-1.0, R-78B-1.0, R-78C-1.0 (haven't explored other brands).

Default U16 part: Recom R-78E5.0-1.0 - [Datasheet](https://g.recomcdn.com/media/Datasheet/pdf/.ft\_PFiEZ/.tac185dad0176a7816122/Datasheet-274/R-78E-1\_0.pdf)

>Output ripple/noise min (unspecified), typ (unspecified), max 120mVp-p

R-78K-1.0 - [Datasheet](https://g.recomcdn.com/media/Datasheet/pdf/.f-qnjhnZ/.tea817ab1782bc0523037/Datasheet-551/R-78K-1\_0.pdf)

>Output ripple/noise min (unspecified), typ 50mVp-p, max (unspecified)

R-78-1.0 - [Datasheet](https://g.recomcdn.com/media/Datasheet/pdf/.fAtFDPZY/.t973c52ae2da39e2e8e08/Datasheet-273/R-78-1\_0.pdf)

>Output ripple and Noise min (unspecified), typ 20mVp-p, max 30mVp-p)

I tried to order some samples from them and never heard back...

## External 5VDC supply

Another option is replacing the converter with a direct connection to a separate 5VDC supply (pin 12 of a modular PSU with 5V, for example). Remove U16 and wire to Vin and Ground pins.

[modwiggler thread](https://modwiggler.com/forum/viewtopic.php?p=4341025#p4341025)

![External 5VDC](https://modwiggler.com/forum/download/file.php?id=211705&mode=view)

## Display contrast current (firmware)

I've also tried reducing the display contrast current.

Files:
<br>
[src/apps/bootloader/Lcd.cpp](https://github.com/mtklr/performer/blob/mk/src/apps/bootloader/Lcd.cpp)
<br>
[src/platform/stm32/drivers/Lcd.cpp](https://github.com/mtklr/performer/blob/master/src/platform/stm32/drivers/Lcd.cpp)

```
{ 0xc1, 1, 0xff }, // Set Contrast Current
{ 0xc7, 1, 0x0f }, // Master Contrast Current Control | 0x0f = no change
```

I halved these values (`0xff -> 0x7f`, `0x0f -> 0x07`). Requires recompiling and updating the firmware. Display looks fine not at full blast, they could probably be lowered even more.

[SSD1322 Display driver documentation](https://support.newhavendisplay.com/hc/en-us/article_attachments/4414477845911)

## Etc.

Other factors include power supplies and grounds. Search the [modwiggler](https://www.modwiggler.com/forum/viewtopic.php?t=214360) [threads](https://modwiggler.com/forum/viewtopic.php?t=213251) for more info.

The [djphazer fork](https://github.com/djphazer/performer/) incorporates some changes from the [jackpf fork](https://github.com/jackpf/performer) that add brightness control and other mods to [help with display noise](https://github.com/jackpf/performer/pull/5).

## TODO

Hook up an oscilloscope to see if any of this actually makes a difference...:)
