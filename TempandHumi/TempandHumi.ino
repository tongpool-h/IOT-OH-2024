#include "DHTesp.h"

int led1_PIN = 4;
int led2_PIN = 2;

int dht_PIN = 23;

DHTesp dhtSensor; //dhtSensor = new DHTesp()

void setup() {
  //Setup code here!
  Serial.begin(115200);
  Serial.println("Hello, EPS32.");

  dhtSensor.setup(dht_PIN, DHTesp::DHT22);

  pinMode(led1_PIN, OUTPUT);
  pinMode(led2_PIN, OUTPUT);

}

void loop() {
  //Put your main code here!
  delay(200);
  currtime = millis();
  if (currtime-prevtime >= timer){

    TempAndHumidity data = dhtSensor.getTempAndHumidity();
    Serial.println("Temp: " + String(data.temperature, 2) + "°C");
    Serial.println("Humidity: " + String(data.humidity, 1) + "%");
    Serial.println("---");
    digitalWrite(led1_PIN, HIGH);
    delay(200);
    prevtime = currtime;
  }
  digitalWrite(led1_PIN, LOW);
}
