int getGPS(){
//    Serial.println("Getting position......");
    if(sim7000.getPosition()){    //Get the current position
        longitude = sim7000.getLongitude();
        latitude = sim7000.getLatitude();
//        Serial.println("Posistion acquired");
      //  Serial.println(sim7000.getLongitude());                    //Get longitude
      //  Serial.print("Latitude :");
      //  Serial.println(sim7000.getLatitude());                     //Get latitude
    }
}

int waterHeight(){                                                //Get Water Height
  byte minLim = 0;
  int maxLim =  300;
  int count = 100;
  float avg = 0;
  byte err = 0;
  int i = 0;
  byte j = 0;
  int k = 0;
  float dist[count], distance, duration;
  while(i < count){
      digitalWrite(trigPin, LOW); 
      delayMicroseconds(2); 
      digitalWrite(trigPin, HIGH); 
      delayMicroseconds(10); 
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = (duration*.0343)/2; 
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
        if(err > 50){
          i = 0;
        }
      }
      else if(j > count*2){
        i = 0;
        j = 0;
//        Serial.println();
//        Serial.println("Reset");
      }
      else if(i == 3){
        k++;
        if(k > 7){
          i = 0;
          k = 0;
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
}


int relayData(){                                          //Store, display, and send collected data
  sprintf(data,"T: %dC H: %d% WLA: %dcm Max: %dcm Min: %dcm Long: %d Lat: %d",temperature, humidity, average, Max, Min, longitude, latitude); 

  pinMode(10, OUTPUT);
  SD.begin(4);
  Serial.println("initialization done.");
  myFile = SD.open("test.txt", FILE_WRITE);
  myFile.println("testing 1, 2, 3.");
  myFile.close();

  sim7000.turnON();
  //sim7000.openNetwork();
  sim7000.setBaudRate(19200);
  sim7000.checkSIMStatus();
  sim7000.setNetMode(NB);
  sim7000.checkSignalQuality();
  sim7000.attacthService();
//  sim7000.connect(TCP, "www.taobao",80); //Modify? see orginal code else statement
  sim7000.send("HEAD/HTTP/1.1\r\nHost:www.taobao.com\r\nConnection:keep-alive\r\n\r\n");     //Send Data Through TCP or UDP Connection
  int dataNum=sim7000.recv(buff,350,0);                                                          //Receive data
//  Serial.print("dataNum=");
//  Serial.println(dataNum);
//  Serial.println(buff);
  sim7000.closeNetwork();
  sim7000.turnOFF();
    
  Serial.println(data);
  delay(10);
}
