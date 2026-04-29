#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define OUT_PIN 6

float readFrequency() {
  unsigned long tLow = pulseIn(OUT_PIN, LOW);
  unsigned long tHigh = pulseIn(OUT_PIN, HIGH);

  if (tLow == 0 || tHigh == 0) return 0;

  float period = tLow + tHigh;
  return 1000000.0 / period;
}

void readRawRGB(float &R, float &G, float &B) {
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  delay(50);
  R = readFrequency();

  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  delay(50);
  G = readFrequency();

  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  delay(50);
  B = readFrequency();
}

void setup() {
  Serial.begin(9600);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT_PIN, INPUT);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
}

void loop() {
  float R, G, B;
  readRawRGB(R, G, B);

  Serial.print("R: ");
  Serial.print(R);
  Serial.print(" G: ");
  Serial.print(G);
  Serial.print(" B: ");
  Serial.println(B);

  delay(1000);
}