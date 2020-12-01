#include <SPI.h>
#include <SD.h>
#include <string.h>

const int trigPin = 23; 
const int echoPin = 25;
float duration, distance; 
float MaxWH = 0;
float MinWH = 0;
float averageWH = 0;

File myFile;
int pinCS = 53;
int day = 1;
unsigned long count = 100; //1 day at 10 a second = 864000
int rate = 100; //10 a second = 100
unsigned long t;
String fileName[] = {"day1.txt","day2.txt","day3.txt","day4.txt","day5.txt","day6.txt","day7.txt"};


void setup() { 
 pinMode(trigPin, OUTPUT); 
 pinMode(echoPin, INPUT); 
 pinMode(pinCS, OUTPUT);
 Serial.begin(9600); 
} 


void loop(){
  getHeight();
  Serial.println(); //Return Data
  Serial.print("Max: ");
  Serial.print(MaxWH);
  Serial.print(" | Min: ");
  Serial.print(MinWH);
  Serial.print(" | Average: ");
  Serial.println(averageWH);
  ReadWrite();
}


int getHeight(){
  int minLim = 0;
  int maxLim =  300;
  int count = 100;
  float avg = 0;
  float err = 0;
  int i = 0;
  int j = 0;
  int k = 0;
  float dist[count] = {};
  while(i < count){
      digitalWrite(trigPin, LOW); 
      delayMicroseconds(2); 
      digitalWrite(trigPin, HIGH); 
      delayMicroseconds(10); 
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = (duration*.0343)/2; 
      //Serial.println(distance);
      if(i > 2){
        err = abs(distance-avg)/avg*100;
        if(err < 20){
          dist[i] = distance;
          i++;
        }
      }
      else if(distance < maxLim && distance > minLim && i <= 2){
        dist[i] = distance;
        i++;
      }
      if(i == 2){
        avg = (dist[0]+dist[1]+dist[2])/3;
        err = abs(dist[0]-avg)/avg*100;
        if(err > 50){
          i = 0;
        }
      }
      else if(j > count*2){
        i = 0;
        j = 0;
        Serial.println();
        Serial.println("Reset");
      }
      else if(i == 3){
        k++;
        if(k > 7){
          i = 0;
          k = 0;
        }
      }
      j++;
      //Serial.println(err);
      //Serial.print(i);
      delay(10);
  }

  MaxWH = dist[0]; //Calculate Max and Min's
  MinWH = dist[0];
  float tot = dist[0];
  for(int i = 1; i<count; i++){
    MaxWH = max(MaxWH,dist[i]);
    MinWH = min(MinWH,dist[i]);
    tot = tot + dist[i];
  }
  averageWH = tot/count;
}

int ReadWrite(){
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  myFile = SD.open(fileName[day-1], FILE_WRITE);
  if(myFile){
    Serial.println(fileName[day-1]);
    for (unsigned long i=0;i<count;i=i+1) {
      getHeight();
      t = millis();
      myFile.print(t);
      myFile.print(",");
      myFile.print(MaxWH);
      myFile.print(",");
      myFile.print(MinWH);
      myFile.print(",");
      myFile.print(averageWH);
      myFile.print(";");
      Serial.println(i);
      delay(rate);
    }
    day = day + 1;
  }
  else{
    Serial.println("FAILURE");
  }
  myFile.close();
}
