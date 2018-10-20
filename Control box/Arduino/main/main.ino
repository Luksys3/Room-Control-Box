#include "header.h";

int buttonState;
int lastButtonState = LOW;
int lastRealButtonState = LOW;

unsigned long lastTimeSentAlive = 0;
unsigned long lastDebounceTime = 0;

void setup() {
	Serial.begin(9600);

	pinMode(BUTTON_PIN, INPUT);
}


void loop() {

	// == Button stuff
	int reading = digitalRead(BUTTON_PIN);

	if (reading != lastButtonState)
	lastDebounceTime = millis();

	if ((millis() - lastDebounceTime) > BUTTON_DEBOUNCE_DELAY) {
		buttonState = reading;


		if (buttonState == HIGH && lastRealButtonState == LOW)
		serial_write("launch_chrome");

		lastRealButtonState = buttonState;
	}

	lastButtonState = reading;
	// == #Button stuff


	if ((millis() - lastTimeSentAlive) > SERIAL_ALIVE_DELAY) {
		lastTimeSentAlive = millis();
		serial_write(SERIAL_ALIVE_CODE);
	}

}

void serial_write(String message) {
	Serial.print(message + SERIAL_MESSAGE_END);
}
