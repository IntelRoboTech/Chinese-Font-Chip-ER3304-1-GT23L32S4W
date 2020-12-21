#include <SPI.h>
#include "hz.c"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   12
#define LOGO_WIDTH    12

/************** GB2312*****************/
#define BaseAdd_12X12  0x00000
#define BaseAdd_16X16 0x2C9D0
#define BaseAdd_24X24 0x68190
#define BaseAdd_32X32 0xEDF00
/************** ASCII*****************/
#define ASCIIAdd_5X7  0x1DDF80
#define ASCIIAdd_7X8  0x1DE280
#define ASCIIAdd_6X12 0x1DBE00
#define ASCIIAdd_8X16 0x1DD780
#define ASCIIAdd_12X24  0x1DFF00
#define ASCIIAdd_16X32  0x1E5A50

void GenitopZk_Address(uint32_t Address);
void ASCII_8X16(const char *ASCIICode, uint8_t *BUF);
void ASCII_16X32(const char *ASCIICode, uint8_t *BUF);
void GB2312_12X12(const char *GBCode, uint8_t *BUF);
void GB2312_24X24(const char *GBCode, uint8_t *BUF);
void GB2312_32X32(const char *GBCode, uint8_t *BUF);

const unsigned char PROGMEM logo_bmp[] =
{ 0x0, 0x0, 0x38, 0x0, 0x38, 0x0, 0x6C, 0x0, 0x6C, 0x0, 0x6C, 0x0, 0xC6, 0x0, 0xC6, 0x0, 0xC6, 0x0, 0x83, 0x0, 0x0, 0xA, 0x0, 0x0
 };

uint8_t dat[72]={ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
// using two incompatible SPI devices, A and B. Incompatible means that they need different SPI_MODE
const int slaveAPin = PA4;

// set up the speed, data order and data mode
SPISettings settingsA(8000000, MSBFIRST, SPI_MODE0);
uint8_t val[20];

void GenitopZk_Address(uint32_t Address) 
{
  uint8_t AddH, AddM, AddL;
  
  AddH = Address >> 16;
  AddM = Address >> 8;
  AddL = Address;
  
  SPI.transfer(0x03);
  SPI.transfer(AddH);
  SPI.transfer(AddM);
  SPI.transfer(AddL);

//  SPI.transfer(0x0B);
//  SPI.transfer(AddH);
//  SPI.transfer(AddM);
//  SPI.transfer(AddL);
//  SPI.transfer(0xFF);
}

void ASCII_8X16(const char *ASCIICode, uint8_t *BUF)
{
    uint8_t i;
    uint32_t BaseAdd;
    uint32_t Address;
  
    BaseAdd = ASCIIAdd_8X16;
  
    if(*ASCIICode >= 0x20 && *ASCIICode <= 0x7E){
        Address = (*ASCIICode - 0x20) * 16 + BaseAdd;
    }

    digitalWrite(slaveAPin, LOW);
    Serial.println(Address,HEX);
    GenitopZk_Address(Address);
    for(i = 0; i < 16 ; i++){
      BUF[i] = SPI.transfer(0xFF); 
      Serial.print(BUF[i],HEX);
      Serial.print(" ");
    }
    Serial.println();
    digitalWrite(slaveAPin, HIGH);
}

void ASCII_16X32(const char *ASCIICode, uint8_t *BUF)
{
    uint8_t i;
    uint32_t BaseAdd;
    uint32_t Address;
  
    BaseAdd = ASCIIAdd_16X32;
  
    if(*ASCIICode >= 0x20 && *ASCIICode <= 0x7E){
      Address = (*ASCIICode - 0x20) * 64 + BaseAdd;
    }

    digitalWrite(slaveAPin, LOW);
    Serial.println(Address,HEX);
    GenitopZk_Address(Address);
    for(i = 0; i < 64 ; i++){
      BUF[i] = SPI.transfer(0xFF);
      Serial.print(BUF[i],HEX);
      Serial.print(" ");
    }
    Serial.println();
    digitalWrite(slaveAPin, HIGH);
}

void GB2312_12X12(const char *GBCode, uint8_t *BUF)
{
    uint8_t i;
    uint8_t MSB;      
    uint8_t LSB;      
    uint32_t BaseAdd; 
    uint32_t Address;  
    
    BaseAdd = BaseAdd_12X12;
    
    MSB = *GBCode;
    LSB = *(++GBCode);
  
    Serial.print("MSB: ");
    Serial.print(MSB,HEX);
    Serial.print(",  LSB: ");
    Serial.println(LSB,HEX);
  
    if(MSB >= 0xA1 && MSB <= 0xA9 && LSB >= 0xA1){
      Address = ((MSB - 0xA1) * 94 + (LSB - 0xA1)) * 24 + BaseAdd;
    }
    else if(MSB >= 0xB0 && MSB <= 0xF7 && LSB >= 0xA1){
      Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1) + 846) * 24 + BaseAdd;
    }
  
    digitalWrite(slaveAPin, LOW);
    Serial.println(Address,HEX);
    GenitopZk_Address(Address);
    for(i = 0; i < 24; i++){
      BUF[i] = SPI.transfer(0xFF);
      Serial.print("0x");
      Serial.print(BUF[i],HEX);
      Serial.print(", ");
  }
  Serial.println();
  digitalWrite(slaveAPin, HIGH);
}

