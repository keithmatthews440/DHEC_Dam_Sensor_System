String Read(){
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

void Send(String strIN, int wait= 1000){
  fonaSS.println(strIN);
  Serial.println("-->"+strIN);
  delay(wait);
  Read();
}

void Start(){
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
  Send("ATE0");
  Send("ATI");
  Send("AT+CPMS=\"SM\",\"SM\",\"SM\"");
  Send("AT+GSN");
  Send("AT+CFUN=1"); 
  Send("AT+CGDCONT=1,\"IP\",\"hologram\"");
  Send("AT+CGREG?");
  Send("AT+CMGF=1");
  delay(3000);
}

void SendHex(byte hexIN[]){
  fonaSS.write(hexIN, sizeof(hexIN));
  Read();
}

void SendText(String number, byte mess[]){
  Send("AT+CMGS=?");
  Serial.println("Sending Text"); //fonaSS.print("AT+CMGS=\"+18645016496\"CR\"Hello World\"SUB");//fonaSS.println(74, HEX);
  
  fonaSS.print("AT+CMGS=\""+number+"\""); //ln represents CR
  delay(4000);
  fonaSS.write(mess, sizeof(mess));
  delay(100);
  String out;
  while(fonaSS.available()){
    char r = char(fonaSS.read());
    out = out + String(r);
    delay(50);
  }
  Serial.print("<--");
  Serial.println(out);
}

void checkSerial(){  
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
