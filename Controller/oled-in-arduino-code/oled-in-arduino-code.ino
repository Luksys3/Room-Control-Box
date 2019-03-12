/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

/*********************************************************************
I change the adafruit SSD1306 to SH1106

SH1106 driver don't provide several functions such as scroll commands.

*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include "Bitmaps.h"

#define WIDTH 128
#define HEIGHT 64

#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

const String ACTIONS_LIST[] = {
  "Information",
  "Led Strip",
  "Computer",
  "Send command",
  "Volume",
  "Options"
};
#define ACTIONS_NUM (sizeof(ACTIONS_LIST)/sizeof(String))
#define LINE_HEIGHT 9
#define DEFAULT_POINTER_POSITION 0

#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SH1106.h!");
#endif

void setup()   {                
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  setContrast(0);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
  display.clearDisplay();
  
  setScene("actions");
  drawHeader();
  pointerDown();
  
  display.display();
  delay(1000);
}

void setContrast(int c) {
  display.SH1106_command(SH1106_SETCONTRAST);
  display.SH1106_command(c);
  display.SH1106_command(SH1106_DISPLAYOFF);
  display.SH1106_command(SH1106_DISPLAYON);
}

// - Header
void drawHeader() {


  // TX battery
  display.drawBitmap(WIDTH - BATTERY_BMP_WIDTH - TX_BMP_WIDTH - 2, 1 + BATTERY_BMP_HEIGHT,
    tx_bmp, TX_BMP_WIDTH, TX_BMP_HEIGHT, WHITE);
  display.drawBitmap(WIDTH - BATTERY_BMP_WIDTH, BATTERY_BMP_HEIGHT + 1, battery_bmp, BATTERY_BMP_WIDTH, BATTERY_BMP_HEIGHT, WHITE);
  
  // RX battery
  display.drawBitmap(WIDTH - BATTERY_BMP_WIDTH - RX_BMP_WIDTH - 2, 0,
    rx_bmp, RX_BMP_WIDTH, RX_BMP_HEIGHT, WHITE);
  display.drawBitmap(WIDTH - BATTERY_BMP_WIDTH, 0, battery_bmp, BATTERY_BMP_WIDTH, BATTERY_BMP_HEIGHT, WHITE);
}

// - Scene
void setScene(String name) {
  if (name == "actions") {
    int cx = 6;
    int cy = 0;
    for (int i=0; i<ACTIONS_NUM; i++) {
      display.setCursor(cx, cy);
      display.println(ACTIONS_LIST[i]);
      cy += LINE_HEIGHT;
    }
  }
}

// - Pointer
int pointerPos = DEFAULT_POINTER_POSITION;
void updatePointer() {
  if (pointerPos < 0) pointerPos = ACTIONS_NUM - 1;
  if (pointerPos >= ACTIONS_NUM) pointerPos = 0;
  
  int px = 0;
  int py = pointerPos * LINE_HEIGHT;
  display.drawBitmap(px, py, pointer_bmp, POINTER_BMP_WIDTH, POINTER_BMP_HEIGHT, WHITE);
}

void pointerDown() {
  pointerPos++;
  updatePointer();
}

void pointerUp() {
  pointerPos--;
  updatePointer();
}
