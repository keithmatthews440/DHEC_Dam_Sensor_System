
#define FONA_PWRKEY 6
#define FONA_RST 7
#define FONA_TX 10 // Microcontroller RX
#define FONA_RX 11 // Microcontroller TX

#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
#include "AT_shorts.h"

void setup() {
  Serial.begin(9600);
  Start();

  sendText("8645016496","ABC");
}

void loop() {
  delay(100);
  if(Serial.available()){
    checkSerial();
  }
}
