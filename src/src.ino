// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

// Load servo library
#include <Servo.h>

// Replace with your network credentials
const char* ssid     = "SSID";
const char* password = "PASSWORD";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Servo settings
int angle = 0;
String lastSort = "none sorted yet";
Servo myservo;  // create servo object to control a servo

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  myservo.attach(D1);  // attaches the servo to the servo object
  myservo.write(angle);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp8266.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!MDNS.begin("sorter")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) { delay(1000); }
  }
  Serial.println("mDNS responder started");
  
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

    // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);
}

void loop() {
  MDNS.update();
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /home") >= 0) {
              angle = 0;
              lastSort = "home";
            } else if (header.indexOf("GET /sort?color=red") >= 0) {
              angle = 40;
              lastSort = "red";
            } else if (header.indexOf("GET /sort?color=green") >= 0) {
              angle = 60;
              lastSort = "green";
            } else if (header.indexOf("GET /sort?color=blue") >= 0) {
              angle = 80;
              lastSort = "blue";
            } else if (header.indexOf("GET /sort?color=white") >= 0) {
              angle = 100;
              lastSort = "white";
            } else if (header.indexOf("GET /sort?color=black") >= 0) {
              angle = 120;
              lastSort = "black";
            } else if (header.indexOf("GET /sort?color=multi") >= 0) {
              angle = 140;
              lastSort = "multi";
            } else if (header.indexOf("GET /sort?color=colorless") >= 0) {
              angle = 160;
              lastSort = "colorless";
            } else if (header.indexOf("GET /sort") >= 0) { // For all other calls, treat it as other
              angle = 180;
              lastSort = "other";
            }
            myservo.write(angle);

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            client.println("<link rel=\"stylesheet\" href=\"https://cdn.simplecss.org/simple.css\">");
            client.println("</head>");

            // Web Page Heading
            client.println("<body><header><h1>MTG Card sorter</h1></header>");
            client.print("<p> Let Delver Lens send GET requests to: <code>http://sorter.local/sort?color=$color</code></p>");

            // Display current state
            client.println("<p class=\"notice\">Last sorted color: " + lastSort + "</p>");
            
            client.println("<h2>Manual sort</h2>");
            client.println("<a href=\"/home\" class=\"button\">Home</a>");
            client.println("<a href=\"/sort?color=red\" class=\"button\">Red</a>");
            client.println("<a href=\"/sort?color=green\" class=\"button\">Green</a>");
            client.println("<a href=\"/sort?color=blue\" class=\"button\">Blue</a>");
            client.println("<a href=\"/sort?color=white\" class=\"button\">White</a>");
            client.println("<a href=\"/sort?color=black\" class=\"button\">Black</a>");
            client.println("<a href=\"/sort?color=multi\" class=\"button\">Multicolor</a>");
            client.println("<a href=\"/sort?color=colorless\" class=\"button\">Colorless</button></a>");
            client.println("<a href=\"/sort?color=other\" class=\"button\">Other</a>");


            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
