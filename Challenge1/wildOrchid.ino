#include <SPI.h>
#include <WiFiNINA.h>

#define redPin 10
#define bluePin 11
#define batteryIndicatorLED 12
#define dayIndicatorLED 13
#define IRDector A3
#define ambientLightPin A2
#define colorPin A1
#define batteryPin A0
#define leftFor 2
#define leftRev 3
#define rightFor 7
#define rightRev 8
#define blueSig 0
#define redSig 1
#define greenSig 4
#define yellowSig 6
#define brakeLights 5
#define headLights 9
#define leftTurnSignal A4
#define rightTurnSignal A5

char ssid[] = "junior";                // your network SSID (name)
char pass[] = "designdesign";          // your network password
char server[] = "ee31.ece.tufts.edu";  // server
int portNumber = 80;

int status = WL_IDLE_STATUS;

String our_id = "F392FC86D8D7";
String companion_team_id = "F79721857DC5";

WiFiClient client;

double turn_const = 1300;
enum color { red,
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
  pinMode(leftEnable, OUTPUT);
  pinMode(rightEnable, OUTPUT);
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
  analogWrite(leftEnable, 255);
  analogWrite(rightEnable, 255);
  setupWifi();
}
void loop() {
  botOne();
  //colorTest();
  while (true);
}

void botOne() {
  // Flash blue and red LEDS three times to show the bot is out of diagnostic state
  showDiagnostic();
  // Move forward
  // Detect the wall
  while (not determine_collision()) {
    forward(50);
  }
  pivot_left(250);
  // Turn 180 degrees
  // Move forward until it finds red
  while (determine_color() != red and determine_color() != red and determine_color() != red) {
    forward(50);
  }
  // Turn on the red led
  digitalWrite(redSig, HIGH);
  while(true); 
  // Use light based communication to tell the other bot that it can go
  // add light based communication here

  // Get a light based message back from bot 2, flash headlights and brake lights, beep horn twice
  // add light based communication here

  // Go on red until wall collision or end of red
  while (determine_color() == red and not determine_collision()) {
    forward(50);
  }
  digitalWrite(redSig, LOW);


  // Blink red led
  blinkRed(2);
  // do some magic to get the second signal

  // When you receive bot 2’s signal blink red led 3 times
  blinkRed(3);
  // Turn on green led
  digitalWrite(greenSig, HIGH);
    // Back up
    reverse(100);
  // Make 90 degree left turn (exactly 90)
  pivot_left(90);
  // Go until detect yellow
  while (determine_color() != yellow and determine_color() != yellow and determine_color() != yellow) {
    forward(50);
  }
  // Beep the horn twice
  // figure out how to beep the horn 

  pivot_left(90);
  // Follow yellow path until the end
  while (determine_color() == yellow and determine_color() == yellow and determine_color() == yellow) {
    forward(50);
  }

  pivot_left(90);
  // Turn left and go to end
  forward(300);
  // Signal to bot two that you have reached the end
}

void botTwo() {
}

void colorTest()
{
  while(true)
  {
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
  bool output = reading > 250;
  return output;
}

color determine_color() {
  int brightness = 0;
  int blue_reading = take_reading(HIGH, LOW) + brightness;
  int red_reading = take_reading(LOW, HIGH) + brightness;
  int both_reading = take_reading(HIGH, HIGH) + brightness;
  int diff = red_reading - blue_reading;
  int acceptable_range = 10;

  int blue_blue_read = 25;
  int blue_red_read = 32;
  int blue_both_read = 61;

  int red_blue_read = 15;
  int red_red_read = 74;
  int red_both_read = 86;

  int yellow_blue_read = 51;
  int yellow_red_read = 105;
  int yellow_both_read = 149;

  int black_blue_read = 7;
  int black_red_read = 26;
  int black_both_read = 36;

  Serial.print("Blue, Red, Both Readings: ");
  Serial.print(blue_reading);
  Serial.print(", ");
  Serial.print(red_reading);
  Serial.print(", ");
  Serial.println(both_reading);


  if (abs(red_reading - blue_red_read) <= acceptable_range) {
    if (abs(blue_reading - blue_blue_read) <= acceptable_range) {
      if (abs(both_reading - blue_both_read) <= acceptable_range) {
        if (diff < 15) {
          Serial.println("blue");
          return blue;
        }
      }
    }
  }
  if (abs(red_reading - red_red_read) <= acceptable_range) {
    if (abs(blue_reading - red_blue_read) <= acceptable_range) {
      if (abs(both_reading - red_both_read) <= acceptable_range) {
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
        return yellow;
      }
    }
  }


  if (abs(red_reading - black_red_read) <= acceptable_range) {
    if (abs(blue_reading - black_blue_read) <= acceptable_range) {
      if (abs(both_reading - black_both_read) <= acceptable_range) {
        if (diff > 10) {
          Serial.println("black");
          return black;
        }
      }
    }
  }
}


int take_reading(bool blue, bool red) {
  digitalWrite(bluePin, blue);
  digitalWrite(redPin, red);
  delay(20);
  float sum = 0;
  for (int i = 0; i < 30; i++) {
    sum += analogRead(colorPin);
  }
  delay(20);
  digitalWrite(bluePin, LOW);
  digitalWrite(redPin, LOW);
  return sum / 30;
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
  analogWrite(rightEnable, 240);
  digitalWrite(leftRev, HIGH);
  digitalWrite(rightRev, HIGH);
  digitalWrite(leftFor, LOW);
  digitalWrite(rightFor, LOW);
  delay(duration);
  digitalWrite(leftRev, LOW);
  digitalWrite(rightRev, LOW);
  analogWrite(rightEnable, 255);
}

void pivot_right(int angle)  // duration in sec
{
  analogWrite(rightEnable, 127);
  analogWrite(leftEnable, 127);
  digitalWrite(leftRev, LOW);
  digitalWrite(rightRev, HIGH);
  digitalWrite(leftFor, HIGH);
  digitalWrite(rightFor, LOW);
  double delay_time = (turn_const / 360.0) * angle;
  delay((int)delay_time - 100);
  digitalWrite(rightRev, LOW);
  digitalWrite(leftFor, LOW);
  analogWrite(leftEnable, 255);
  analogWrite(rightEnable, 255);
}

void pivot_left(int angle) {
  analogWrite(leftEnable, 255);
  analogWrite(rightEnable, 255);
  digitalWrite(leftRev, HIGH);
  digitalWrite(rightRev, LOW);
  digitalWrite(leftFor, LOW);
  digitalWrite(rightFor, HIGH);
  double delay_time = (turn_const / 360.0) * angle;
  delay((int)delay_time);
  digitalWrite(leftRev, LOW);
  digitalWrite(rightFor, LOW);
  analogWrite(leftEnable, 255);
  analogWrite(rightEnable, 255);
}

void turn_left(int duration) {
  digitalWrite(dayIndicatorLED, HIGH);
  analogWrite(leftEnable, 20);
  analogWrite(rightEnable, 200);
  digitalWrite(leftRev, LOW);
  digitalWrite(rightRev, LOW);
  digitalWrite(leftFor, HIGH);
  digitalWrite(rightFor, HIGH);
  delay(duration);
  digitalWrite(leftFor, LOW);
  digitalWrite(rightFor, LOW);
  analogWrite(leftEnable, 255);
  analogWrite(rightEnable, 255);
  digitalWrite(dayIndicatorLED, LOW);
}

void stop_moving() {

  analogWrite(leftEnable, 0);
  analogWrite(rightEnable, 0);
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