void GB2312_24X24(const char *GBCode, uint8_t *BUF)
{
  uint8_t i;
  uint8_t MSB;      
  uint8_t LSB;      
  uint32_t BaseAdd; 
  uint32_t Address;

  BaseAdd = BaseAdd_24X24;
  
  MSB = *GBCode;
  LSB = *(++GBCode);

  Serial.print("MSB: ");
  Serial.print(MSB,HEX);
  Serial.print(",  LSB: ");
  Serial.println(LSB,HEX);

  if(MSB >= 0xA1 && MSB <= 0xA9 && LSB >= 0xA1){
    Address = ((MSB - 0xA1) * 94 + (LSB - 0xA1)) * 72 + BaseAdd;
  }
  else if(MSB >= 0xB0 && MSB <= 0xF7 && LSB >= 0xA1){
    Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1) + 846) * 72 + BaseAdd;
  }
  //Address=0x1A298;
  digitalWrite(slaveAPin, LOW);
  Serial.println(Address,HEX);
  GenitopZk_Address(Address);
  for(i = 0; i < 72; i++){
    BUF[i] = SPI.transfer(0xFF);
    Serial.print("0x");
    Serial.print(BUF[i],HEX);
    Serial.print(", ");
  }
  Serial.println();
  digitalWrite(slaveAPin, HIGH);
}

void GB2312_32X32(const char *GBCode, uint8_t *BUF)
{
  uint8_t i;
  uint8_t MSB;      
  uint8_t LSB;      
  uint32_t BaseAdd; 
  uint32_t Address;

  BaseAdd = BaseAdd_32X32;
  
  MSB = *GBCode;
  LSB = *(++GBCode);

  if(MSB >= 0xA1 && MSB <= 0xA9 && LSB >= 0xA1){
    Address = ((MSB - 0xA1) * 94 + (LSB - 0xA1)) * 128 + BaseAdd;
  }
  else if(MSB >= 0xB0 && MSB <= 0xF7 && LSB >= 0xA1){
    Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1) + 846) * 128 + BaseAdd;
  }

    digitalWrite(slaveAPin, LOW);
    Serial.println(Address,HEX);
  GenitopZk_Address(Address);
  for(i = 0; i < 128; i++){
    BUF[i] = SPI.transfer(0xFF);
        Serial.print(BUF[i],HEX);
        Serial.print(" ");
  }
    Serial.println();
    digitalWrite(slaveAPin, HIGH);
}

void testdrawbitmap(uint8_t *BUF) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}


void drawchar24x24(int i, int j, uint8_t *data){
  display.clearDisplay();
  int b=0;
  j=0;
  for (b=0; b<72; b=b+3){
    i=0;
    for (int c=7;c>=0;c--){
        if( bitRead(data[b], c)){
          display.drawPixel(i,j,SSD1306_WHITE);
        }
        else{
          display.drawPixel(i,j,SSD1306_BLACK);
        }

        if( bitRead(data[b+1], c)){
          display.drawPixel(i+8,j,SSD1306_WHITE);
        }
        else{
          display.drawPixel(i+8,j,SSD1306_BLACK);
        }

        if( bitRead(data[b+2], c)){
          display.drawPixel(i+16,j,SSD1306_WHITE);
        }
        else{
          display.drawPixel(i+16,j,SSD1306_BLACK); 
        }
        i++;        
    }
    i=0;
    j++;
  }
  display.display();  
  delay(1000);
}

void drawchar12x12(int i, int j, uint8_t *data){
  display.clearDisplay();
  int b=0;
  j=0;
  for (b=0; b<24; b=b+2){
    i=0;
    for (int c=7;c>=0;c--){
        if( bitRead(data[b], c)){
          display.drawPixel(i,j,SSD1306_WHITE);
        }
        else{
          display.drawPixel(i,j,SSD1306_BLACK);
        }

        if( bitRead(data[b+1], c)){
          display.drawPixel(i+8,j,SSD1306_WHITE);
        }
        else{
          display.drawPixel(i+8,j,SSD1306_BLACK);
        }
        i++;        
    }
    i=0;
    j++;
  }
  display.display();
  delay(1000);
}

void setup() {
  Serial.begin(9600);
  // set the Slave Select Pins as outputs:
  pinMode (slaveAPin, OUTPUT);
  // initialize SPI:
  SPI.begin();
  
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

    // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  
  Serial.print("Hello!\n");
}

void loop() {
  SPI.beginTransaction(settingsA);
   
  uint8_t data[72];
  GB2312_24X24(hz[0], data);
  drawchar24x24(0, 0, data);
  delay(50);
  GB2312_24X24(hz[1], data);
  drawchar24x24(0, 0, data);
  delay(50);
  GB2312_24X24(hz[2], data);
  drawchar24x24(0, 0, data);
  delay(50);
}
