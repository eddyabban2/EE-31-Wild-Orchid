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
#define leftEnable 5
#define rightFor 7
#define rightRev 8
#define rightEnable 9

char ssid[] = "junior";        // your network SSID (name)
char pass[] = "designdesign";  // your network password
char server[] = "ee31.ece.tufts.edu";  // server
int portNumber = 80;

const char UID[] = "F392FC86D8D7";

char postBody[] = "red=eddywashere";

int status = WL_IDLE_STATUS;

const char getRoute[] = "GET /F392FC86D8D7/F392FC86D8D7 HTTP/1.1";
const char postRoute[] = "POST /F392FC86D8D7/F392FC86D8D7 HTTP/1.1";

WiFiClient client;

double turn_const = 2500;
enum color {red, blue, yellow, black, none};

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
  analogWrite(leftEnable, 255);
  analogWrite(rightEnable, 255);
  // setupWifi();
}
void loop() {
  // day_detecter_trial();
  // movement_trial();
  // determine_color();
  // delay(500);
  // delay(10000000);
  // determine_collision();
  // collision_test();
  battery_detecter_trial();
}
void day_detecter_trial() {
  // battery_reading();
  if (determine_day()) {
    // blink if day
    digitalWrite(dayIndicatorLED, HIGH);
    delay(100);
    digitalWrite(dayIndicatorLED, LOW);
    delay(100);
    
  } else {
    digitalWrite(dayIndicatorLED, HIGH);
  }
}

void battery_detecter_trial()
{
  if (battery_reading()) {
    digitalWrite(batteryIndicatorLED, LOW);
    
  } else {
    digitalWrite(batteryIndicatorLED, HIGH);
  }
}

void collision_test()
{
  while(true)
  {
    if(not determine_collision())
    {
      forward(10);
    }
    else
    {
      pivot_left(110);
      forward(10);
      while(true);
    }
  }
}

bool determine_collision()
{
  int reading = analogRead(IRDector);
  bool output = reading > 350;
  Serial.print("reading: ");
  Serial.println(reading);
  Serial.print("output: ");
  Serial.println(output);
  return output;
}



void movement_trial() 
{
  const int forward_const = 50;
  while (true) {
    Serial.println("on blue");
    while (determine_color() == blue or determine_color() == blue or determine_color() == blue) {
      forward(forward_const);
      Serial.println("forward on blue");
    }
    Serial.println("off blue");
    delay(200);
    forward(325);
    pivot_left(45);
    delay(100);
    Serial.println("looking for yellow");
    while (determine_color() == yellow or determine_color() == yellow or determine_color() == yellow) {
      forward(forward_const);
      Serial.println("forward on yellow");
    }
    forward(220);
    pivot_left(55);
    Serial.println("found red");
    delay(200);
    while (determine_color() == red or determine_color() == red or determine_color() == red) {
      forward(forward_const);
      Serial.println("forward on red");
    }
    while (true);
  }
}
bool battery_reading() {
  const double battery_fraction = 5.0 / 1023.0;
  const int threshold = 3;
  int reading = analogRead(batteryPin);
  Serial.print("Raw reading: ");
  Serial.println(reading);

  double voltage = reading * battery_fraction;
  bool output = voltage > threshold;
  Serial.print("battery voltage: ");
  Serial.println(voltage);
  return output;
}

bool determine_day() {
  int threshold = 175;
  int reading = analogRead(ambientLightPin);
  bool output = reading > threshold;
  Serial.print("ambient light reading:");
  Serial.println(reading);
  return output;
}


// both-103 ish

color determine_color() {
    int brightness = 0;
    int blue_reading = take_reading(HIGH, LOW)  + brightness;
    int red_reading = take_reading(LOW, HIGH) + brightness;
    int both_reading = take_reading(HIGH, HIGH) + brightness;
    int diff = red_reading - blue_reading;
    int acceptable_range = 10;

    int blue_blue_read = 6;
    int blue_red_read = 13;
    int blue_both_read = 31;

    int red_blue_read = 5;
    int red_red_read = 51;
    int red_both_read = 63;

    int yellow_blue_read = 15;
    int yellow_red_read = 66;
    int yellow_both_read = 90;

    int black_blue_read = 5;
    int black_red_read = 21;
    int black_both_read = 28;

    Serial.print("Blue, Red, Both Readings: ");
    Serial.print(blue_reading);
    Serial.print(", ");
    Serial.print(red_reading);
    Serial.print(", ");
    Serial.println(both_reading);


    if(abs(red_reading - blue_red_read) <= acceptable_range)
    {
      if(abs(blue_reading - blue_blue_read) <= acceptable_range)
      {
        if(abs(both_reading - blue_both_read) <= acceptable_range)
        {
          if(diff < 15)
          {
            Serial.println("blue");
            return blue;
          }
        }
      }
    }
    if(abs(red_reading - red_red_read) <= acceptable_range)
    {
      if(abs(blue_reading - red_blue_read) <= acceptable_range)
      {
        if(abs(both_reading - red_both_read) <= acceptable_range)
        {
          Serial.println("red");
          return red;
        }
      }
    }
    if(abs(red_reading - yellow_red_read) <= acceptable_range)
    {
      // Serial.println("value matched yellow");
      if(abs(blue_reading - yellow_blue_read) <= acceptable_range)
      {
        if(abs(both_reading - yellow_both_read) <= acceptable_range)
        {
          Serial.println("yellow");
          return yellow;
        }
      }
    }
    

    if(abs(red_reading - black_red_read) <= acceptable_range)
    {
      if(abs(blue_reading - black_blue_read) <= acceptable_range)
      {
        if(abs(both_reading - black_both_read) <= acceptable_range)
        {
          if(diff > 10)
          {
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

void rev(int duration)  // duration in sec
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
void setupWifi()
{
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

void send_readings(const char theRoute[],int blue_reading, int red_reading, int both_reading) {
  if(client.connect(server, portNumber))
  {
    String message = "red=";
    message += String(red_reading);
    message += "blue=";
    message += String(blue_reading);
    message += "both=";
    message += String(both_reading);
    client.println(theRoute);
    client.print("Host: ");
    client.println(server);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    int postBodyLength = message.length();
    client.println(message);
    client.println();
    client.print(message);
  }
}
