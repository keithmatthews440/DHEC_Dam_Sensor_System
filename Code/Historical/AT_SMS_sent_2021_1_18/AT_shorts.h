/*
 * This is the AT-command 'library'
 * The hope is to migrate this to an arduino libray afterwards, but to for now keep it open so it can be edited, tweeked, and streamlined
 * I apologize for the lack of comments (a very bad habit)
 */

String Read(){ //Read the output from the GSM chip
  String out;
  char r;
  while(fonaSS.available()){
    r = char(fonaSS.read());
    out = out + String(r);
    delay(50);
  }
  Serial.print("<--");
  Serial.println(out);
  return out;
}

void Send(String strIN, int wait= 1000){ //Send 
  fonaSS.println(strIN);
  Serial.println("-->"+strIN);
  delay(wait);
  Read();
}

void Start(){ //The start up of the GSM shield
  fonaSS.begin(115200);

  Serial.println(F("Configuring to 9600 baud"));
  fonaSS.println("AT+IPR=9600"); // Set baud rate
  delay(100); // Short pause to let the command run
  fonaSS.begin(9600);
  
  pinMode(FONA_RST, OUTPUT);
  pinMode(FONA_PWRKEY, OUTPUT);
  
  digitalWrite(FONA_RST, HIGH); // Default state  
  digitalWrite(FONA_PWRKEY, LOW);
  delay(100); // For SIM7000
  digitalWrite(FONA_PWRKEY, HIGH); 
  delay(1000);
  Send("AT+CSCS=\"UCS2\"\r");
  Send("AT");
  Send("ATE1");
  Send("ATI");
  Send("AT+CPMS=\"SM\",\"SM\",\"SM\"");
  Send("AT+GSN");
  Send("AT+CFUN=1"); 
  Send("AT+CGDCONT=1,\"IP\",\"hologram\"");
  Send("AT+CGREG?");
  Send("AT+CMGF=1");
  delay(3000);
}

void sendHex(byte hexIN[]){ //Send data to GSM chip as a hexidecimal code
  fonaSS.write(hexIN, sizeof(hexIN));
  Read();
}

void waitReply(int wait = 120){ //wait for reply specifically for long waits. Variable wait in seconds
  int i=0;
  while(1){
    if(fonaSS.available()){
      Read();
      break;
    }
    if(i>wait){//wait two minutes
      Serial.print("timed out after ");
      Serial.print(wait);
      Serial.println("seconds");
      break;
    }
    delay(1000);
    i++;
  }
}

void sendText(String number, String mess){//Send a text message
  byte ctrlZ = 0x1A;
  Send("AT+CMGS=?");
  Serial.println("Sending Text"); //fonaSS.print("AT+CMGS=\"+18645016496\"CR\"Hello World\"SUB");//fonaSS.println(74, HEX);
  fonaSS.print("AT+CMGS=\""+number+"\""); //ln represents CR
  delay(4000);
  fonaSS.print(mess);
  //fonaSS.write(mess, sizeof(mess));
  sendHex(ctrlZ);
  waitReply();
}

void checkSerial(){  //Check to see if there is anything in the arduino serial (check for user inputs, for debugging).
  if(Serial.available()){
    String input = "";
    String byteRead = Serial.readString();
    input = input + byteRead;
    delay(100);
    if(!Serial.available()){
      Send(input);
      input = "";
    }
  }
}
