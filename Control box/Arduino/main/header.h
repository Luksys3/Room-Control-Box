
const int SERIAL_BAUD_RATE = 9600;
const String SERIAL_MESSAGE_END = "\n";
const String SERIAL_ALIVE_CODE = "7q8w9e";
const int SERIAL_ALIVE_DELAY = 2000; // msec

const int BUTTON_PIN = 2;
const int BUTTON_DEBOUNCE_DELAY = 30; // msec

const int MAX_GPIO_PINS = 20;

struct Button {
	Button() : lastButtonState(LOW), lastRealButtonState(LOW), lastDebounceTime(0) {}
	int lastButtonState;
	int lastRealButtonState;
	unsigned long lastDebounceTime;
};
