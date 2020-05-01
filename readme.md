# Art Deco Earrings
Shimmering electronic earrings in the Art Deco style. Dangle style earring that just lightly shimmers when you are still, and glow brightly when dancing. The lights travel down the length of the earring revealing movement over time.

![shimmer]

The earrings use an IMU, sensing changes in acceleration and rotation and based on the magnitude of the change, the brightness is set. Each of the three axes of motion are mapped to a color so the color depends on the way you move.

## Design
The design follows the Art Deco style. Clean sharp lines, hard contrast, metallics including gold, and bold colors. A focus on materials and glamour. This can be seen in the sharp point on the bottom or the light ray motif running along the sides along with other small touches across the board.

![render-front]

### Electrical
The electrical design is made to be very minimal and manufacturable. Keeping all parts on one side and using no smaller the 0402 components helps with that. Total packages are also kept to a minimum to simplify assembly. The circuit has voltage range of 3.3V to 5.5V. It is expected to consume 5mA in low activity and about 25mA in high activity. A theoretical maximum consumption is 133.49mA. 

![schematic]

The [`schematic.pdf`](./docs/schematic.pdf) version is also available.

#### Microcontroller
Microchip's [ATtiny20](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf) in a QFN 4mmx4mm package was selected due to its size. Without getting into WLCSP, selecting a device with the correct peripherals that is smaller is difficult. It is a reasonably low cost 8-bit processor, able to be programmed through the Arduino IDE, somewhat low power, and could be prototyped with the PDIP package. It has limited RAM, but with only eight LEDs that is not a problem. 

