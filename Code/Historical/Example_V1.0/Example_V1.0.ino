const int trigPin = 9; 
const int echoPin = 10;
float duration, distance;
int goodData = true;

//int count = 10; //Cross functions Data Count ***VITAL***

void setup() { 
 pinMode(trigPin, OUTPUT); 
 pinMode(echoPin, INPUT); 
 Serial.begin(9600); 
 //BluetoothSerial.begin(9600);
}

void loop() { 
 dataSample();
}

int dataSample(){
  int count = 10;
  float dist[count] = {};
  for(int i = 0; i<count; i++){ //Gather data
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2); 
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration*.0343)/2; 
    dist[i] = distance;
    //Serial.print(dist[i]);
    //Serial.print(", ");
    delay(100);
  }
  
  float Max = dist[0]; //Calculate Max and Min's
  float Min = dist[0];
  float tot = dist[0];
  for(int i = 1; i<count; i++){
    Max = max(Max,dist[i]);
    Min = min(Min,dist[i]);
    tot = tot + dist[i];
  }
  float avg = tot/count;

  dataCheck(dist, avg, count);
  
  if(goodData){
    Serial.println(); //Return Data
    Serial.print("Max: ");
    Serial.print(Max);
    Serial.print(" | Min: ");
    Serial.print(Min);
    Serial.print(" | Average: ");
    Serial.println(avg);
  }
}

int dataCheck(float data[], float average, float Count){ //Better code for later https://www.geeksforgeeks.org/c-program-to-insert-an-element-in-an-array/
  goodData = true;
  int e;
  for(int i = 1; i<Count; i++){
    e = abs(data[i+1]-data[i])/average;
    if(e > 0.2){
      goodData = false;
    }
  }
}
