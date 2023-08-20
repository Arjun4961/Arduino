#include <ESP8266WiFi.h>
#include <Stepper.h>

// Enter your WiFi network name and WiFi Password
const char* ssid = "IQOO Z5";
const char* password = "Anusha123";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

Stepper my_Stepper(200, 12,13,14,15);
bool Right = false;
bool Left = false;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void Stepper1(int Direction, int Rotation) {
  for (int i = 0; i < Rotation; i++) {
    my_Stepper.step(Direction * 200);
  }
}

void loop() {
  WiFiClient client = server.available(); // Listen for incoming clients

  if (client) { // If a new client connects,
    Serial.println("New Client."); // Print a message out in the serial port
    String currentLine = ""; // Make a String to hold incoming data from the client
    while (client.connected()) { // Loop while the client's connected
      if (client.available()) { // If there's bytes to read from the client
        char c = client.read(); // Read a byte
        header += c;
        if (c == '\n') { // If the byte is a newline character
          // If the current line is blank, you got two newline characters in a row.
          // That's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // Stepper motor control logic
            if (header.indexOf("GET /stepper/right") >= 0) {
              Serial.println("Stepper motor: Right turn");
              Right = true;
            } else if (header.indexOf("GET /stepper/left") >= 0) {
              Serial.println("Stepper motor: Left turn");
              Left = true;
            }

            // Send the HTML response
            // ...

            // Clear the header variable
            header = "";
            // Break out of the while loop
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    // Clear the header variable
    header = "";

    // Execute stepper motor actions if necessary
    if (Right) {
      Stepper1(1, 10); // Rotate 10 times 360 degrees right
      Right = false;
    }

    if (Left) {
      Stepper1(-1, 10); // Rotate 10 times 360 degrees left
      Left = false;
    }

    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
