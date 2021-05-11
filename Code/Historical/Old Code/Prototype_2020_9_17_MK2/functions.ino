

int waterHeight(){   //Get Water Height
  float pressure, distance, duration;  
  
  Serial.println("Getting Water Height");     
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2; 
  pressure = digitalRead(pPin);
  
  
  
}


int relayData(){   //Store, display, and send collected data
  
}
