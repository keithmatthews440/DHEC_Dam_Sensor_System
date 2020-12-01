
#define FONA_PWRKEY 6
#define FONA_RST 7
#define FONA_TX 10 // Microcontroller RX
#define FONA_RX 11 // Microcontroller TX

#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
#include "AT_shorts.h"

char imei[16] = {0};
int i = 0;
String byte1, byte2, input;
byte message[] = {0x0D, 0x22, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x22, 0x1A}; //CR"Hello World"^Z
String number = "+18645016496";
byte AT[] = {0x41, 0x54};

void setup() {
  //fonaSS.write(AT, sizeof(AT));
  SendHex(AT);
  delay(1000);
  Read();
  Serial.begin(9600);
  Start();
}

void loop() {
  delay(1000);
  Read();
}

/*
 * "AT+CMGS=\"%s\""
 * 
 * AT+CMGS="18645016496"\nHello_World\n^Z
 * AT+CMGS=\"18645016496\" <CR> \"Hello World\"SUB
 * <CR> means enter AKA println
 */ 
