//Challenge 4 -- Night Conditions

#include <SPI.h>
#include <WiFiNINA.h>

#define redPin 10 // color detection blue and red
#define bluePin 11

#define batteryIndicatorLED 12 //sensor indication
#define dayIndicatorLED 13
#define IRDector A3

#define ambientLightPin A2 // detectors 
#define colorPin A1
#define batteryPin A0

#define leftFor 2 // motor control
#define leftRev 3
#define rightFor 7
#define rightRev 8

#define blueSig 0 // indicator LEDS 
#define redSig 1
#define greenSig 4
#define yellowSig 6

#define brakeLights 5 // Lights
#define headLights 9
#define leftTurnSignal A4
#define rightTurnSignal A5

char ssid[] = "junior";                // your network SSID (name)
char pass[] = "designdesign";          // your network password
char server[] = "ee31.ece.tufts.edu";  // server
int portNumber = 80;

int status = WL_IDLE_STATUS;
int battery_offset = 0;

String our_id = "F392FC86D8D7";
String companion_team_id = "F79721857DC5";

String getRoute = "GET /" + our_id + "/" + companion_team_id + " HTTP/1.1";
String postRoute = "POST /" + companion_team_id + "/" + our_id  + " HTTP/1.1";

WiFiClient client;

double turn_const = 1300;
enum Color { red,
             blue,
             yellow,
             black,
             none };

void setup() {
  Serial.begin(9600);
  Serial.println("staring up");
  pinMode(leftFor, OUTPUT);
  pinMode(leftRev, OUTPUT);
  pinMode(rightFor, OUTPUT);
  pinMode(rightRev, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(batteryIndicatorLED, OUTPUT);
  pinMode(dayIndicatorLED, OUTPUT);
  pinMode(ambientLightPin, INPUT);
  pinMode(batteryPin, INPUT);
  pinMode(colorPin, INPUT);
  pinMode(blueSig, OUTPUT);
  pinMode(redSig, OUTPUT);
  pinMode(greenSig, OUTPUT);
  pinMode(yellowSig, OUTPUT);
  pinMode(headLights, OUTPUT);
  pinMode(leftTurnSignal, OUTPUT);
  pinMode(rightTurnSignal, OUTPUT);
  digitalWrite(yellowSig, HIGH);
  setupWifi();
  digitalWrite(yellowSig, LOW);}

void loop() {
   botOne();
  // colorTest();
  //determine_color();
  // String newRoute = "GET /" + our_id + "/" + our_id + " HTTP/1.1";
  // String message = GET(newRoute);
  // Serial.println(message);
}

void botOne() {
  // Flash blue and red LEDS three times to show the bot is out of diagnostic state
  showDiagnostic();

  //sync start with companion bot over wifi
  POST(postRoute, "let_us_begin");
  while(GET(getRoute) != "begin_now") {
    stop(500);
  }
  digitalWrite(headLights, HIGH); //activate head lights for night

  // Move for on yellow until other bot detected -- then stop and warn other bot 
  while (not determine_collision() or GET(getRoute) != "collision_incoming") {
    find(yellow);
  }
  stop(500);
  POST(postRoute, "collision_incoming"); // warn other bot of collision

  // Use light based communication to warn other bot
  for(int i = 0; i < 2; i++)
  {
    digitalWrite(headLights, LOW);
    delay(500);
    digitalWrite(headLights, HIGH);
    delay(500);
  }

  Serial.println("COLLISION AVOIDED"); //let the user know collision avoided

  while(true){ // put on both turn signals for hazard lights
    digitalWrite(leftTurnSignal, HIGH);
    digitalWrite(rightTurnSignal, HIGH);
    delay(500);
    digitalWrite(leftTurnSignal, LOW);
    digitalWrite(rightTurnSignal, LOW);    
    delay(500);
  }

void botTwo() {
  // Flash blue and red LEDS three times to show the bot is out of diagnostic state
  showDiagnostic();

  //sync start with companion bot over wifi
  while(GET(getRoute) != "let_us_begin") {
    stop(500);
  }    
  POST(postRoute, "begin_now");
  digitalWrite(headLights, HIGH); //activate head lights for night

  // Move for on yellow until other bot detected -- then stop and warn other bot 
  while (not determine_collision() or GET(getRoute) != "collision_incoming") {
    find(yellow);
  }
  stop(500);
  POST(postRoute, "collision_incoming"); // warn other bot of collision

  // Use light based communication to warn other bot
  for(int i = 0; i < 2; i++)
  {
    digitalWrite(headLights, LOW);
    delay(500);
    digitalWrite(headLights, HIGH);
    delay(500);
  }

  Serial.println("COLLISION AVOIDED"); //let the user know collision avoided

  while(true){ // put on both turn signals for hazard lights
    digitalWrite(leftTurnSignal, HIGH);
    digitalWrite(rightTurnSignal, HIGH);
    delay(500);
    digitalWrite(leftTurnSignal, LOW);
    digitalWrite(rightTurnSignal, LOW);    
    delay(500);
}

void colorTest() {
  while (true) {
    digitalWrite(leftFor, HIGH);
    digitalWrite(rightFor, HIGH);
    determine_color();
  }
}
void blinkRed(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(redSig, HIGH);
    delay(200);
    digitalWrite(redSig, LOW);
    delay(200);
  }
}

void blinkGreen(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(greenSig, HIGH);
    delay(200);
    digitalWrite(greenSig, LOW);
    delay(200);
  }
}

void showDiagnostic() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(blueSig, HIGH);
    digitalWrite(redSig, HIGH);
    delay(200);
    digitalWrite(blueSig, LOW);
    digitalWrite(redSig, LOW);
    delay(200);
  }
}

