int8_t answer;
int onModulePin= 2;
char aux_str[129];

//Write here you server and account data
const char smtp_server[ ] = "mail.smtp2go.com";      // SMTP server
const char smtp_user_name[ ] = "finalprojectgt1@gmail.com";   // SMTP user name
const char smtp_password[ ] = "***********";    // SMTP password
const char smtp_port[ ] = "2525";              // SMTP server port

//Change here your data
const char pin[]="****";
const char phone_number[]="0279804892";                 //phone number to which message or voice call is to be originated to
const char phone_number1[]="0504972328";                 //phone number to which message or voice call is to be originated to
const char sms_text[]="Hello dear one, there is an intruder in your house. Please call the police as soon as possible.";     //message to be sent as the short message. 

//Write here you SIM card data 
const char pin_number[] = "****";
const char apn[] = "internet";
const char user_name[] = "Airtelinternet";
const char password[] = "*********";

//Write here your information about sender, direcctions and names
const char sender_address[ ] = "finalprojectgt1@gmail.com";    // Sender address
const char sender_name[ ] = "Final ProjectGT1";       // Sender name

const char to_address[ ] = "elvispluwn1@gmail.com";        // Recipient address
const char to_name[ ] = "Elvis";           // Recipient name

//Write here the subject and body of the email
char subject[ ] = "INTRUDER ALERT";
const char body[ ] = "Hello dear one, there is an intruder in your house. Please call the police as soon as possible.";   //message to be sent as email.

//Variables initialization and decleration
int PIRpin = 3;
int blueled = 4;
int greenled = 5;
int redled = 6;
int Buzzerpin = 9;




void setup(){
  pinMode(onModulePin, OUTPUT);
  Serial.begin(115200);    
   // make the PIRpin's pin an input:
  pinMode(PIRpin, INPUT);
  pinMode(blueled, OUTPUT);
  pinMode(greenled, OUTPUT);
  pinMode(redled, OUTPUT);
  pinMode(Buzzerpin, OUTPUT);
 
}


void loop(){
    delay(1000);
    digitalWrite(blueled, HIGH);
    digitalWrite(greenled, LOW);
    digitalWrite(redled, LOW);
    digitalWrite(Buzzerpin, LOW);
    // read the input pin:
    int PIRstate = digitalRead(PIRpin);
    // print out the state of the button:
    Serial.println(PIRstate);
    delay(100); 
    digitalWrite(blueled, LOW);
    
    while(PIRstate == HIGH){
     int PIRstate = digitalRead(PIRpin);
    // print out the state of the button:
    Serial.println(PIRstate);
    delay(200);
    digitalWrite(blueled, LOW);
    digitalWrite(greenled, LOW);
    digitalWrite(redled, HIGH);
    digitalWrite(Buzzerpin, HIGH);
    sendsms();
    delay(10000);
    sendsms1();
    delay(10000);
    makevoicecall();
    delay(10000);
    makevoicecall1();
    delay(10000);
    sendemail();
    delay(20000);
    PIRstate = digitalRead(PIRpin);
    Serial.println(PIRstate);
    delay(200);
    Serial.println(PIRstate);
    delay(200);
    
    if(PIRstate == LOW){
    digitalWrite(redled, LOW);
    digitalWrite(Buzzerpin, LOW);
    break;
    }
  }  
  
}





///////////////////////////////////////////// FUNCTION USED /////////////////////////////////////////
void power_on(){

  uint8_t answer=0;

  // checks if the module is started
  answer = sendATcommand("AT", "OK", 2000);
  if (answer == 0)
  {
    // power on pulse
    digitalWrite(onModulePin,HIGH);
    delay(3000);
    digitalWrite(onModulePin,LOW);

    // waits for an answer from the module
    while(answer == 0){     // Send AT every two seconds and wait for the answer
      answer = sendATcommand("AT", "OK", 2000);    
    }
  }

}

int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout){

  uint8_t x=0,  answer=0;
  char response[100];
  unsigned long previous;

  memset(response, '\0', 100);    // Initialice the string

  delay(100);

  while( Serial.available() > 0) Serial.read();    // Clean the input buffer

  Serial.println(ATcommand);    // Send the AT command 


    x = 0;
  previous = millis();

  // this loop waits for the answer
  do{
    // if there are data in the UART input buffer, reads it and checks for the answer
    if(Serial.available() != 0){    
      response[x] = Serial.read();
      x++;
      // check if the desired answer is in the response of the module
      if (strstr(response, expected_answer) != NULL)    
      {
        answer = 1;
      }
    }
    // Waits for the answer with time out
  }
  while((answer == 0) && ((millis() - previous) < timeout));    

  return answer;
}
 
void sendsms(){
    Serial.println("Starting Shield to send sms...");
  power_on();

  delay(3000);

  //sets the PIN code
  sprintf(aux_str, "AT+CPIN=%s", pin);
  sendATcommand(aux_str, "OK", 2000);

  delay(3000);

  Serial.println("Connecting to the network...");

  while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || 
    sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );

  Serial.print("Setting SMS mode...");
  digitalWrite(greenled, HIGH);
  digitalWrite(redled, LOW);
  sendATcommand("AT+CMGF=1", "OK", 1000);    // sets the SMS mode to text
  Serial.println("Sending SMS");
    
  sprintf(aux_str,"AT+CMGS=\"%s\"", phone_number);
  answer = sendATcommand(aux_str, ">", 2000);    // send the SMS number
  if (answer == 1)
  {
    Serial.println(sms_text);
    Serial.write(0x1A);
    answer = sendATcommand("", "OK", 20000);
    if (answer == 1)
    {
      Serial.print("Sent ");
      digitalWrite(greenled, LOW);
      digitalWrite(redled, HIGH);    
    }
    else
    {
      Serial.print("error ");
    }
  }
  else
  {
    Serial.print("error ");
    Serial.println(answer, DEC);
  }

}