The device is programed with a Tag-Connect [TC2030](https://www.tag-connect.com/product/tc2030-idc-nl) programing header that has the corresponding ICSP contacts on the back of the board along with the needed locating pins.

![render-back]

#### IMU
ST-Microelectronics's [LSM6DSM](https://www.st.com/resource/en/datasheet/lsm6dsm.pdf) available in a small (2.5mmx3mm) is a low power accelerometer and gyroscope with I2C and SPI. It is very low power and also has additional embedded functions that are not being used currently. 

#### LED Array
WorldSemi [WS2812B-2020](http://www.world-semi.com/DownLoadFile/140) RGB LEDs in the 2mmx2.2mm package provide the light. They come in a small package that can glow brightly and are addressable so only a single data line provides data to the whole array in a shift-register fashion.

![render-tip]

#### Power Supply
Microchip's [MIC5365](http://ww1.microchip.com/downloads/en/DeviceDoc/mic5365.pdf) and battery pads are the main power components. The 3.3V LDO linear regulator provides power to the microcontroller and IMU. There is no UVLO so it is recommended your battery has one. With voltage below 3.3V the output voltage corresponds to the input with the ~60mA drop, the board should continue working until 2.7V where the blue diode will cut out and the microcontroller falls out of it's speed grade and may not be able to maintain its clock speed. The two battery pads on the back are marked with plus and minus symbols and can have enameled wires soldered to them to provide power to the board.

![prototype-back]

#### Coating
After programming and testing, four thin coats of [OPI Top Coat](https://www.opi.com/shop-products/nail-care/top-base-coats/opi-top-coat) were added on top to secure the enameled wires and provide encapsulation. 

![prototype-encapsulated]

Then following the lines on the back silkscreen, the board is trimmed to its final dimensions removing the locating holes for the programmer.

![layout-trim]

## Firmware
The main loop is shown below (an excerpt from [main.cpp](./firmware/src/main.cpp)) and the logic is currently very minimal. The sensor is read every 25ms, and its current value is compared to the previous value. The magnitude of difference between those values is mapped (after dividing by 2^6 to reduce sensitivity) to the three colors, one for each axis.

All the LEDs are moved down one spot, and the calculated values are assigned to the top LED. As this repeats, the colors waterfall down the chain until they "fall off" at the end.

```cpp
void loop() {
  imu.readRawAccel();

  // Calculate change, and map to lower sensitivity
  uint8_t red =   abs((imu.accelData.x >> 6) - (prevX >> 6));
  uint8_t green = abs((imu.accelData.y >> 6) - (prevY >> 6));
  uint8_t blue =  abs((imu.accelData.z >> 6) - (prevZ >> 6));

  // Store current reading as new previous data
  prevX = imu.accelData.x;
  prevY = imu.accelData.y;
  prevZ = imu.accelData.z;

  // Shift all LEDs down one spot
  memmove(&leds[1], &leds[0], NUM_LEDS * sizeof(CRGB) - 1);
  // Set color of 0th LED
  leds[0] = CRGB(red, green, blue);
  FastLED.show();
  delay(25);
}
```

On boot there is a pulse of light from the acceleration vector of gravity being compared to the initial all zero values. 

### LSM6DM Driver
The driver is meant to be minimal and fast. Producing only `int16_t` values and no floating-point operations. It can access the accelerometer, gyroscope, and temperature sensor as well as set their max scale and rates.

### Compiling
It is set up to be compile through [PlatformIO](https://platformio.org/). More information can be found through its [doucmentation](https://docs.platformio.org/en/latest/). Changes may be need to the settings in the `ini` file.

### Flashing
An ICSP programmer along with the TagConnect programming header is needed to program the board. A [retaining clip](https://www.tag-connect.com/product/tc2030-clip-retaining-clip-board-for-tc2030-nl-cables) is useful for extended programing sessions.

Recommended programers are the [Atmel-ICE](https://www.microchip.com/DevelopmentTools/ProductDetails/ATATMEL-ICE) or [Adafruit's USBtinyISP](https://www.adafruit.com/product/46). Take care to remove the power target jumper if using the USBTinyISP as the IMU has an absolute max voltage of 4.8V. The board will need external power regardless for programming, so attach a battery.

Uploading using PlatformIO is currently used. Refer to [documentaiton](https://docs.platformio.org/en/latest/). Changes may be needed to the `ini` file settings.

### Fuses
The highest fuse bit in the low fuses needs to be to disable the `CKDIV8` flag. The fuses can either be flashed though PlatformIO as they are set in the `ini` file, or through avrdude as shown below.

```bash
avrdude -p t85 -c atmelice_isp -U lfuse:w:0xE2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
```

### Timing Analysis
An extra GPIO was used toggling around the delay function. This will provide the approximate time the processor needs to be awake. There is a lot of overhead from the Arduino libraries. Running an empty loop takes ~700us (5600 cycles) from the end of the loop to the start of the next. 

![timingCapture]

The values were averaged over 64 samples and will not sum across the measurements due to noise but should be within 0.5%. The frequency value was taken to cross check the high and low time.

| Domain | Value | Unit | Percentage |
| -----  | -----:| ---- | ----------:|
| Full   | 26.33 | ms   |    100.00% |
| Sleep  | 24.67 | ms   |     93.70% |
| Active | 1.746 | ms   |      6.63% |

Based on that rough data, and the current draw values from the datasheet, we can estimate average power consumption.

```
4mA Active * 0.0663 = 0.2652 mA
1mA Idle   * 0.9370 = 0.9370 mA +
                      ------------
                      1.2022 mA
```

This can be significantly improved by switching to C to reduce the active time.

## BOM
The bill of materials is available in [`bom.csv`](bom.csv) and is copied into a table below.

| Ref Des                 | Qty | Manufacturer          | MPN               | Description                           |
| ----- | ---:| --------------------- | ----------------- | ------------------------------------- |
| C1    | 1   | Samsung               | CL05A105KQ5NNNC   | CAP: Ceramic 1uF 6.3V X5R 0402        |
| C2    | 1   | Taiyo Yuden           | JMK105BJ225KV-F   | CAP: Ceramic 2.2uF 6.3V X5R 0402      |
| C3    | 1   | Wuerth Elektronik     | 885012105010      | CAP: Ceramic 0.1uF 10V X5R 0402       |
| D1-D8 | 8   | WorldSemi             | WS2812B-2020      | LED: Addressable RGB (2x2)            |
| R1 R2 | 2   | Stackpole Electronics | RMCF0402JT10K0    | RES: 10K OHM 5% 1/16W 0402            |
| U1    | 1   | Microchip             | MIC5365-3.3YMT-TZ | REG Linear 3.3V 150mA 4TMLF (1x1)     |
| U2    | 1   | Microchip             | ATTINY85-20MU     | MCU: AVR 8b 8KB Flash 20QFN (4x4)     |
| U3    | 1   | STMicroelectronics    | LSM6DSMTR         | IMU: Accel/Gyro I2C/SPI 14LGA (2.5x3) |

A [Octopart](https://octopart.com/bom-tool/vS6z9Vt5) BOM is also available to simplify ordering. 

## Assembly
Assembly is all surface mount with components on one side of the board to make PNP assembly easy. The layout layer duplicated below lists components by their reference designators and values.

![layout-fab]

Solder paste can be applied with 25ga+ syringe, or with a stencil of the top paste layer. The components can then be placed, taking note of those that are polarity dependent, being U1-U3 and D1-D8 as marked in the assembly layer. A standard reflow profile, a hotplate, or a hot air rework station should work equally well. Inspect the side joints of the microcontroller as they will experience a lot of stress when trimming the board.

![layout-trim]

Two pads on the back are to provide power from enameled wire that can be soldered, and then twisted and routed out through the mounting hole on top. Then after final testing the device can be encapsulated.

## PCB Ordering
The board is a two-layer polyimide flex with 6/6mil spacing, plated through vias. These are generally the defaults boards so they can be made by most fabrication houses provided flex is an available option. A set of three was purchased at [Oshpark](http://oshpark.com/) for 2.00 USD and a shared project is available [here](https://oshpark.com/shared_projects/V0E4EFRY).

![prototype-boards]

A full set of gerbers are also available in [`./docs/gerbers`](./docs/gerbers). They have been visually checked although the fabrication of the prototypes was from the KiCad source file.

### Dimensions
The board is rather small so it may need to be panelized for some manufacturers. 

*PCB Dimensions*
|        | Width | Height |
| ------ | ----- | ------ |
| Inches | 1.53  |  0.26  |
| mm     | 38.9  |   6.6  |

Since its tapered, it can be placed in an alternating orientation for better space utilization.

![layout-dim]

Dimensions of board after being trimmed to size.

## To-Do
- [ ] Change delay to sleep
- [ ] Move code to simple makefile
- [ ] Find ideal low power IMU rate
- [ ] Switch to C for performance improvements
- [ ] Add passives (R & C) to project library

<!-- Images -->
[shimmer]:                /docs/shimmer.gif                "Animated view of the shimmer"
[render-front]:           /docs/render-front.jpg           "Ray Traced render of front"
[render-back]:            /docs/render-back.jpg            "Ray Traced render of back"
[render-tip]:             /docs/render-tip.png             "Render of tip"
[render-rays]:            /docs/render-rays.png            "Render of ray motif"
[prototype-boards]:       /docs/prototype-boards.jpg       "Three fabricated unpopulated boards"
[prototype-scale]:        /docs/prototype-scale.jpg        "Assembled prototype with various coins as scale"
[prototype-encapsulated]: /docs/prototype-encapsulated.jpg "Nail polish topcoat encapsulation detail"
[prototype-back]:         /docs/prototype-back.jpg         "Back of assembled prototype boards"
[layout-dim]:             /docs/layout-dim.png             "Final dimensions after trimming"
[layout-fab]:             /docs/layout-fab.png             "Component placement for fabrication"
[layout-trim]:            /docs/layout-trim.png            "Trimming line on back of boards"
[schematic]:              /docs/schematic.png              "Full schematic"
[timingcapture]:          /docs/timingCapture.png          "Timing of"