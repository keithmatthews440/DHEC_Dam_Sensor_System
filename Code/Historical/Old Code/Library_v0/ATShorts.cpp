#define FONA_PWRKEY 6
#define FONA_RST 7
#define FONA_TX 10 // Microcontroller RX
#define FONA_RX 11 // Microcontroller TX

#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

#include "Arduino.h"
#include "ATShorts.h"

ATShorts::ATShorts(){
  Serial.println("Stuff");
}

String ATShorts::read(String strIN){
  String out;
  char r;
  while(fonaSS.available()){
    r = char(fonaSS.read());
    out = out + String(r);
    delay(50);
  }
  Serial.print("<--");
  Serial.print(strIN);
  Serial.println(out);
  return out;
}

void ATShorts::print(String strIN, int wait= 1000){
  fonaSS.println(strIN);
  Serial.println("-->"+strIN);
  delay(wait);
  read(strIN);
}

void ATShorts::begin(void){
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
  print("AT+CSCS=\"UCS2\"\r");
  print("AT");
  print("ATE0");
  print("ATI");
  print("AT+CPMS=\"SM\",\"SM\",\"SM\"");
  print("AT+GSN");
  print("AT+CFUN=1"); 
  print("AT+CGDCONT=1,\"IP\",\"hologram\"");
  print("AT+CGREG?");
  print("AT+CMGF=1");
  delay(3000);
}

void ATShorts::printHex(byte hexIN[]){
  fonaSS.write(hexIN, sizeof(hexIN));
  read();
}