void sendsms1(){
    Serial.println("Starting Shield to send sms...");
  power_on();

  delay(3000);

  //sets the PIN code
  sprintf(aux_str, "AT+CPIN=%s", pin);
  sendATcommand(aux_str, "OK", 2000);

  delay(3000);

  Serial.println("Connecting to the network...");

  while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || 
    sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );

  Serial.print("Setting SMS mode...");
  digitalWrite(greenled, HIGH);
  digitalWrite(redled, LOW);
  sendATcommand("AT+CMGF=1", "OK", 1000);    // sets the SMS mode to text
  Serial.println("Sending SMS");
    
  sprintf(aux_str,"AT+CMGS=\"%s\"", phone_number1);
  answer = sendATcommand(aux_str, ">", 2000);    // send the SMS number
  if (answer == 1)
  {
    Serial.println(sms_text);
    Serial.write(0x1A);
    answer = sendATcommand("", "OK", 20000);
    if (answer == 1)
    {
      Serial.print("Sent ");
      digitalWrite(greenled, LOW);
      digitalWrite(redled, HIGH);    
    }
    else
    {
      Serial.print("error ");
    }
  }
  else
  {
    Serial.print("error ");
    Serial.println(answer, DEC);
  }

}

 void makevoicecall(){
  
     Serial.println("Starting Shield to make a voice call...");
  power_on();

  delay(3000);

  //sets the PIN code
  sprintf(aux_str, "AT+CPIN=%s", pin);
  sendATcommand(aux_str, "OK", 2000);

  delay(3000);

  Serial.println("Connecting to the network...");

  //Enables the use of command ATH
  sendATcommand("AT+CVHU=0", "OK", 10000);

  while ( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) ||
           sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );
  
  Serial.print("Calling to ");
  digitalWrite(greenled, HIGH);
  digitalWrite(redled, LOW);
  Serial.print(phone_number);


  //Make the phone call
  sprintf(aux_str, "ATD%s;", phone_number);
  sendATcommand(aux_str, "OK", 10000);


  delay(25000);  //after this time elapse .....
  // disconnects the existing call
  digitalWrite(greenled, LOW);
  digitalWrite(redled, HIGH);
  Serial.println("ATH");            
  Serial.println("Call disconnected");
 }   

void makevoicecall1(){
  
     Serial.println("Starting Shield to make a voice call...");
  power_on();

  delay(3000);

  //sets the PIN code
  sprintf(aux_str, "AT+CPIN=%s", pin);
  sendATcommand(aux_str, "OK", 2000);

  delay(3000);

  Serial.println("Connecting to the network...");

  //Enables the use of command ATH
  sendATcommand("AT+CVHU=0", "OK", 10000);

  while ( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) ||
           sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );
  
  Serial.print("Calling to ");
  digitalWrite(greenled, HIGH);
  digitalWrite(redled, LOW);
  Serial.print(phone_number);


  //Make the phone call
  sprintf(aux_str, "ATD%s;", phone_number1);
  sendATcommand(aux_str, "OK", 10000);


  delay(25000);  //after this time elapse .....
  // disconnects the existing call
  digitalWrite(greenled, LOW);
  digitalWrite(redled, HIGH);
  Serial.println("ATH");            
  Serial.println("Call disconnected");
 }   

 
 
 void sendemail(){
  
     Serial.println("Starting Shield to send an email...");
  power_on();

  delay(3000);

  //sets the PIN code
  digitalWrite(greenled, HIGH);
  digitalWrite(redled, LOW);
  sprintf(aux_str, "AT+CPIN=%s", pin_number);
  sendATcommand(aux_str, "OK", 2000);

  delay(3000);

  while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || 
    sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );

  // sets the SMTP server and port
  sprintf(aux_str, "AT+SMTPSRV=\"%s\",%s", smtp_server, smtp_port);
  sendATcommand(aux_str, "OK", 2000);

  // sets user name and password
  sprintf(aux_str, "AT+SMTPAUTH=1,\"%s\",\"%s\"", smtp_user_name, smtp_password);
  sendATcommand(aux_str, "OK", 2000);

  // sets sender adress and name
  sprintf(aux_str, "AT+SMTPFROM=\"%s\",\"%s\"", sender_address, sender_name);
  sendATcommand(aux_str, "OK", 2000);

  // sets sender adress and name
  sprintf(aux_str, "AT+SMTPRCPT=1,0,\"%s\",\"%s\"", to_address, to_name);
  sendATcommand(aux_str, "OK", 2000);

  // subjet of the email
  sprintf(aux_str, "AT+SMTPSUB=\"%s\"", subject);
  sendATcommand(aux_str, "OK", 2000);

  // body of the email
  sprintf(aux_str, "AT+SMTPBODY=\"%s\"", body);
  sendATcommand(aux_str, "OK", 2000);


  // sets APN, user name and password
  sprintf(aux_str, "AT+CGSOCKCONT=1,\"IP\",\"%s\"", apn);
  sendATcommand(aux_str, "OK", 2000);

  sprintf(aux_str, "AT+CSOCKAUTH=1,1,\"%s\",\"%s\"", user_name, password);
  sendATcommand(aux_str, "OK", 2000);

  delay(2000);

  Serial.println("Sending email..."); 
  // sends the email and waits the answer of the module
  answer = sendATcommand("AT+SMTPSEND", "+SMTP: SUCCESS", 60000);
  if (answer == 1)
  {
    digitalWrite(greenled, LOW);
     digitalWrite(redled, HIGH);
    Serial.println("Done!");    
  }
  else
  {
    Serial.println("Error");         
  }
 }