bool determine_collision() {
  int reading = analogRead(IRDector);
  Serial.print("reading:");
  Serial.println(reading);
  bool output = reading > 350;
  return output;
}

Color determine_color() {
  int brightness = 0;
  int blue_reading = take_reading(HIGH, LOW) + brightness;
  int red_reading = take_reading(LOW, HIGH) + brightness;
  int both_reading = take_reading(HIGH, HIGH) + brightness;
  int diff = red_reading - blue_reading;
  int acceptable_range = 40;
  // String currPR = "POST /" + our_id + "/" + our_id  + " HTTP/1.1";
  // String readings = "blue:" + String(blue_reading) + "red:" + String(red_reading) + "both:"+ String(both_reading);
  // POST(currPR, readings);
  int blue_blue_read = 873;
  int blue_red_read = 898;
  int blue_both_read = 779;

  int red_blue_read = 910;
  int red_red_read = 700;
  int red_both_read = 645;

  int yellow_blue_read = 650;
  int yellow_red_read = 588;
  int yellow_both_read = 465;

  int black_blue_read = 937;
  int black_red_read = 917;
  int black_both_read = 847;

  Serial.print("Blue, Red, Both Readings: ");
  Serial.print(blue_reading);
  Serial.print(", ");
  Serial.print(red_reading);
  Serial.print(", ");
  Serial.println(both_reading);


  if (abs(red_reading - blue_red_read) <= acceptable_range) {
    if (abs(blue_reading - blue_blue_read) <= acceptable_range) {
      if (abs(both_reading - blue_both_read) <= acceptable_range) {
        Serial.println("blue");
        digitalWrite(redSig, LOW);
        digitalWrite(blueSig, HIGH);
        digitalWrite(yellowSig, LOW);
        digitalWrite(greenSig, LOW);
        return blue;
      }
    }
  }
  if (abs(red_reading - red_red_read) <= acceptable_range) {
    if (abs(blue_reading - red_blue_read) <= acceptable_range) {
      if (abs(both_reading - red_both_read) <= acceptable_range) {
        digitalWrite(redSig, HIGH);
        digitalWrite(blueSig, LOW);
        digitalWrite(yellowSig, LOW);
        digitalWrite(greenSig, LOW);
        Serial.println("red");
        return red;
      }
    }
  }
  if (abs(red_reading - yellow_red_read) <= acceptable_range) {
    // Serial.println("value matched yellow");
    if (abs(blue_reading - yellow_blue_read) <= acceptable_range) {
      if (abs(both_reading - yellow_both_read) <= acceptable_range) {
        Serial.println("yellow");
        digitalWrite(redSig, LOW);
        digitalWrite(blueSig, LOW);
        digitalWrite(yellowSig, HIGH);
        digitalWrite(greenSig, LOW);
        digitalWrite(redSig, LOW);
        return yellow;
      }
    }
  }

  if (abs(red_reading - black_red_read) <= acceptable_range) {
    if (abs(blue_reading - black_blue_read) <= acceptable_range) {
      if (abs(both_reading - black_both_read) <= acceptable_range) {
        Serial.println("black");
        digitalWrite(redSig, LOW);
        digitalWrite(blueSig, LOW);
        digitalWrite(yellowSig, LOW);
        digitalWrite(greenSig, HIGH);
        return black;
      }
    }
  }
}

