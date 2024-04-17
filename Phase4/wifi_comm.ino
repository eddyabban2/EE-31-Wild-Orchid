#include <SPI.h>
#include <WiFiNINA.h>
//#include "arduino_secrets.h"

char ssid[] = "junior";        // your network SSID (name)
char pass[] = "designdesign";  // your network password
char server[] = "ee31.ece.tufts.edu";  // server
int portNumber = 80;

const char UID[] = "F392FC86D8D7";

char postBody[] = "data=eddywashere";

int status = WL_IDLE_STATUS;

const char getRoute[] = "GET /F392FC86D8D7/F392FC86D8D7 HTTP/1.1";
const char postRoute[] = "POST /F392FC86D8D7/F392FC86D8D7 HTTP/1.1";

WiFiClient client;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {} // wait for serial port to connect. Needed for native USB port only


  // check for the WiFi module:

  if (WiFi.status() == WL_NO_MODULE) {

    Serial.println("Communication with WiFi module failed!");
    while (true); //don't continue
  }

  String fv = WiFi.firmwareVersion();

  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(1000);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();
  Serial.println("\nStarting connection to server...");

  // POST(postRoute, postBody);
  GET(getRoute);


}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  
  while (client.available()) {
    char c = client.read();
    Serial.write(c); 
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void POST(const char theRoute[], char *bodyMessage) {
  if(client.connect(server, portNumber))
  {
    //Serial.println("Entered POST");
    client.println(theRoute);
    client.print("Host: ");
    client.println(server);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    int postBodyLength = strlen(bodyMessage);
    client.println(postBodyLength);
    client.println();
    client.print(bodyMessage);
  }
}

void GET(const char theRoute[]) {
  if (client.connect(server, portNumber)) {
    //Serial.println("Entered GET");
  // Make a HTTP GET request:
    client.println(theRoute);
    client.print("Host: ");
    client.println(server);
    //client.println("Connection: close");
    client.println();
    // get the message from the server
    // place in appropriate form and variable
    // if the server's disconnected, stop the client:
    if (!client.connected()) {
      client.stop();
    }
  }
}



