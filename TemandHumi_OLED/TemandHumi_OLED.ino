#include "DHTesp.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int led1_PIN = 4;
int led2_PIN = 2;

int dht_PIN = 23;

unsigned long timer = 5000;
unsigned long prevtime = 0;
unsigned long currtime = 0;

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHTesp dhtSensor; //dhtSensor = new DHTesp()

void init_oled(){
  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
    delay(2000);         // wait for initializing           
  }

  oled.clearDisplay();          // clear display
  oled.setTextSize(1);          // text size
  oled.setTextColor(WHITE);     // text color
  oled.setCursor(0, 10);
  oled.println("Hello, OLED !.");
  oled.display();
}

void oled_display(int t, int h){
  oled.clearDisplay();          // clear display
  oled.setTextSize(1);          // text size
  oled.setTextColor(WHITE);
  oled.setCursor(0, 10);        // position x,y to display
  oled.println("Temp: " + String(t) + " C."); 
  oled.setCursor(0, 25);  
  oled.println("Humidity: " + String(h) + " %");
  oled.display();
}


void setup() {
  //Setup code here!
  Serial.begin(115200);
  Serial.println("Hello, EPS32.");
  dhtSensor.setup(dht_PIN, DHTesp::DHT22);
  init_oled();
  pinMode(led1_PIN, OUTPUT);
  pinMode(led2_PIN, OUTPUT);

  pinMode(trig_Pin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echo_Pin, INPUT); // Sets the echoPin as an Input
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
    
    oled_display(data.temperature, data.humidity);

    prevtime = currtime;
  }
  digitalWrite(led1_PIN, LOW);
}
