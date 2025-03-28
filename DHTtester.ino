
// SET THE STANDARD LIBRARY FIRST
// USE ARDUINO IDE TO RUN THIS CODE 

#include <Wire.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

// #define BLYNK_TEMPLATE_ID "TMPL6Q_pqhTUW"
// #define BLYNK_TEMPLATE_NAME "weather "
//#define BLYNK_AUTH_TOKEN "a_GRTXedh9g1mOyVSI6qe9j0INvZa3GX"

#define DHTPIN 5 
#define Rain 34    
#define LDR 4

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)


DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
// Enter your Auth token
char auth[] = "1JEtRd0EaKZfvyBJFeIj_LniKKIgTdPM";

//Enter your WIFI SSID and password
char ssid[] = "junaid";
char pass[] = "23456789";

void setup() {
  Serial.begin(9600);
  Serial.println(F("Welcome to IoT based weather reportiing system :-) "));
  Serial.printf("Connecting to %s ", ssid);

 
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 8080);
  dht.begin();
  pinMode(Rain, INPUT);
}

void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, t+.01);
  Blynk.virtualWrite(V2, h);

}

void rainSensor() {
  int Rvalue = analogRead(Rain);
  Rvalue = map(Rvalue, 0, 4095, 0, 100);
  Rvalue = (Rvalue - 100) * -1;
  Blynk.virtualWrite(V5, Rvalue);

  Serial.println(Rvalue);
}


void loop() {
  delay(1000);

  
  float h = dht.readHumidity();
  
  float t = dht.readTemperature();
  
  float f = dht.readTemperature(true);

  
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Blynk.virtualWrite(V1, f);

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  Blynk.virtualWrite(V4, hif);
  
  float hic = dht.computeHeatIndex(t, h, false);
  Blynk.virtualWrite(V3, hic);


  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  DHT11sensor();
  rainSensor();
  Blynk.run();
}
