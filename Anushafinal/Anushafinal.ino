#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Stepper.h>
//Our HTML webpage contents in program memory
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<center>
<h1>LBRCE</h1>
<h1>PBL</h1><br>
<h1>ENHANCING MOBILE SECURITY WITH AN AUTOMATED SIM SLOT EJECTION SYSTEM AND AUTHENTICATION MECHANISM</h1><br>
<h1>Click to <a href="Clockwise" target="myIframe">Eject</a><h1>
<h1>Click to <a href="Anti-Clockwise" target="myIframe">Admit</a><h1><br>
<h1>BY<h1>
<h1>Arjun & Eswar<h1>
<h1>Motor Running Status:<iframe name="myIframe" width="100" height="25" frameBorder="0"><h1><br>
</center>
</body>
</html>
)=====";
//---------------------------------------------------------------
//On board LED Connected to GPIO2
#define m11 5
#define m12 4
#define m21 0
#define m22 2
 
//SSID and Password of your WiFi router
const char* ssid = "project";
const char* password = "12345678910";
//Declare a global object variable from the ESP8266WebServer class.
ESP8266WebServer server(80); //Server on port 80
const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
Stepper myStepper(stepsPerRevolution, 5, 4, 0, 2);

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 Serial.println("You called root page");
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
 
void handleLEDon() { 
// Serial.println("LED on page");
 Serial.println("Clockwise Direction");
  myStepper.step(stepsPerRevolution);
  delay(500);
 server.send(200, "text/html", "Clockwise Direction"); //Send ADC value only to client ajax request
}
 
void handleLEDoff() { 
 //Serial.println("LED off page");
   Serial.println("Anti-Clockwise Direction");
  myStepper.step(-stepsPerRevolution);
  delay(500);
 server.send(200, "text/html", "Anti-Clockwise Direction"); //Send ADC value only to client ajax request
}
//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  myStepper.setSpeed(60);
  Serial.begin(115200);
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  //Onboard LED port Direction output
  pinMode(m11,OUTPUT);
  pinMode(m12,OUTPUT);
  pinMode(m21,OUTPUT);
  pinMode(m22,OUTPUT);
  //Power on LED state off
//  digitalWrite(LED,HIGH);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/Clockwise", handleLEDon); //as Per  <a href="ledOn">, Subroutine to be called
  server.on("/Anti-Clockwise", handleLEDoff);
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}