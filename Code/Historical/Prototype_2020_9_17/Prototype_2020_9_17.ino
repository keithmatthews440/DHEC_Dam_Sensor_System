/*
 * Written by: Keith Matthews as part of a USC senior design project for use by DHEC
 * 
 * Included libraries
 */
#include <SPI.h>
#include <SD.h>
File myFile;


/*
 * INPUTS
 */



 int pSD = 9; //Power for the SD card reader (so it can be turned on and off)
 int pUS = 8; //Power for the ultrasonic sensor
 int pPS = 7; //Power for the pressure sensor



void setup() {
  pinMode(pSD, OUTPUT); //Set power pins to outputs
  pinMode(pUS, OUTPUT);
  pinMode(pPS, OUTPUT);

  digitalWrite(pSD, HIGH); //Power on units
  digitalWrite(pUS, HIGH);
  digitalWrite(pPS, HIGH);  
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial); // wait for serial port to connect. Needed for native USB port only
  Serial.print("Initializing SD card...");
  
  if (!SD.begin(10)) {  // open the file. note that only one file can be open at a time, so you have to close this one before opening another.
    Serial.println("initialization failed!");
    while (1);
  }
  
  Serial.println("initialization done.");
  myFile = SD.open("test.txt", FILE_WRITE);
  
  if (myFile) {// if the file opened okay, write to it:
    Serial.print("Writing to test.txt...");
    myFile.println("This is a test file :)");
    myFile.println("testing 1, 2, 3.");
    for (int i = 0; i < 20; i++) {
      myFile.println(i);
    }
    myFile.close();// close the file:
    Serial.println("done.");
  } 
  else {// if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop() {
  //if time% getData
  //if time% sendData
  //if dH> or H> or H< sendData + alert

}


/*
 * Loops: Get GPS, Get Water heights and solve for dH, Get and store data (1 or 2 loops?)
 */
