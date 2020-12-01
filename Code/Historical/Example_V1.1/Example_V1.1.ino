const int trigPin = 2; 
const int echoPin = 3;
float duration, distance;

void setup() { 
 pinMode(trigPin, OUTPUT); 
 pinMode(echoPin, INPUT); 
 Serial.begin(9600); 
}

void loop() { 
 dataSample();
}

int dataSample(){
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

  float Max = dist[0]; //Calculate Max and Min's
  float Min = dist[0];
  float tot = dist[0];
  for(int i = 1; i<count; i++){
    Max = max(Max,dist[i]);
    Min = min(Min,dist[i]);
    tot = tot + dist[i];
  }
  float average = tot/count;
  
  Serial.println(); //Return Data
  Serial.print("Max: ");
  Serial.print(Max);
  Serial.print(" | Min: ");
  Serial.print(Min);
  Serial.print(" | Average: ");
  Serial.println(average);
}
