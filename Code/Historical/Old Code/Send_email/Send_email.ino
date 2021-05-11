#include "Adafruit_FONA.h" // https://github.com/botletics/SIM7000-LTE-Shield/tree/master/Code

// You don't need the following includes if you're not using MQTT
// You can find the Adafruit MQTT library here: https://github.com/adafruit/Adafruit_MQTT_Library
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_FONA.h"

#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  // Required for Serial on Zero based boards
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif

// Define *one* of the following lines:
//#define SIMCOM_2G // SIM800/808/900/908, etc.
//#define SIMCOM_3G // SIM5320
#define SIMCOM_7000 // SIM7000
//#define SIMCOM_7500 // SIM7500

// Uncomment *one* of the following protocols you want to use
// to send data to the cloud! Leave the other commented out
#define PROTOCOL_HTTP_GET         // Generic
//#define PROTOCOL_HTTP_POST        // Generic
//#define PROTOCOL_MQTT_AIO         // Adafruit IO
//#define PROTOCOL_MQTT_CLOUDMQTT   // CloudMQTT

/************************* PIN DEFINITIONS *********************************/
// For SIM7000 shield
#define FONA_PWRKEY 6
#define FONA_RST 7
//#define FONA_DTR 8 // Connect with solder jumper
//#define FONA_RI 9 // Need to enable via AT commands
#define FONA_TX 10 // Microcontroller RX
#define FONA_RX 11 // Microcontroller TX
//#define T_ALERT 12 // Connect with solder jumper

// For SIM7500 shield
//#define FONA_PWRKEY 6
//#define FONA_RST 7
////#define FONA_DTR 9 // Connect with solder jumper
////#define FONA_RI 8 // Need to enable via AT commands
//#define FONA_TX 11 // Microcontroller RX
//#define FONA_RX 10 // Microcontroller TX
////#define T_ALERT 5 // Connect with solder jumper

#define LED 13 // Just for testing if needed!

// We default to using software serial. If you want to use hardware serial
// (because softserial isnt supported) comment out the following three lines 
// and uncomment the HardwareSerial line
#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);

// Use the following line for ESP8266 instead of the line above (comment out the one above)
//SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX, false, 256); // TX, RX, inverted logic, buffer size

SoftwareSerial *fonaSerial = &fonaSS;

// Hardware serial is also possible!
//HardwareSerial *fonaSerial = &Serial1;

// For ESP32 hardware serial use these lines instead
//#include <HardwareSerial.h>
//HardwareSerial fonaSS(1);

// Use this for 2G modules
#ifdef SIMCOM_2G
  Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
  
// Use this one for 3G modules
#elif defined(SIMCOM_3G)
  Adafruit_FONA_3G fona = Adafruit_FONA_3G(FONA_RST);
  
// Use this one for LTE CAT-M/NB-IoT modules (like SIM7000)
// Notice how we don't include the reset pin because it's reserved for emergencies on the LTE module!
#elif defined(SIMCOM_7000) || defined(SIMCOM_7500)
  Adafruit_FONA_LTE fona = Adafruit_FONA_LTE();
#endif

  /************************* MQTT SETUP *********************************/
  // MQTT setup (if you're using it, that is)
  // For Adafruit IO:
  #define AIO_SERVER      "io.adafruit.com"
  #define AIO_SERVERPORT  1883
  #define AIO_USERNAME    "YOUR_AIO_USERNAME"
  #define AIO_KEY         "YOUR_AIO_KEY"
  
  // How many transmission failures in a row we're OK with before reset
  uint8_t txfailures = 0;  
  
 
/****************************** OTHER STUFF ***************************************/
// For sleeping the AVR
#include <avr/sleep.h>
#include <avr/power.h>

// The following line is used for applications that require repeated data posting, like GPS trackers
// Comment it out if you only want it to post once, not repeatedly every so often
#define samplingRate 10 // The time in between posts, in seconds

// The following line can be used to turn off the shield after posting data. This
// could be useful for saving energy for sparse readings but keep in mind that it
// will take longer to get a fix on location after turning back on than if it had
// already been on. Comment out to leave the shield on after it posts data.
//#define turnOffShield // Turn off shield after posting data

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);
char imei[16] = {0}; // Use this for device ID
uint8_t type;
//char PIN[5] = "1234"; // SIM card PIN

void setup() {
  Serial.begin(9600);
  Serial.println(F("*** SIMCom Module IoT Example ***"));
  
  pinMode(FONA_RST, OUTPUT);
  digitalWrite(FONA_RST, HIGH); // Default state

  pinMode(FONA_PWRKEY, OUTPUT);
  powerOn(); // Power on the module
  moduleSetup(); // Establishes first-time serial comm and prints IMEI

  // Unlock SIM card if needed
  // Remember to uncomment the "PIN" variable definition above
  /*
  if (!fona.unlockSIM(PIN)) {
    Serial.println(F("Failed to unlock SIM card"));
  }
  */

  // Set modem to full functionality
  fona.setFunctionality(1); // AT+CFUN=1

  // Configure a GPRS APN, username, and password.
  // You might need to do this to access your network's GPRS/data
  // network.  Contact your provider for the exact APN, username,
  // and password values.  Username and password are optional and
  // can be removed, but APN is required.
  //fona.setNetworkSettings(F("your APN"), F("your username"), F("your password"));
  //fona.setNetworkSettings(F("m2m.com.attz")); // For AT&T IoT SIM card
  //fona.setNetworkSettings(F("telstra.internet")); // For Telstra (Australia) SIM card - CAT-M1 (Band 28)
  fona.setNetworkSettings(F("hologram")); // For Hologram SIM card

  // Perform first-time GPS/GPRS setup if the shield is going to remain on,
  // otherwise these won't be enabled in loop() and it won't work!
#ifndef turnOffShield
  // Enable GPS

  #if !defined(SIMCOM_3G) && !defined(SIMCOM_7500)
    // Disable GPRS just to make sure it was actually off so that we can turn it on
    if (!fona.enableGPRS(false)) Serial.println(F("Failed to disable GPRS!"));
    
    // Turn on GPRS
    while (!fona.enableGPRS(true)) {
      Serial.println(F("Failed to enable GPRS, retrying..."));
      delay(2000); // Retry every 2s
    }
    Serial.println(F("Enabled GPRS!"));
  #endif
#endif

//  while (!netStatus()) {
//    Serial.println(F("Failed to connect to cell network, retrying..."));
//    delay(2000); // Retry every 2s
//  }
//  Serial.println(F("Connected to cell network!"));
}

