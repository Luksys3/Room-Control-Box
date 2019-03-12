#include <Keypad.h>
#include <SPI.h>

#include "RF24.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include "Bitmaps.h"

#include "SdFat.h"

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

const byte ROWS = 5;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', '4'},
  {'5', '6', '7', '8'},
  {'9', '0', 'A', 'B'},
  {'C', 'D', 'E', 'F'},
  {'I', 'G', 'H', 'J'}
};

byte rowPins[ROWS] = {3, 5, 7, 9, 11};
byte colPins[COLS] = {13, 15, 17, 19};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

SdFat SD;
#define SD_CS_PIN 53
File myFile;

#define RADIO_CSN_PIN 49
RF24 radio(48, RADIO_CSN_PIN);
byte addresses[][6] = {"1Node", "2Node"};


void setup() {
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  setContrast(0);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  pinMode(RADIO_CSN_PIN, OUTPUT);
  pinMode(SD_CS_PIN, OUTPUT);

  readCard();

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
}

void loop() {
  char customKey = customKeypad.getKey();

  display.clearDisplay();

  setScene("actions");
  drawHeader();

  if (customKey) {
    pointerDown();
    Serial.println(customKey);
    
    readCard();

    if (!radio.write( &customKey, sizeof(customKey) )) {
      Serial.println(F("failed"));
    }
  }

  display.display();
}

void setContrast(int c) {
  display.sh1106_command(SH1106_SETCONTRAST);
  display.sh1106_command(c);
  display.sh1106_command(SH1106_DISPLAYOFF);
  display.sh1106_command(SH1106_DISPLAYON);
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
    for (int i = 0; i < ACTIONS_NUM; i++) {
      display.setCursor(cx, cy);
      display.println(ACTIONS_LIST[i]);
      cy += LINE_HEIGHT;
    }
    updatePointer();
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
}

void pointerUp() {
  pointerPos--;
}

void readCard() {
  digitalWrite(RADIO_CSN_PIN, HIGH); // disable radio


  Serial.print("Initializing SD card...");

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  //
  //  if (!card.init(SPI_HALF_SPEED, 4)) {
  //    Serial.println("initialization failed. Things to check:");
  //    Serial.println("* is a card inserted?");
  //    Serial.println("* is your wiring correct?");
  //    Serial.println("* did you change the chipSelect pin to match your shield or module?");
  //    return;
  //  } else {
  //    Serial.println("Wiring is correct and a card is present.");
  //  }
  //
  //
  //
  //
  //  // print the type of card
  //  Serial.print("\nCard type: ");
  //  switch (card.type()) {
  //    case SD_CARD_TYPE_SD1:
  //      Serial.println("SD1");
  //      break;
  //    case SD_CARD_TYPE_SD2:
  //      Serial.println("SD2");
  //      break;
  //    case SD_CARD_TYPE_SDHC:
  //      Serial.println("SDHC");
  //      break;
  //    default:
  //      Serial.println("Unknown");
  //  }
  //
  //  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  //  if (!volume.init(card)) {
  //    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
  //    return;
  //  }
  //
  //
  //  // print the type and size of the first FAT-type volume
  //  uint32_t volumesize;
  //  Serial.print("\nVolume type is FAT");
  //  Serial.println(volume.fatType(), DEC);
  //  Serial.println();
  //
  //  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  //  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  //  volumesize *= 512;                            // SD card blocks are always 512 bytes
  //  Serial.print("Volume size (bytes): ");
  //  Serial.println(volumesize);
  //  Serial.print("Volume size (Kbytes): ");
  //  volumesize /= 1024;
  //  Serial.println(volumesize);
  //  Serial.print("Volume size (Mbytes): ");
  //  volumesize /= 1024;
  //  Serial.println(volumesize);
  //
  //
  //  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  //  root.openRoot(volume);
  //
  //  // list all files in the card with date and size
  //  root.ls(LS_R | LS_DATE | LS_SIZE);

  digitalWrite(SD_CS_PIN, HIGH); // disable sd-kortet


  digitalWrite(RADIO_CSN_PIN, LOW); // enable radio
}
