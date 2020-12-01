 //by keith matthews
//backlog: relative time, code comments, clean code

#include <SPI.h>
#include <SD.h>
#include <Wire.h>

File myFile;

//******PARAMETERS*****
String fileName = "test2.csv";
int Delay = 3000; //delay between each set of measurements 
int count = 100;   //data set size
int cDelay = 100; //delay inbetween each measurement


const int vPin = A3; //Pin being read
int Min, Max;
float USHeight;
float Ave; //Average

const int trigPin =22;                                            //Ultrasonic
const int echoPin = 23;



void setup() {
  Wire.begin();
  Serial.begin(9600);
  pinMode(vPin, INPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  
  while (!Serial) {;}
  Serial.print("Initializing SD card...");
  while(!SD.begin(53)) {
    Serial.println("initialization failed!");
    delay(1000);
  }
  myFile = SD.open(fileName, FILE_WRITE);
  myFile.println("Absolute Time,Minimum,Average,Maximum");
  myFile.close();
}

void loop() {
  //getData();
  fileWrite();
  delay(Delay);
  //Serial.println(Ave);
  Serial.println(analogRead(A8));
}

int fileWrite(){
  while (!Serial) {;}
  Serial.print("Initializing SD card...");
  
  while(!SD.begin(53 )) { //normally 10 for uno
    Serial.println("initialization failed!");
    delay(1000);
  }
  
  myFile = SD.open(fileName, FILE_WRITE);
  
  if(myFile){
    Serial.println(fileName);

    myFile.print(millis()); //Alternative if RTC doesn't work
    
//    myFile.print(",");
//    myFile.print(Min);
//    myFile.print(",");
//    myFile.print(Ave);
//    myFile.print(",");
//    myFile.print(Max);
    myFile.print(",");
    myFile.println(analogRead(A8));

    Serial.println(analogRead(A8));
    myFile.close();
  }
  
  else{
    Serial.println("Failed to collect data");
  }
}

//int getData(){
//  Min = 1023;
//  Ave = 0;
//  Max = 0;
//  int dataSet[count];
//  for(int i=0; count>i; i++){
//    getUS();
//    dataSet[i] = USHeight;
//    Ave = Ave + dataSet[i];
//    if(dataSet[i] > Max){
//      Max = dataSet[i];    
//    }
//    if(dataSet[i] < Min){
//      Min = dataSet[i];
//    }
//  }
//  Ave = Ave/count;
//}
//
//int getUS(){
//  float duration;
//  digitalWrite(trigPin, LOW); 
//  delayMicroseconds(2); 
//  digitalWrite(trigPin, HIGH); 
//  delayMicroseconds(10); 
//  digitalWrite(trigPin, LOW);
//  duration = pulseIn(echoPin, HIGH);
//  USHeight = (duration*.0343)/2; 
//}
