//Tech Trends Shameer
//Control Bulb and Pump using Blynk IOT

#define BLYNK_TEMPLATE_ID "TEMPLATEID"
#define BLYNK_DEVICE_NAME "Control Bulb Pump Using Blynk IOT"
#define BLYNK_AUTH_TOKEN "AUTHTOKEN"


#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
 
#include <BlynkSimpleEsp8266.h>
 

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "WIFI Username";  // type your wifi name
char pass[] = "WIFI Password";  // type your wifi password

int relaypin = D4;
int pump = D2;
void setup()
{     
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);    
  pinMode(relaypin,OUTPUT);
  pinMode(pump,OUTPUT);
 
  }

void loop()
{
  Blynk.run(); 
 }
