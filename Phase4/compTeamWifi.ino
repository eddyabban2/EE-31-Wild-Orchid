#include <SPI.h>
#include <WiFiNINA.h>
//#include "arduino_secrets.h"

char ssid[] = "junior";        // your network SSID (name)
char pass[] = "designdesign";  // your network password
char server[] = "ee31.ece.tufts.edu";  // server
int portNumber = 80;

int status = WL_IDLE_STATUS;

String our_id = "F392FC86D8D7";
String companion_team_id = "F79721857DC5";


WiFiClient client;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {} // wait for serial port to connect. Needed for native USB port only

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
  String getRoute = "GET /" + our_id + "/" + companion_team_id + " HTTP/1.1";
  String postRoute = "POST /" + our_id + "/" + companion_team_id + " HTTP/1.1";
  POST(postRoute, "Hello_from_Wild_Orchid");
  Serial.print("message gotten from get: ");
  Serial.println(GET(getRoute));
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
  }

    // do nothing forevermore:
  Serial.println("message sent turning off");
  while (true);
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

void POST(String theRoute, String message) {
  if(client.connect(server, portNumber))
  {
    client.println(theRoute);
    client.print("Host: ");
    client.println(server);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    int postBodyLength = message.length();
    client.println(postBodyLength);
    client.println();
    client.print(message);
  }
}

String GET(String theRoute) {
  if (client.connect(server, portNumber)) {
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
    delay(500);
    String result = "";
    while (client.available()) {
      char message = client.read();
      result += message;
    }
    return process_message(result);
  }
}

String process_message(String message)
{
  int index = 0;
  int count_new_line = 0;
  while(index < message.length() and count_new_line < 3)
  {
    if(message[index] == '&')
    {
      count_new_line += 1;
    }
    index++;
  }
  index += 8;
  String output_string = "";
  while(index < message.length())
  {
    output_string += message[index];
    index++;
  }
  return output_string;
}


