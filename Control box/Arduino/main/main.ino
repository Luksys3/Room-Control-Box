#include "header.h";

int lastButtonState = LOW;
int lastRealButtonState = LOW;
unsigned long lastTimeSentAlive = 0;
unsigned long lastDebounceTime = 0;

Button buttons[MAX_GPIO_PINS + 1];

void setup() {
	Serial.begin(9600);

	pinMode(BUTTON_PIN, INPUT);
}

void loop() {
	if (is_pressed(BUTTON_PIN))
		serial_write("BUTTON PRESSED");

	if ((millis() - lastTimeSentAlive) > SERIAL_ALIVE_DELAY) {
		lastTimeSentAlive = millis();
		serial_write(SERIAL_ALIVE_CODE);
	}
}

bool is_pressed(int pin) {
	int pressed = false;
	int buttonState = digitalRead(pin);

	if (buttonState != buttons[pin].lastButtonState)
		lastDebounceTime = millis();

	if ((millis() - lastDebounceTime) > BUTTON_DEBOUNCE_DELAY) {
		if (buttonState == HIGH && buttons[pin].lastRealButtonState == LOW)
			pressed = true;

		buttons[pin].lastRealButtonState = buttonState;
	}

	buttons[pin].lastButtonState = buttonState;

	return pressed;
}

void serial_write(String message) {
	Serial.print(message + SERIAL_MESSAGE_END);
}

String serial_read() {
	if (Serial.available() <= 0)
		return "";

	return Serial.readString();
}

bool serial_is_empty() {
	return Serial.available() <= 0
			? true
			: false;
}
