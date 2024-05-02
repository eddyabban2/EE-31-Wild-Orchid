
// pin setup
#define leftFor 2
#define leftRev 3
#define leftEnable 5
#define rightFor 7
#define rightRev 8
#define rightEnable 9

#define blueSig 0  // indicator LEDS
#define redSig 1
#define greenSig 4
#define yellowSig 6



void setup() {
  Serial.begin(9600);
  pinMode(leftFor, OUTPUT);
  pinMode(leftRev, OUTPUT);
  pinMode(rightFor, OUTPUT);
  pinMode(rightRev, OUTPUT); 
  pinMode(leftEnable, OUTPUT);
  pinMode(rightEnable, OUTPUT);
}

void loop() {
  
  // analogWrite(leftEnable, 100);
  // analogWrite(rightEnable, 90);
  // forward(20000);

  botSpeed(80, 38);
  analogWrite(leftEnable, 0);
  analogWrite(rightEnable, 0);
  while(true); 

}

void forward(int duration) // duration in sec
{
  digitalWrite(leftRev, LOW);
  digitalWrite(rightRev, LOW);
  digitalWrite(leftFor, HIGH);
  digitalWrite(rightFor, HIGH);
  delay(duration);
  digitalWrite(leftFor, LOW);
  digitalWrite(rightFor, LOW);
}

void botSpeed(int time, int duty) {
  
  digitalWrite(leftRev, LOW);
  digitalWrite(rightRev, LOW);
  digitalWrite(leftFor, HIGH);
  digitalWrite(rightFor, HIGH);

  for (int i = 0; i < time; i++) {

    digitalWrite(leftEnable, HIGH);
    digitalWrite(rightEnable, HIGH);
    delay(duty);
    digitalWrite(leftEnable, LOW);
    digitalWrite(rightEnable, LOW);
    delay(255 - duty);

  }

  digitalWrite(leftFor, LOW);
  digitalWrite(rightFor, LOW);

}
