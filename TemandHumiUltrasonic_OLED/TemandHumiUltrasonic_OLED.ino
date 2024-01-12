#include "DHTesp.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int led1_PIN = 4;
int led2_PIN = 2;

int dht_PIN = 23;

int trig_Pin = 33;
int echo_Pin = 32;

unsigned long timer = 5000;
unsigned long prevtime = 0;
unsigned long currtime = 0;

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHTesp dhtSensor; //dhtSensor = new DHTesp()

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

float distanceInput;

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

void oled_display(int t, int h, int d){
  oled.clearDisplay();          // clear display
  oled.setTextSize(1);          // text size
  oled.setTextColor(WHITE);
  oled.setCursor(0, 10);        // position x,y to display
  oled.println("Temp: " + String(t) + " C."); 
  oled.setCursor(0, 25);  
  oled.println("Humidity: " + String(h) + " %");
  oled.setCursor(0, 45);  
  oled.println("Distance: " + String(d) + " cm.");
  oled.display();
}

float readUltraSonicSensor(){
  // Clears the trigPin
  digitalWrite(trig_Pin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig_Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_Pin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo_Pin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
  delay(200);
  return distanceCm;
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
    
    distanceInput = readUltraSonicSensor();

    oled_display(data.temperature, data.humidity, distanceInput);

    if (distanceInput <= 20){
      digitalWrite(led2_PIN, HIGH);
    }else{
      digitalWrite(led2_PIN, LOW);
    }
    prevtime = currtime;
  }
  digitalWrite(led1_PIN, LOW);
}
