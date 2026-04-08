/*
  =====================================================
  IoT Noise Pollution Monitor — LCD I2C Version
  Hardware : ESP32 + KY-038 + 16x2 LCD I2C + Blynk
  =====================================================
*/

#define BLYNK_TEMPLATE_ID "Template ID"
#define BLYNK_TEMPLATE_NAME "Noise Pollution Monitoring"
#define BLYNK_AUTH_TOKEN "Auth Token"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>   // ← replaces Adafruit SSD1306

// ─── WiFi Credentials ────────────────────────────────

const char* ssid     = "WiFi Username";
const char* password = "WiFi Password";

// ─── LCD Setup ───────────────────────────────────────
// Try 0x27 first; if blank, change to 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ─── Pin Definitions ─────────────────────────────────
#define SOUND_ANALOG_PIN  34
#define LED_GREEN         25
#define LED_YELLOW        26
#define LED_RED           27

// ─── dB Calibration ──────────────────────────────────
#define DB_MIN  30
#define DB_MAX  120
#define ADC_MIN 0
#define ADC_MAX 4095

// ─── Timing ──────────────────────────────────────────
BlynkTimer timer;
const int SAMPLE_WINDOW = 50;  // ms

// ─────────────────────────────────────────────────────
float adcToDecibels(int raw) {
  float db = map(raw, ADC_MIN, ADC_MAX, DB_MIN, DB_MAX);
  return constrain(db, DB_MIN, DB_MAX);
}

// ─────────────────────────────────────────────────────
String getStatus(float db) {
  if (db < 60)  return "SAFE    ";
  if (db < 70)  return "MODERATE";
  if (db < 80)  return "LOUD    ";
  if (db < 90)  return "VERY LUD";   // 8 chars max for LCD
  return "DANGER! ";
}

// ─────────────────────────────────────────────────────
void updateLEDs(float db) {
  digitalWrite(LED_GREEN,  db < 60  ? HIGH : LOW);
  digitalWrite(LED_YELLOW, (db >= 60 && db < 80) ? HIGH : LOW);
  digitalWrite(LED_RED,    db >= 80 ? HIGH : LOW);
}

// ─────────────────────────────────────────────────────
void updateLCD(float db, String status) {
  // Line 1:  "Noise: 072 dB   "
  lcd.setCursor(0, 0);
  lcd.print("Noise:");
  lcd.print((int)db < 10 ? "  " : (int)db < 100 ? " " : "");
  lcd.print((int)db);
  lcd.print(" dB     ");

  // Line 2:  "Status:MODERATE "
  lcd.setCursor(0, 1);
  lcd.print("St:");
  lcd.print(status);  // already padded to 8 chars
}

// ─────────────────────────────────────────────────────
void measureAndSend() {
  int signalMax = 0, signalMin = 4095;
  unsigned long start = millis();

  while (millis() - start < SAMPLE_WINDOW) {
    int s = analogRead(SOUND_ANALOG_PIN);
    if (s > signalMax) signalMax = s;
    if (s < signalMin) signalMin = s;
  }

  int   peakToPeak = signalMax - signalMin;
  float db         = adcToDecibels(peakToPeak);
  String status    = getStatus(db);

  updateLEDs(db);
  updateLCD(db, status);

  Blynk.virtualWrite(V0, (int)db);
  Blynk.virtualWrite(V1, status);
  Blynk.virtualWrite(V2, peakToPeak);

  Serial.printf("ADC P2P: %d  |  dB: %.1f  |  %s\n",
                peakToPeak, db, status.c_str());
}

// ─────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  pinMode(LED_GREEN,  OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED,    OUTPUT);

  analogReadResolution(12);

  // LCD init
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Noise Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Connecting...");

  // Connect Blynk + WiFi
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected!");
  delay(1500);
  lcd.clear();

  timer.setInterval(1000L, measureAndSend);
}

void loop() {
  Blynk.run();
  timer.run();
}
