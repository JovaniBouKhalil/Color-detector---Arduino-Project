#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define OUT_PIN 6

#define RED_LED 9
#define GREEN_LED 10
#define BLUE_LED 11

#define BUTTON_PIN A0

int R_black = 1149;
int G_black = 1388;
int B_black = 1533;

int R_white = 8196;
int G_white = 9615;
int B_white = 10638;

uint8_t normalizeChannel(int raw, int black, int white) {
  if (white == black) return 0;

  float norm = (float)(raw - black) / (float)(white - black);

  if (norm < 0) norm = 0;
  if (norm > 1) norm = 1;

  return (uint8_t)(norm * 255);
}

float normalizeChannelFloat(float raw, int black, int white) {
  if (white == black) return 0.0f;

  float norm = (raw - black) / (float)(white - black);

  if (norm < 0.0f) norm = 0.0f;
  if (norm > 1.0f) norm = 1.0f;

  return norm;
}

int clamp255(float x) {
  int v = (int)(x + 0.5f);

  if (v < 0) v = 0;
  if (v > 255) v = 255;

  return v;
}

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

void noambientNormalize(uint8_t Rc, uint8_t Gc, uint8_t Bc,
                        float &r, float &g, float &b) {

  float Rp = (float)Rc;
  float Gp = (float)Gc;
  float Bp = (float)Bc;

  float sum = Rp + Gp + Bp;

  if (sum <= 0.0f) {
    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
    return;
  }

  r = Rp / sum;
  g = Gp / sum;
  b = Bp / sum;
}

void computeBetterRGB(float nR, float nG, float nB,
                      uint8_t &R_out, uint8_t &G_out, uint8_t &B_out) {

  float sum = nR + nG + nB;

  if (sum < 0.015f) {
    R_out = 0;
    G_out = 0;
    B_out = 0;
    return;
  }

  float r = nR / sum;
  float g = nG / sum;
  float b = nB / sum;

  float brightness = sum / 3.0f;

  float maxRatio = max(r, max(g, b));
  float minRatio = min(r, min(g, b));
  float dominance = maxRatio - minRatio;

  uint8_t R_linear = clamp255(nR * 255.0f);
  uint8_t G_linear = clamp255(nG * 255.0f);
  uint8_t B_linear = clamp255(nB * 255.0f);

  uint8_t R_ratio = clamp255(r * 255.0f);
  uint8_t G_ratio = clamp255(g * 255.0f);
  uint8_t B_ratio = clamp255(b * 255.0f);

  if (brightness < 0.22f && dominance > 0.12f) {
    float lift = 0.55f;

    R_out = clamp255((1.0f - lift) * R_linear + lift * R_ratio);
    G_out = clamp255((1.0f - lift) * G_linear + lift * G_ratio);
    B_out = clamp255((1.0f - lift) * B_linear + lift * B_ratio);
    return;
  }

  if (brightness < 0.45f && dominance > 0.12f) {
    float blend = (0.45f - brightness) / (0.45f - 0.22f);

    if (blend < 0.0f) blend = 0.0f;
    if (blend > 1.0f) blend = 1.0f;

    blend = blend * 0.45f;

    R_out = clamp255((1.0f - blend) * R_linear + blend * R_ratio);
    G_out = clamp255((1.0f - blend) * G_linear + blend * G_ratio);
    B_out = clamp255((1.0f - blend) * B_linear + blend * B_ratio);
    return;
  }

  R_out = R_linear;
  G_out = G_linear;
  B_out = B_linear;
}

void setRGBLed(uint8_t r, uint8_t g, uint8_t b) {

  uint8_t mx = max(r, max(g, b));
  uint8_t mn = min(r, min(g, b));

  float cr = (r == mx) ? 1.0 : (r == mn) ? 0.2 : 0.5;
  float cg = (g == mx) ? 1.0 : (g == mn) ? 0.2 : 0.5;
  float cb = (b == mx) ? 1.0 : (b == mn) ? 0.2 : 0.5;

  analogWrite(RED_LED,   (uint8_t)(cr * r));
  analogWrite(GREEN_LED, (uint8_t)(cg * g));
  analogWrite(BLUE_LED,  (uint8_t)(cb * b));
}

String getColorName(uint8_t r, uint8_t g, uint8_t b) {
  uint8_t mx = max(r, max(g, b));
  uint8_t mn = min(r, min(g, b));

  if (mx < 25) return "Black";
  if (mn > 180 && (mx - mn) < 45) return "White";

  if ((mx - mn) < 25) return "Gray";

  if (r > 200 && g > 180 && b < 170 && abs((int)r - (int)g) < 90) {
    return "Yellow";
  }

  if (r > 90 && b > 50 && g < r && b > g * 0.7) {
    return "Pink";
  }

  if (r >= g && r >= b) return "Red";
  if (g >= r && g >= b) return "Green";
  if (b >= r && b >= g) return "Blue";

  return "Unknown";
}

void setup() {
  Serial.begin(9600);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT_PIN, INPUT);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ready...");
  lcd.setCursor(0, 1);
  lcd.print("Press button");

  setRGBLed(0, 0, 0);
}

void loop() {
  static bool lastState = HIGH;
  bool currentState = digitalRead(BUTTON_PIN);

  if (lastState == HIGH && currentState == LOW) {
    delay(50);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Scanning...");

    float R, G, B;
    readRawRGB(R, G, B);

    uint8_t outR = normalizeChannel((int)R, R_black, R_white);
    uint8_t outG = normalizeChannel((int)G, G_black, G_white);
    uint8_t outB = normalizeChannel((int)B, B_black, B_white);

    float nR = normalizeChannelFloat(R, R_black, R_white);
    float nG = normalizeChannelFloat(G, G_black, G_white);
    float nB = normalizeChannelFloat(B, B_black, B_white);

    float r, g, b;
    noambientNormalize(outR, outG, outB, r, g, b);

    uint8_t normR = (uint8_t)(r * 255.0f);
    uint8_t normG = (uint8_t)(g * 255.0f);
    uint8_t normB = (uint8_t)(b * 255.0f);

    uint8_t finalR, finalG, finalB;
    computeBetterRGB(nR, nG, nB, finalR, finalG, finalB);

    setRGBLed(finalR, finalG, finalB);

    String colorName = getColorName(finalR, finalG, finalB);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Color:");
    lcd.print(colorName);

    lcd.setCursor(0, 1);
    lcd.print("RGB:");
    lcd.print(finalR);
    lcd.print(" ");
    lcd.print(finalG);
    lcd.print(" ");
    lcd.print(finalB);

    while (digitalRead(BUTTON_PIN) == LOW) {
      delay(10);
    }
  }

  lastState = currentState;
}