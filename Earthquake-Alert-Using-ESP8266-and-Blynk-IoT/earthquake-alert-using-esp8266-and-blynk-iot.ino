#define BLYNK_TEMPLATE_ID "TMPL3yi3ExBBc"
#define BLYNK_TEMPLATE_NAME "Earthquake Alert"
#define BLYNK_AUTH_TOKEN "cVb7CdsfhJLhnwIBihb6csfehpjfjl1QkllgPsddEONi"

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
 
// Blynk settings
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "WiFi Username";
char pass[] = "WiFi Password";

// Vibration sensor settings
const int vibrationPin = A0; // Use the appropriate analog pin on your ESP8266
const int threshold = 500;  // Adjust this threshold based on your sensor and environment

// Blynk Virtual Pins
#define VIRTUAL_PIN_VIBRATION  V1 

// Timer settings
BlynkTimer timer;

// Function prototypes
void checkVibration(); 

void setup() {
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);

  timer.setInterval(1000, checkVibration);
}

void loop() {
  Blynk.run();
  timer.run();
}

void checkVibration() {
  int vibrationValue = analogRead(vibrationPin);

  Blynk.virtualWrite(VIRTUAL_PIN_VIBRATION, vibrationValue);

  if (vibrationValue > threshold) {
    Serial.println("Vibration detected!");
    Blynk.logEvent("earthquake_alert","Earthquake Detected!");
    
  }
}
 
