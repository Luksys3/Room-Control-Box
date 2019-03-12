
#define AWAKE_PIN 2

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(AWAKE_PIN, OUTPUT);

  digitalWrite(AWAKE_PIN, HIGH);
}

void loop() {

  digitalWrite(13, HIGH);
  delay(5000);
  digitalWrite(13, LOW);


  digitalWrite(AWAKE_PIN, LOW);

}
