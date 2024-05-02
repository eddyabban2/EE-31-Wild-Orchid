//Challenge 4 -- Night Conditions

//#include <SPI.h>
//#include <WiFiNINA.h>

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
/*
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

WiFiClient client;*/

double turn_const = 1300;
enum Color { red,
             blue,
             yellow,
             black,
             none };

void setup() {
  Serial.begin(9600);
  Serial.println("staring up");
  pinMode(IRDector, INPUT);
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
  //setupWifi();
  digitalWrite(yellowSig, LOW);
  
  }


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

  //sync start with companion bot over wifi
  /*POST(postRoute, "let_us_begin");
  while(GET(getRoute) != "begin_now") {
    stop(500);
  }*/
  digitalWrite(headLights, HIGH); //activate head lights for night

  // Move for on yellow until other bot detected -- then stop and warn other bot 
  while (not determine_collision()) { /*or GET(getRoute) != "collision_incoming"*/
    //find(yellow);
    forward(100);
  }

  stop(500);
  //POST(postRoute, "collision_incoming"); // warn other bot of collision

  // Use light based communication to warn other bot
  for(int i = 0; i < 2; i++)
  {
    digitalWrite(headLights, LOW);
    delay(500);
    digitalWrite(headLights, HIGH);
    delay(500);
  }

  digitalWrite(headLights, LOW);

  //Serial.println("COLLISION AVOIDED"); //let the user know collision avoided

  while(true){ // put on both turn signals for hazard lights
    digitalWrite(leftTurnSignal, HIGH);
    digitalWrite(rightTurnSignal, HIGH);
    delay(500);
    digitalWrite(leftTurnSignal, LOW);
    digitalWrite(rightTurnSignal, LOW);    
    delay(500);
  }
}



bool determine_collision() {
  int reading = analogRead(IRDector);
  Serial.print("reading:");
  Serial.println(reading);
  bool output = reading > 35;
  return output;
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