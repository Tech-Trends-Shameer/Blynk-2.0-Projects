// Tech Trends Shameer
// Control Water Pump Using Blynk 2.0 with ESP32

#define BLYNK_TEMPLATE_ID "TMPL38yxhS6CJ"
#define BLYNK_TEMPLATE_NAME "Control Water Pump with ESP32"
#define BLYNK_AUTH_TOKEN "bv724Dqw6nBsDZ-LriSgl9oS7yJWnHWQxrOKAsG6IXT"


#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "WiFi Username";  // Enter your WiFi name
char pass[] = "WiFi Password";  // Enter your WiFi password

int relaypin = 2; // Use GPIO26 (D4 equivalent in ESP32)

void setup() {
    Serial.begin(115200);
    Blynk.begin(auth, ssid, pass);
    pinMode(relaypin, OUTPUT);
}

void loop() {
    Blynk.run(); 
}
