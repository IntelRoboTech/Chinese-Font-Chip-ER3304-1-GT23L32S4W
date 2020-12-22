# Chinese-Font-Chip-ER3304-1-GT23L32S4W
Demo for: Chip Font Library (includes Chinese Character Set) GB2312, ASCII - Interfacing with OLED 12864 Display using Arduino Platform

In this demo, I used GT23L32S4W Font Chip from Genitop (Datasheet can be found in the repo) to show Chinese characters on 0.96" OLED 12864 Display. Font Chip use SPI protocol for communication with Arduino UNO and the OLED use I2C protocol.
Font chip includes Chinese Character Set GB2312 as well as ASCII Character set, for both the character sets different font sizes can be chosen.
Below are the base addresses for the font sets:

Chinese character set in GT23L32S4W font chip includes 6763+846 characters.
/************** GB2312*****************/
#define BaseAdd_12X12  0x00000
#define BaseAdd_16X16 0x2C9D0
#define BaseAdd_24X24 0x68190
#define BaseAdd_32X32 0xEDF00

ASCII character set in GT23L32S4W font chip includes 96 characters only.
/************** ASCII*****************/
#define ASCIIAdd_5X7  0x1DDF80
#define ASCIIAdd_7X8  0x1DE280
#define ASCIIAdd_6X12 0x1DBE00
#define ASCIIAdd_8X16 0x1DD780
#define ASCIIAdd_12X24  0x1DFF00
#define ASCIIAdd_16X32  0x1E5A50
