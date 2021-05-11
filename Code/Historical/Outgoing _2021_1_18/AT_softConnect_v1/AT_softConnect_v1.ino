
#define FONA_PWRKEY 6
#define FONA_RST 7
#define FONA_TX 10 // Microcontroller RX
#define FONA_RX 11 // Microcontroller TX

#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
#include "AT_shorts.h"

byte ctrlZ = 0x1A;

void setup() {
  Serial.begin(9600);
  Start();

  /*
   * Code below is the test code, attempting an TCP connection with holograms servers. Hologram requires data to be sent in a JSON format
   *
  delay(1000);
  Send("AT+CIPSTART=?");
  //Send("AT+CIPMUX=0");
  Send("AT+CIPSTART=\"TCP\",cloudsocket.hologram.io,9999",5000);
  waitReply(); //try removing this?
  Send("AT+CIPSEND");
  delay(1000);
  //Read();
  fonaSS.print("{\"devicekey\":\"@b(e0O2d\", \"data\":\"Hello, World!\", \"tags\":[\"Dam_Data\", \"TOPIC2\"]}");
  SendHex(ctrlZ);
  waitReply();
  Send("AT+CIPCLOSE=0");
  */
  SendText("8645016496","K");
}

void loop() {
  checkSerial();
  delay(1000);
}
