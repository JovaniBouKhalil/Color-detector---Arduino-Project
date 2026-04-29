#define RED_LED 9
#define GREEN_LED 10
#define BLUE_LED 11

void setRGBLed(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(RED_LED, r);
  analogWrite(GREEN_LED, g);
  analogWrite(BLUE_LED, b);
}

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
}

void loop() {
  setRGBLed(255, 0, 0);
  delay(1000);

  setRGBLed(0, 255, 0);
  delay(1000);

  setRGBLed(0, 0, 255);
  delay(1000);

  setRGBLed(255, 255, 0);
  delay(1000);

  setRGBLed(0, 0, 0);
  delay(1000);
}
