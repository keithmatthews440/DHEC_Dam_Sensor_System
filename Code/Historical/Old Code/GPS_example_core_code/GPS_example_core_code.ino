

#include "Adafruit_FONA.h" // https://github.com/botletics/SIM7000-LTE-Shield/tree/master/Code
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_FONA.h"

#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif

#define SIMCOM_7000 // SIM7000
#define PROTOCOL_HTTP_GET         // Generic

/************************* PIN DEFINITIONS *********************************/
#define FONA_PWRKEY 6
#define FONA_RST 7
#define FONA_TX 10 // Microcontroller RX
#define FONA_RX 11 // Microcontroller TX
#define LED 13 // Just for testing if needed!

#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

#ifdef SIMCOM_2G
  Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
#elif defined(SIMCOM_3G)
  Adafruit_FONA_3G fona = Adafruit_FONA_3G(FONA_RST);
#elif defined(SIMCOM_7000) || defined(SIMCOM_7500)
  Adafruit_FONA_LTE fona = Adafruit_FONA_LTE();
#endif

/****************************** OTHER STUFF ***************************************/
#include <avr/sleep.h>
#include <avr/power.h>
#include <Wire.h>
#include "Adafruit_MCP9808.h"

Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

#define samplingRate 10 // The time in between posts, in seconds

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);
char imei[16] = {0}; // Use this for device ID
uint8_t type;
uint16_t battLevel = 0; // Battery level (percentage)
float latitude, longitude, speed_kph, heading, altitude, second;
uint8_t counter = 0;

char URL[200];  // Make sure this is long enough for your request URL
char body[100]; // Make sure this is long enough for POST body
char latBuff[12], longBuff[12], locBuff[50], speedBuff[12],
     headBuff[12], altBuff[12], tempBuff[12], battBuff[12];

void setup() {
  Serial.begin(9600);
  Serial.println(F("*** SIMCom Module IoT Example ***"));

  #ifdef LED
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);
  #endif
  
  pinMode(FONA_RST, OUTPUT);
  digitalWrite(FONA_RST, HIGH); // Default state
  pinMode(FONA_PWRKEY, OUTPUT);
  powerOn(); // Power on the module
  moduleSetup(); // Establishes first-time serial comm and prints IMEI
  if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find the MCP9808!");
    while (1);
  }
  fona.setFunctionality(1); // AT+CFUN=1
  fona.setNetworkSettings(F("hologram")); // For Hologram SIM card

  #ifndef turnOffShield
    while (!fona.enableGPS(true)) {
      Serial.println(F("Failed to turn on GPS, retrying..."));
      delay(2000); // Retry every 2s
    }
    Serial.println(F("Turned on GPS!"));
  #endif
}

void loop() {
  delay(500); // I found that this helps
  #ifdef turnOffShield
    while (!fona.enableGPS(true)) {
      Serial.println(F("Failed to turn on GPS, retrying..."));
      delay(2000); // Retry every 2s
    }
    Serial.println(F("Turned on GPS!"));
  #endif

  while (!fona.getGPS(&latitude, &longitude, &speed_kph, &heading, &altitude)) {
    Serial.println(F("Failed to get GPS location, retrying..."));
    delay(2000); // Retry every 2s
  }
  Serial.println(F("Found 'eeeeem!"));
  Serial.println(F("---------------------"));
  Serial.print(F("Latitude: ")); Serial.println(latitude, 6);
  Serial.print(F("Longitude: ")); Serial.println(longitude, 6);
  Serial.print(F("Speed: ")); Serial.println(speed_kph);
  Serial.print(F("Heading: ")); Serial.println(heading);
  Serial.print(F("Altitude: ")); Serial.println(altitude);

  #if defined(turnOffShield) && !defined(SIMCOM_3G) && !defined(SIMCOM_7500) // If the shield was already on, no need to re-enable
    if (!fona.enableGPRS(false)) Serial.println(F("Failed to disable GPRS!"));
    while (!fona.enableGPRS(true)) {
      Serial.println(F("Failed to enable GPRS, retrying..."));
      delay(2000); // Retry every 2s
    }
    Serial.println(F("Enabled GPRS!"));
  #endif
  dtostrf(latitude, 1, 6, latBuff);
  dtostrf(longitude, 1, 6, longBuff);
  dtostrf(speed_kph, 1, 0, speedBuff);
  dtostrf(heading, 1, 0, headBuff);
  dtostrf(altitude, 1, 1, altBuff);
  sprintf(locBuff, "%s,%s,%s,%s", speedBuff, latBuff, longBuff, altBuff); // This could look like "10,33.123456,-85.123456,120.5"
 
  #ifdef turnOffShield
    if (!fona.enableGPRS(false)) Serial.println(F("Failed to disable GPRS!"));
    if (!fona.enableGPS(false)) Serial.println(F("Failed to turn off GPS!"));
    counter = 0;
    while (counter < 3 && !fona.powerDown()) { // Try shutting down 
      Serial.println(F("Failed to power down FONA!"));
      counter++; // Increment counter
      delay(1000);
    }
  #endif

  #ifndef samplingRate
    Serial.println(F("Shutting down..."));
    delay(5); // This is just to read the response of the last AT command before shutting down
    MCU_powerDown(); // You could also write your own function to make it sleep for a certain duration instead
  #else
    Serial.print(F("Waiting for ")); Serial.print(samplingRate); Serial.println(F(" seconds\r\n"));
    delay(samplingRate * 1000UL); // Delay  
    powerOn(); // Powers on the module if it was off previously
    #ifdef turnOffShield
      moduleSetup();
    #endif
  #endif
}

void powerOn() {
  digitalWrite(FONA_PWRKEY, LOW);
  #if defined(SIMCOM_7000)
    delay(100); // For SIM7000
  #endif
  digitalWrite(FONA_PWRKEY, HIGH);
}

void moduleSetup() {
  fonaSS.begin(115200); // Default SIM7000 shield baud rate
  Serial.println(F("Configuring to 9600 baud"));
  fonaSS.println("AT+IPR=9600"); // Set baud rate
  delay(100); // Short pause to let the command run
  fonaSS.begin(9600);
  if (! fona.begin(fonaSS)) {
    Serial.println(F("Couldn't find FONA"));
    while(1); // Don't proceed if it couldn't find the device
  }

  type = fona.type();
  Serial.println(F("FONA is OK"));
  Serial.print(F("Found "));
  Serial.println(F("SIM7500A (American)"));
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); Serial.println(imei);
  }
}

void MCU_powerDown() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  ADCSRA = 0; // Turn off ADC
  power_all_disable ();  // Power off ADC, Timer 0 and 1, serial interface
  sleep_enable();
  sleep_cpu();
}
