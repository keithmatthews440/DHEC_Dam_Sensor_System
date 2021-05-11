#include <string.h>

#define FONA_PWRKEY 6
#define FONA_RST 7
#define FONA_TX 10 // Microcontroller RX
#define FONA_RX 11 // Microcontroller TX

#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

char imei[16] = {0};
int i = 0;
int Break = 0;
char input, output;

void Send(char mess, int wait =1000);

void setup() {
  Serial.begin(9600);
  Send("AT");
  Setup();
  Serial.println("End");
}

void loop() {
  if(Serial.available()){
    char byteRead = Serial.read();
    input = input + byteRead;
    delay(100);
    if(!Serial.available()){
      Send(input);
      input = "";
    }
  }
  sendText();
  //GPS_data();
  while(1);
}

void Send(char mess, int wait =1000){
  char out;
  char r;
  fonaSS.println(mess);
  Serial.print("-->");
  Serial.println(mess);
  delay(wait);
  i = 0; 
  while(fonaSS.available()){
    r = char(fonaSS.read());
    if(i>1){
      out = out + char(r);
    }
    i=i+1;
    delay(50);
  }
  Serial.print("<--");
  output = out;
  Serial.println(out);
}

void check(char mess){
  i = 0;
  while(1){
    Send(mess);
    delay(500);
    String Sout = String(output);
    //Serial.println("<"+output.substring(0,2)+">");
    if(Sout.substring(0,2) == "OK"){//output.indexOf("OK")){
      break;
    }
    i=i+1;
    if(i>=10){
      Serial.println("Error, breaking loop");
      Break = 1;
      break;
    }
    Serial.println("Trying again...");
    delay(2000);
    
  }
  if(Break == 1){
    Break = 0;
    setup();
  }
}

void Setup(){
  pinMode(FONA_RST, OUTPUT);
  pinMode(FONA_PWRKEY, OUTPUT);
  digitalWrite(FONA_RST, HIGH); // Default state  
  digitalWrite(FONA_PWRKEY, LOW);
  delay(100); // For SIM7000
  digitalWrite(FONA_PWRKEY, HIGH); 
  
  fonaSS.begin(115200);
  Serial.println(F("Configuring to 9600 baud"));
  fonaSS.println("AT+IPR=9600"); // Set baud rate
  delay(100); // Short pause to let the command run
  fonaSS.begin(9600);

  delay(1000);
  Send(char("AT+CSCS=\"UCS2\"\r"));
  Send("AT");
  Send("ATE0");
  Send("ATI");
  Send("AT+CPMS=\"SM\",\"SM\",\"SM\"");
  Send("AT+GSN");
  Send("AT+CFUN=1"); 
  Send("AT+CGDCONT=1,\"IP\",\"hologram\"");
  Send("AT+CGREG?");
}

void GPS_data(){
  check("AT+CGNSPWR=1");
  delay(1000);
  Send("AT+CGNSINF");
  delay(500);
  //Serial.println("<"+output.substring(12,13)+">");
  String Sout = String(output);
  while(Sout.substring(12,13)!="1"){
    Send("AT+CGNSINF");
    //Serial.println("<"+output.substring(12,13)+">");
    Serial.println("Waiting for GPS");
    delay(5000);
  }
  Serial.println("Location Locked");
//  char *token = strtok(char(output), ",");
//   
//   /* walk through other tokens */
//   while( token != NULL ) {
//      Serial.println(token);
//      token = strtok(NULL, ",");
//   }
  
}

void sendText(){
  Send("AT+CMGS=\"18645016496\" <CR> \"Hello World\""); //(message should be a Char) ctrl-Z
  Send(char("SUB"));
}