void loop() {
  while (!netStatus()) {
    Serial.println(F("Failed to connect to cell network, retrying..."));
    delay(2000); // Retry every 2s
  }
  Serial.println(F("Connected to cell network!"));


  
  Serial.println(F("DONE"));
  while(1);
}

// Power on the module
void powerOn() {
  digitalWrite(FONA_PWRKEY, LOW);
  // See spec sheets for your particular module
  #if defined(SIMCOM_2G)
    delay(1050);
  #elif defined(SIMCOM_3G)
    delay(180); // For SIM5320
  #elif defined(SIMCOM_7000)
    delay(100); // For SIM7000
  #elif defined(SIMCOM_7500)
    delay(500); // For SIM7500
  #endif
  
  digitalWrite(FONA_PWRKEY, HIGH);
}

void moduleSetup() {
  // SIM7000 takes about 3s to turn on but SIM7500 takes about 15s
  // Press reset button if the module is still turning on and the board doesn't find it.
  // When the module is on it should communicate right after pressing reset
  fonaSS.begin(115200); // Default SIM7000 shield baud rate
  
  Serial.println(F("Configuring to 9600 baud"));
  while(1){
    fonaSS.println("AT+IPR=9600"); // Set baud rate
    delay(100); // Short pause to let the command run
    fonaSS.begin(9600);
    if (! fona.begin(fonaSS)) {
      Serial.println(F("Couldn't find FONA"));// Don't proceed if it couldn't find the device
    }
    else{
      break;
    }
  }

  // The commented block of code below is an alternative that will find the module at 115200
  // Then switch it to 9600 without having to wait for the module to turn on and manually
  // press the reset button in order to establish communication. However, once the baud is set
  // this method will be much slower.
  /*
  fonaSerial->begin(115200); // Default LTE shield baud rate
  fona.begin(*fonaSerial); // Don't use if statement because an OK reply could be sent incorrectly at 115200 baud

  Serial.println(F("Configuring to 9600 baud"));
  fona.setBaudrate(9600); // Set to 9600 baud
  fonaSerial->begin(9600);
  if (!fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find modem"));
    while(1); // Don't proceed if it couldn't find the device
  }
  */

  type = fona.type();
  Serial.println(F("FONA is OK"));
  Serial.print(F("Found "));
  switch (type) {
    case SIM800L:
      Serial.println(F("SIM800L")); break;
    case SIM800H:
      Serial.println(F("SIM800H")); break;
    case SIM808_V1:
      Serial.println(F("SIM808 (v1)")); break;
    case SIM808_V2:
      Serial.println(F("SIM808 (v2)")); break;
    case SIM5320A:
      Serial.println(F("SIM5320A (American)")); break;
    case SIM5320E:
      Serial.println(F("SIM5320E (European)")); break;
    case SIM7000A:
      Serial.println(F("SIM7000A (American)")); break;
    case SIM7000C:
      Serial.println(F("SIM7000C (Chinese)")); break;
    case SIM7000E:
      Serial.println(F("SIM7000E (European)")); break;
    case SIM7000G:
      Serial.println(F("SIM7000G (Global)")); break;
    case SIM7500A:
      Serial.println(F("SIM7500A (American)")); break;
    case SIM7500E:
      Serial.println(F("SIM7500E (European)")); break;
    default:
      Serial.println(F("???")); break;
  }
  
  // Print module IMEI number.
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); Serial.println(imei);
  }
}

bool netStatus() {
  int n = fona.getNetworkStatus();
  
  Serial.print(F("Network status ")); Serial.print(n); Serial.print(F(": "));
  if (n == 0) Serial.println(F("Not registered"));
  if (n == 1) Serial.println(F("Registered (home)"));
  if (n == 2) Serial.println(F("Not registered (searching)"));
  if (n == 3) Serial.println(F("Denied"));
  if (n == 4) Serial.println(F("Unknown"));
  if (n == 5) Serial.println(F("Registered roaming"));

  if (!(n == 1 || n == 5)) return false;
  else return true;
}

// Turn off the MCU completely. Can only wake up from RESET button
// However, this can be altered to wake up via a pin change interrupt
void MCU_powerDown() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  ADCSRA = 0; // Turn off ADC
  power_all_disable ();  // Power off ADC, Timer 0 and 1, serial interface
  sleep_enable();
  sleep_cpu();
}
