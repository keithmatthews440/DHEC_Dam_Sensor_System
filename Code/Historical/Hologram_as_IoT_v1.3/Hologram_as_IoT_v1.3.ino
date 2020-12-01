/**************************************************************
  This sketch will send a message to the hologram cloud.
  You can use the Data routers to forward the data to IoT platforms.
  https://hologram.io
  Copyright (c) 2016 Konekt, Inc.  All rights reserved.
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 **************************************************************/

// Select your modem:
#define TINY_GSM_MODEM_SIM800
// #define TINY_GSM_MODEM_SIM808

// Increase RX buffer if needed
//#define TINY_GSM_RX_BUFFER 512

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>


// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1

// or Software Serial on Uno, Nano
//#include <SoftwareSerial.h>
//SoftwareSerial SerialAT(2, 3); // RX, TX

// Hologram Credentials
String api_key = "9CwJWKb7GQRBDhssmqUOhWDA6wQHUr";
String deviceid = "735590";


// Your GPRS credentials
const char apn[]  = "hologram";

// Server details
const char server[] = "dashboard.hologram.io";
const int  port = 443; // port 443 is default for HTTPS

TinyGsm modem(SerialAT);

TinyGsmClientSecure client(modem);
HttpClient http(client, server, port);

void setup() {
  setup_modem();
}

void loop() {
  // Connect Network
  connect_cellular();
  
  // Ready Data
  String data = String(analogRead(0));
  String tags = "arduino,hello_world";

  //Send to Hologram
  send_message(data, tags);

  //Disconnect GPRS and HTTP
  disconnect_networks();

  delay(120000); // Sleep for 2 minutes
}


void send_message(String data, String tags) {
  SerialMon.println("Sending Message to Hologram");

  String path = "/api/1/csr/rdm?apikey=" + api_key;
  String contentType = "application/x-www-form-urlencoded";
  
  String postData = "deviceid=" + deviceid + "&data=" + data;
  postData = postData + "&tags=" + tags;

  http.post(path, contentType, postData); //, contentType, buf

//  read the status code and body of the response
//  int statusCode = http.responseStatusCode();
//  String response = http.responseBody();

//  SerialMon.print("Status code: ");
//  SerialMon.println(statusCode);

//  SerialMon.print("Response: ");
//  SerialMon.println(response);

  delay(5000);
}

void setup_modem(){
  // Set console baud rate
  SerialMon.begin(115200);
  delay(10);

  // Set GSM module baud rate
  SerialAT.begin(115200);
  delay(3000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println(F("Initializing modem..."));
  modem.restart();

  if (!modem.hasSSL()) {
    SerialMon.println(F("SSL is not supported by this modem"));
    while (true) {
      delay(1000);
    }
  }
}

void connect_cellular(){
  SerialMon.print(F("Waiting for network..."));
  if (!modem.waitForNetwork()) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" OK");

  SerialMon.print(F("Connecting to "));
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, "", "")) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" OK");
}

void disconnect_networks(){
  SerialMon.println("Disconnecting");
  http.stop();
  SerialMon.println(F("Server disconnected"));

  modem.gprsDisconnect();
  SerialMon.println(F("GPRS disconnected"));
}
