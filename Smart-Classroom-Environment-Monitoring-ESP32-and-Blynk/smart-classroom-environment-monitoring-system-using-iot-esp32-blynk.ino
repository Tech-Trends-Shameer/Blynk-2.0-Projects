#define BLYNK_TEMPLATE_ID "TMPL3INnbHHJk"
#define BLYNK_TEMPLATE_NAME "Smart Classroom Environmental Monitoring System"
#define BLYNK_AUTH_TOKEN "sHCRhTs9d5h9Z6hrlfsiLSqWnVEggmEBnmHbjr"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// Blynk Credentials  
char ssid[] = "WiFi Username";    
char pass[] = "WiFi Password"; 

// Sensor Pins
#define DHTPIN 4         // DHT11 sensor data pin
#define DHTTYPE DHT11
#define MQ135_PIN 34     // Analog pin for MQ135 sensor
#define LDR_PIN 35       // Analog pin for LDR sensor
#define RELAY_PIN 5      // GPIO controlling relay

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    
    // Initialize Blynk
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
    
    // Initialize DHT Sensor
    dht.begin();
    
    // Set relay pin as output
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW); // Ensure solenoid is OFF initially
    
    Serial.println("System Initialized");
}

void loop() {
    Blynk.run(); // Run Blynk

    float temperature = dht.readTemperature(); // Read temperature in Celsius
    float humidity = dht.readHumidity();       // Read humidity
    int airQuality = analogRead(MQ135_PIN);    // Read MQ135 sensor value
    int lightLevel = analogRead(LDR_PIN);      // Read LDR value

    Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" °C");
    Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
    Serial.print("Air Quality: "); Serial.println(airQuality);
    Serial.print("Light Level: "); Serial.println(lightLevel);

    // Send data to Blynk
    Blynk.virtualWrite(V0, temperature);
    Blynk.virtualWrite(V1, humidity);
    Blynk.virtualWrite(V2, airQuality);
    Blynk.virtualWrite(V3, lightLevel);

    // Control Solenoid Valve
    if (temperature > 32) {
        digitalWrite(RELAY_PIN, HIGH); // Turn ON Solenoid
        Serial.println("Solenoid Activated!");
        Blynk.virtualWrite(V4, "ON"); // Update Blynk Status
    } else {
        digitalWrite(RELAY_PIN, LOW);  // Turn OFF Solenoid
        Serial.println("Solenoid Deactivated!");
        Blynk.virtualWrite(V4, "OFF"); // Update Blynk Status
    }

    delay(2000); // Wait 2 seconds before next reading
}

