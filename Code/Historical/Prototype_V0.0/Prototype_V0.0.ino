#include <Wire.h>                                                  //SIM7000
#include <DFRobot_SIM7000.h>
DFRobot_SIM7000 sim7000;
SoftwareSerial  mySerial(8,7);
static char buff[350];

const int trigPin = 9;                                            //Ultrasonic
const int echoPin = 10;

#include <SPI.h>                                                 //SD card
#include <SD.h>
File myFile;

long int dly = 60000; //ms
float temperature, humidity, average, Max, Min;
char longitude, latitude, data[50];


void setup(){                                                      //Setup
  Serial.begin(115200); //will this work with ultrasonic?
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
}

void loop(){                                                       //Loop
  if(millis()%dly == 0){
    getGPS();
    waterHeight();
    relayData(); //NOT DONE YET
  }
}
