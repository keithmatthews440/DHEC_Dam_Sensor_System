#include <Wire.h>                                                  //SIM7000

const int trigPin = 7;                                            //Ultrasonic
const int echoPin = 6;
const int pPin = 4;

const int pSD = 9;
const int pUSS = 8;
const int pPS = 5;

#include <SPI.h>                                                 //SD card
#include <SD.h>
File myFile;

long int dly = 60000; //ms
float temperature, humidity, average, Max, Min;


void setup(){                                                      //Setup
  Serial.begin(9600); //will this work with ultrasonic?
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(pSD, OUTPUT);
  pinMode(pUSS, OUTPUT);
  pinMode(pPS, OUTPUT);

  digitalWrite(pSD, HIGH);
  digitalWrite(pUSS, HIGH);
}

void loop(){  //Loop
  float pressure, distance, duration;
  Serial.println("Getting Water Height");  //Get data
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2; 
  pressure = digitalRead(pPin);

  Serial.print(distance);
  Serial.print(", ");
  Serial.println(pressure);

  while (!Serial) {;}
  Serial.print("Initializing SD card...");
  
  while(!SD.begin(10)) {
    Serial.println("initialization failed!");
    delay(1000);
  }
  
  myFile = SD.open("Data.csv", FILE_WRITE);
  
  if(myFile){
    Serial.println("writing to file");
    myFile.print(distance);
    myFile.print(",");
    myFile.println(pressure);
  }

  myFile.close();
  Serial.print("File updated!");

  delay(10000);
}
