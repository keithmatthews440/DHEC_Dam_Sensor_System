
#define FONA_PWRKEY 6
#define FONA_RST 7
#define FONA_TX 10 // Microcontroller RX
#define FONA_RX 11 // Microcontroller TX

#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
#include "AT_shorts.h"

const int trigPin = 7;                                            //Ultrasonic
const int echoPin = 6;

#include <SPI.h>                                                 //SD card
#include <SD.h>
File myFile;

char imei[16] = {0};
int i = 0;
String byte1, byte2, input;
byte message[] = {0x0D, 0x22, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x22, 0x1A}; //CR"Hello World"^Z
byte TCPmess[] = {}; //{"devicekey":"@b(e0O2d", "data":"Hello, World!", "tags":["Dam_Data", "TOPIC2"]} ^Z
byte ctrlZ = 0x1A;
String number = "+18645016496";
byte AT[] = {0x41, 0x54};

void setup() {
  //fonaSS.write(AT, sizeof(AT));
  SendHex(AT);
  delay(1000);
  Read();
  Serial.begin(9600);
  Start();

  
  //delay(1000);
  //sendTCP();
  
//  Send("AT+CIPSTART=?");
//  //Send("AT+CIPMUX=0");
//  Send("AT+CIPSTART=\"TCP\",cloudsocket.hologram.io,9999",5000);
//  delay(10000);
//  Send("AT+CIPSEND");
//  delay(1000);
//  //Read();
//  fonaSS.print("{\"devicekey\":\"@b(e0O2d\", \"data\":\"Hello, World!\", \"tags\":[\"Dam_Data\", \"TOPIC2\"]}");
//  SendHex(ctrlZ);
//  Read();
//  delay(5000);
//  //SendHex(TCPmess);
//  Send("AT+CIPCLOSE=0");
}

void loop() {
  
  getData();
  sendTCP();
  delay(900000);
  Start();
}

void sendTCP(){
   Send("AT+CIPSTART=?");
  //Send("AT+CIPMUX=0");
  Send("AT+CIPSTART=\"TCP\",cloudsocket.hologram.io,9999",5000);
  delay(10000);
  Send("AT+CIPSEND");
  delay(1000);
  //Read();
  fonaSS.print("{\"devicekey\":\"@b(e0O2d\", \"data\":\"Hello, World!\", \"tags\":[\"Dam_Data\", \"TOPIC2\"]}");
  SendHex(ctrlZ);
  Read();
  delay(5000);
  //SendHex(TCPmess);
  Send("AT+CIPCLOSE=0");
}

void getData(){
  float distance, duration;
  Serial.println("Getting Water Height");  //Get data
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2; 

  Serial.println(distance);

  while (!Serial) {;}
  Serial.print("Initializing SD card...");
  int i = 0;
  while(!SD.begin(53)) {
    i = i + 1;
    Serial.println("initialization failed!");
    Serial.print("Initializing SD card...");
    delay(1000);
    if(i >10){
      break;
    }
  }
  
  myFile = SD.open("Data.csv", FILE_WRITE);
  
  if(myFile){
    Serial.println("writing to file");
    myFile.print(millis());
    myFile.print(", ");
    myFile.print(analogRead(A0));
    myFile.print(",");
    myFile.println(distance);
  }

  myFile.close();
  Serial.println("File updated!");
}
