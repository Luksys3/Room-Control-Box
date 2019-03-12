
// POINTER
#define POINTER_BMP_WIDTH  3
#define POINTER_BMP_HEIGHT 7
static const unsigned char PROGMEM pointer_bmp[] =
{
	B00000000,
	B10000000,
	B11000000,
	B11100000,
	B11000000,
	B10000000,
	B00000000,
};

// BATERRY
#define BATTERY_BMP_WIDTH  10
#define BATTERY_BMP_HEIGHT 5
static const unsigned char PROGMEM battery_bmp[] =
{
	B01111111, B11000000,
	B11000111, B11000000,
	B11000111, B11000000,
	B11000111, B11000000,
	B01111111, B11000000,
};

// RX
#define RX_BMP_WIDTH  7
#define RX_BMP_HEIGHT 5
static const unsigned char PROGMEM rx_bmp[] =
{
	B11001010,
	B10101010,
	B11000100,
	B10101010,
	B10101010,
};

// TX
#define TX_BMP_WIDTH  7
#define TX_BMP_HEIGHT 5
static const unsigned char PROGMEM tx_bmp[] =
{
	B11101010,
	B01001010,
	B01000100,
	B01001010,
	B01001010,
};