int take_reading(bool blue, bool red) {
  digitalWrite(bluePin, blue);
  int num_readings = 10;
  digitalWrite(redPin, red);
  delay(10);
  float sum = 0;
  for (int i = 0; i < num_readings; i++) {
    sum += analogRead(colorPin);
  }
  delay(10);
  digitalWrite(bluePin, LOW);
  digitalWrite(redPin, LOW);
  return sum / num_readings;
}

void forward(int duration)  // duration in sec
{
  digitalWrite(leftRev, LOW);
  digitalWrite(rightRev, LOW);
  digitalWrite(leftFor, HIGH);
  digitalWrite(rightFor, HIGH);
  delay(duration);
  digitalWrite(leftFor, LOW);
  digitalWrite(rightFor, LOW);
}

void reverse(int duration)  // duration in sec
{
  digitalWrite(leftRev, HIGH);
  digitalWrite(rightRev, HIGH);
  digitalWrite(leftFor, LOW);
  digitalWrite(rightFor, LOW);
  delay(duration);
  digitalWrite(leftRev, LOW);
  digitalWrite(rightRev, LOW);
}

void pivot_right(int angle)  // duration in sec
{
  digitalWrite(leftRev, LOW);
  digitalWrite(rightRev, HIGH);
  digitalWrite(leftFor, HIGH);
  digitalWrite(rightFor, LOW);
  delay(angle);
  digitalWrite(rightRev, LOW);
  digitalWrite(leftFor, LOW);
}

void pivot_left(int angle) {
  digitalWrite(leftRev, HIGH);
  digitalWrite(rightRev, LOW);
  digitalWrite(leftFor, LOW);
  digitalWrite(rightFor, HIGH);
  delay(angle);
  digitalWrite(leftRev, LOW);
  digitalWrite(rightFor, LOW);
}

void turn_left(int duration) {
  digitalWrite(dayIndicatorLED, HIGH);
  digitalWrite(leftRev, LOW);
  digitalWrite(rightRev, LOW);
  digitalWrite(leftFor, HIGH);
  digitalWrite(rightFor, HIGH);
  delay(duration);
  digitalWrite(leftFor, LOW);
  digitalWrite(rightFor, LOW);
  digitalWrite(dayIndicatorLED, LOW);
}

void stop_moving() {
  delay(20);
}

void setupWifi() {
  Serial.println("\nStarting connection to server...");
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true)
      ;  //don't continue
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
    delay(500);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();
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

void stop(int time) {  //time
  digitalWrite(brakeLights, HIGH);
  digitalWrite(leftRev, HIGH);
  digitalWrite(rightRev, HIGH);
  digitalWrite(leftFor, HIGH);
  digitalWrite(rightFor, HIGH);
  delay(50);
  digitalWrite(leftRev, LOW);
  digitalWrite(rightRev, LOW);
  digitalWrite(leftFor, LOW);
  digitalWrite(rightFor, LOW);
  digitalWrite(brakeLights, LOW);
}

void POST(String theRoute, String message) {
  message = "message=" + message;
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
    // client.println("Connection: close");
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
  // Serial.println(message);
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

void find(Color c) {
  if(determine_color() == c or determine_color() == c or determine_color() == c)
    {
      forward(50);
      pivot_right(10);
    }
    else 
    {
      while(determine_color() != c) {
        pivot_left(10);
      }
      
    }
}