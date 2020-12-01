
#define FONA_PWRKEY 6
#define FONA_RST 7
#define FONA_TX 10 // Microcontroller RX
#define FONA_RX 11 // Microcontroller TX

#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

char imei[16] = {0};
int i = 0;
String byte1, byte2, input;

void Send(String mess, int wait =1000);

void setup() {
  Serial.begin(9600);
  fonaSS.begin(115200);

  Serial.println(F("Configuring to 9600 baud"));
  fonaSS.println("AT+IPR=9600"); // Set baud rate
  delay(100); // Short pause to let the command run
  fonaSS.begin(9600);
  
  pinMode(FONA_RST, OUTPUT);
  pinMode(FONA_PWRKEY, OUTPUT);
  
  digitalWrite(FONA_RST, HIGH); // Default state  
  digitalWrite(FONA_PWRKEY, LOW);
  delay(100); // For SIM7000
  digitalWrite(FONA_PWRKEY, HIGH); 
  delay(1000);
  Send("AT+CSCS=\"UCS2\"\r");
  
//  while(i<3){
//    delay(1000);
//    Send("AT");
//    i=i+1;
//  }
//  Serial.println("End");
//
  Send("AT");
  Send("ATE0");
  Send("ATI");
  Send("AT+CPMS=\"SM\",\"SM\",\"SM\"");
  Send("AT+GSN");
  Send("AT+CFUN=1"); 
  Send("AT+CGDCONT=1,\"IP\",\"hologram\"");
  Send("AT+CGREG?");
  Send("AT+CMGF=1");
  delay(3000);

  Serial.println("Sending Text");
  //fonaSS.print("AT+CMGS=\"+18645016496\"CR\"Hello World\"SUB");
  fonaSS.print(AT+CMGS=\"+18645016496\");
  fonaSS.println(74, HEX);
  delay(100);
  String out;
  while(fonaSS.available()){
    char r = char(fonaSS.read());
    out = out + String(r);
    delay(50);
  }
  Serial.print("<--");
  Serial.println(out);
}

void loop() {
  if(Serial.available()){
    String byteRead = Serial.readString();
    input = input + byteRead;
    delay(100);
    if(!Serial.available()){
      Send(input);
      input = "";
    }
  }
}

void Send(String mess, int wait =1000){
  String out;
  char r;
  fonaSS.println(mess);
  Serial.println("-->"+mess);
  delay(wait);
  while(fonaSS.available()){
    r = char(fonaSS.read());
    out = out + String(r);
    delay(50);
  }
  Serial.print("<--");
  Serial.println(out);
}

/*
 * "AT+CMGS=\"%s\""
 * 
 * AT+CMGS="18645016496"\nHello_World\n^Z
 * AT+CMGS=\"18645016496\" <CR> \"Hello World\"SUB
 * 
 * 
 */ 
