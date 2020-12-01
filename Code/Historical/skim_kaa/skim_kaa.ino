#include <SPI.h>
#include <ArduinoMqttClient.h>//this lib(ArduinoMqttClient by Arduino) must be installed via Libarary manager
//#include <Arduino_MKRENV.h>//this lib(MKRENV by Arduino) must be installed via Libarary manager
#include <ArduinoJson.h>//this lib(Arduino_json by Arduino) must be installed via Libarary manager

const char* mqtt_server = "mqtt.cloud.kaaiot.com";
const String TOKEN = "";        // Endpoint token - you get (or specify) it during device provisioning
const String APP_VERSION = "";  // Application version - you specify it during device provisioning

const unsigned long fiveSeconds = 1 * 5 * 1000UL;
static unsigned long lastPublish = 0 - fiveSeconds;


MqttClient mqttClient(wifiClient);

String configurationUpdatesTopic = "kp1/" + APP_VERSION + "/cmx/" + TOKEN + "/config/json/status";
String configurationRequestTopic = "kp1/" + APP_VERSION + "/cmx/" + TOKEN + "/config/json/1";
String configurationResponceTopic = "kp1/" + APP_VERSION + "/cmx/" + TOKEN + "/config/json/1/status";
String metricsPublishTopic = "kp1/" + APP_VERSION + "/dcx/" + TOKEN + "/json";
  

void setup() {
  Serial.begin(115200);


 
}

void loop() {

  
  if (!mqttClient.connected()) {
    reconnect();
  }

  mqttClient.poll();

  unsigned long now = millis();
  if (now - lastPublish >= fiveSeconds) {
    lastPublish += fiveSeconds;

    //Getting data from the sensors
    DynamicJsonDocument telemetry(128);
    telemetry.createNestedObject();
    telemetry[0]["temperature"] = 101//ENV.readTemperature();
    telemetry[0]["humidity"]    = 696//ENV.readHumidity();
//    currentBrightness = ENV.readIlluminance();
//    telemetry[0]["illuminance"] = currentBrightness;
//    telemetry[0]["pressure"]    = ENV.readPressure();
//    telemetry[0]["uva"]         = ENV.readUVA();
//    telemetry[0]["uvb"]         = ENV.readUVB();
//    telemetry[0]["uvIndex"]     = ENV.readUVIndex();

    //Sending data to the Cloud
    String payload = telemetry.as<String>();
    mqttClient.beginMessage(metricsPublishTopic);
    mqttClient.print(payload.c_str());
    mqttClient.endMessage();
    
    Serial.println("Published on topic: " + metricsPublishTopic);
    Serial.println("payload:  " + payload);
  }   
}






void subscribeToConfiguration() {
  //Subscribe on configuration topic for receiving configuration updates 
  mqttClient.subscribe(configurationUpdatesTopic);
  Serial.println("Subscribed on topic: " + configurationUpdatesTopic);

  //Subscribe on configuration topic response
  mqttClient.subscribe(configurationResponceTopic);
  Serial.println("Subscribed on topic: " + configurationResponceTopic);

  // Sending configuration rquest to the server
  DynamicJsonDocument configRequest(128);
  configRequest["observe"] = true;
  String payload = configRequest.as<String>();
  Serial.println("Request configuration topic: " + configurationRequestTopic);
  Serial.print("Request configuration payload =  ");Serial.println(payload);   
  configRequest.clear();
  mqttClient.beginMessage(configurationRequestTopic);
  mqttClient.print(payload.c_str());
  mqttClient.endMessage();
 
}

void onMqttMessage(int messageSize) {
  // we received a message, print out the topic and contents
  String topic = mqttClient.messageTopic();
  Serial.println("Received a message on topic '");
  Serial.print(topic);
  Serial.print("', length ");
  Serial.print(messageSize);

  //Getting payload
  byte buffer[messageSize];
  mqttClient.read(buffer, messageSize);
  Serial.print("  Payload: ");
  for(int i = 0 ; i < messageSize ; i++){
    Serial.print((char)buffer[i]);
  }
  Serial.println();

  if(topic == configurationResponceTopic || topic == configurationUpdatesTopic){
    Serial.println("Configuration received");

    //Getting brightness parameter from payload
    DynamicJsonDocument newConfig(1023);
    deserializeJson(newConfig, buffer);
    int brightness = newConfig.as<JsonVariant>()["config"].as<JsonVariant>()["brightness"].as<int>();
    Serial.print(" New brightness: ");Serial.println(String(brightness));
    
    if(brightness < 1){
      userBrightness = -1; 
      ledBrightness = 0; 
    } else {
      userBrightness = brightness;
    }
  }
}
