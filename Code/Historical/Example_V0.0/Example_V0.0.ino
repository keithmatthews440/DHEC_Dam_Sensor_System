#include "dht11.h"
#define DHT11PIN 4
dht11 DHT11;

#include "SoftwareSerial.h"
#define BT_SERIAL_RX 12
#define BT_SERIAL_TX 11
SoftwareSerial BluetoothSerial(BT_SERIAL_RX, BT_SERIAL_TX);

const int trigPin = 9; 
const int echoPin = 10;
float duration, distance;

int photoPin = A0;
int light = 0;

void setup() { 
 pinMode(trigPin, OUTPUT); 
 pinMode(echoPin, INPUT); 
 Serial.begin(9600); 
 BluetoothSerial.begin(9600);
}

void loop() { 
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 
 digitalWrite(trigPin, HIGH); 
 delayMicroseconds(10); 
 digitalWrite(trigPin, LOW);

 duration = pulseIn(echoPin, HIGH);

 distance = (duration*.0343)/2; 

 Serial.print("D:"); 
 if(distance > 3000){
  Serial.print("null   ");
 }
 else{
  Serial.print(distance);
  Serial.print("cm");
 }

 int chk = DHT11.read(DHT11PIN);
 float h = ((float)DHT11.humidity, 2);
  
 Serial.print("  H:");
 Serial.print((float)DHT11.humidity, 2);
 Serial.print("%  T:");
 Serial.print((float)DHT11.temperature, 2);
 Serial.print("C  L:");

 light = analogRead(photoPin);
 Serial.println(light);

 delay(500); 
}
