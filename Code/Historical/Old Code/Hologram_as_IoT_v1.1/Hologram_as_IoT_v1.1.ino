#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 8);
void setup(){  
  mySerial.begin(19200); // the GPRS baud rate  
  Serial.begin(19200); // the GPRS baud rate  
  delay(500);
}
  
void loop(){  // Input 'h' to run the test HTTP program  
  if (Serial.available())    
  switch(Serial.read())    {case 'h':        SubmitHttpRequest();        break;    }  
  if (mySerial.available())    
  Serial.write(mySerial.read());
  SubmitHttpRequest();
}//// Note: the time of the delays are very important

void SubmitHttpRequest(){  // Query signal strength of device  
  Serial.print("H");
  mySerial.println("AT+CSQ");  
  delay(100);  
  ShowSerialData();  // Check the status of Packet service attach. '0' implies device is not attached and '1' implies device is attached.  
  mySerial.println("AT+CGATT?");  
  delay(100);  
  ShowSerialData();  // Set the SAPBR, the connection type is using gprs  
  mySerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");  delay(1000);  ShowSerialData();  // Set the APN  
  mySerial.println("AT+SAPBR=3,1,\"APN\",\"apn.konekt.io\"");  delay(4000);  ShowSerialData();  // Set the SAPBR, for detail you can refer to the AT command manual  
  mySerial.println("AT+SAPBR=1,1");  delay(2000);  ShowSerialData();  // Init the HTTP request  
  mySerial.println("AT+HTTPINIT");  delay(2000);  ShowSerialData();  // Set HTTP params, the second param is the website to request  
  mySerial.println("AT+HTTPPARA=\"URL\",\"konekt.io/test.html\"");  delay(1000);  ShowSerialData();  //Set the context ID  
  mySerial.println("AT+HTTPPARA=\"CID\",1");  delay(1000);  ShowSerialData();  // Submit the request  
  mySerial.println("AT+HTTPACTION=0");  // The delay is very important, the delay time is base on the  // return time from the website, if the return data is very  // large, the time required might be longer.  
  delay(10000);  ShowSerialData();  // Read the data from the accessed website  
  mySerial.println("AT+HTTPREAD");  delay(10000);  ShowSerialData();  // Close the HTTP connection and display the data  
  mySerial.println("AT+HTTPTERM");  delay(100);// 
  ShowSerialData();// This is to show the data from gprs shield, to help// see how the gprs shield submits an http request.
}

void ShowSerialData(){  
  while(mySerial.available()!=0){
    Serial.write(mySerial.read());
  }
}
