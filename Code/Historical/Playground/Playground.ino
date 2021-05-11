#define BLYNK_PRINT Serial
#include "ESP8266.h"

#define SSID        "ITEAD"
#define PASSWORD    "12345678"

ESP8266 wifi(Serial1);

void setup(void)
{
    Serial.begin(9600);
    Serial.print("setup begin\r\n");

    Serial.print("FW Version: ");
    Serial.println(wifi.getVersion().c_str());
    
    
    if (wifi.setOprToStation()) {
        Serial.print("to station ok\r\n");
    } else {
        Serial.print("to station err\r\n");
    }

    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("Join AP success\r\n");
        Serial.print("IP: ");       
        Serial.println(wifi.getLocalIP().c_str());
    } else {
        Serial.print("Join AP failure\r\n");
    }
    
    Serial.print("setup end\r\n");
}

void loop(void)
{
}
/*#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>
#define ESP8266_BAUD 9600

char auth[] = "6bae2d0e7bfa4b6096feeaffa2030e4f";
char ssid[] = "Verizon-MiFi6620L-D829"; //You can replace the wifi name to your wifi 

char pass[] = "83cc19c4";  //Type password of your wifi.

SoftwareSerial EspSerial(2, 3); // RX, TX
WidgetLCD lcd(V0);

ESP8266 wifi(&EspSerial);

void setup()
{

  Serial.begin(9600);
  EspSerial.begin(ESP8266_BAUD);
  Blynk.begin(auth, wifi, ssid, pass);
   lcd.clear();
 lcd.print(1, 1, "IoT");
}

void loop()
{
  Blynk.run();
}


/*include <SoftwareSerial.h>

SoftwareSerial ESPserial(2, 3); // RX | TX

void setup()

{

Serial.begin(115200); // communication with the host computer

//while (!Serial) { ; }

// Start the software serial for communication with the ESP8266

ESPserial.begin(115200);

Serial.println("");

Serial.println("Remember to to set Both NL & CR in the serial monitor.");

Serial.println("Ready");

Serial.println("");

}

void loop()

{

// listen for communication from the ESP8266 and then write it to the serial monitor

if ( ESPserial.available() ) { Serial.write( ESPserial.read() ); }

// listen for user input and send it to the ESP8266

if ( Serial.available() ) { ESPserial.write( Serial.read() ); }

}*/
