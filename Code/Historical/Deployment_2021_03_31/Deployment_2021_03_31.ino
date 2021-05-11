
#include <SPI.h>                                                 //SD card
#include <SD.h>
File myFile;

const int LED = 13;

String fileName = "4_23_s0.CSV";


/*
 * Loop:
 *  Collect Data ~1min
 *  Start SD
 *  Store Data
 *  Stop SD
 *  Wait ~4min
 *
*/
#define trigPin 6
#define echoPin 7
// Define variables:
long duration;
double distance;
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}
void loop() {
  float average = waterHeight();
  float pressure = analogRead(A0);
  Serial.println(average);

  while (!Serial) {;}
  Serial.print("Initializing SD card...");
  
  if(!SD.begin(53)) {//work on this
    Serial.println("initialization failed!");
    digitalWrite(LED, LOW);
    delay(10000);
  }
  else{
    Serial.println("Success!");
  }
  
  digitalWrite(LED, HIGH);
  myFile = SD.open(fileName, FILE_WRITE);
  
  if(myFile){
    //Serial.println(fileName);
    long int t = millis();
    myFile.print(t);
    myFile.print(",");
    myFile.print(average);
    myFile.print(",");
    myFile.println(pressure);
    Serial.print(t);
    Serial.print(", ");
    Serial.print(average);
    Serial.print(",");
    Serial.println(pressure);
  }
  myFile.close();
  
  delay(1000); //delay between samples
}




float waterHeight(){                                                //Get Water Height
  byte minLim = 0;
  int maxLim =  1000;
  int count = 10;
  float avg = 0;
  byte err = 0;
  int i = 0;
  byte j = 0; //number of iterations on single i
  int k = 0; //number of iterations
  float dist[count], distance; 
  long duration;
  while(i < count){
      digitalWrite(trigPin, LOW); 
      delayMicroseconds(5); 
      digitalWrite(trigPin, HIGH); 
      delayMicroseconds(10); 
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = (duration*.0343)/2; 
      //Serial.println(i);
      if(i > 2){                                                  //Get "good" data
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
        if(err > 200){
          //Serial.print("Error: ");
          //Serial.print(err);
          //Serial.println(dist[0]);
          i = 0;
        }
      }
      else if(j > count*2){
        i = 0;
        j = 0;
//        Serial.println();
        //Serial.println("Reset");
      }
      else if(i == 3){
        k++;
        if(k > 7){
          i = 0;
          k = 0;
          //Serial.println("Reset");
        }
      }
      j++;
      delay(10);
  }
  float Max = dist[0]; //Calculate Max and Min's                               //Get Max, Min, and Average
  float Min = dist[0];
  float tot = dist[0];
  for(int i = 1; i<count; i++){
    Max = max(Max,dist[i]);
    Min = min(Min,dist[i]);
    tot = tot + dist[i];
  }
  float average = tot/count;
  return(average);
}
