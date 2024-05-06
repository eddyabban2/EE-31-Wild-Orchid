
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

  //Forward 12 inches
  botForwardSpeed(24, 40);
  
  delay(2000);
  // 180 deg turn
  botRightInPlace(20,39); 
  delay(2000);
  //rev 3 in
  botReverseSpeed(6,40); 
  delay(2000);
  // Left Turn
  botLeftInPlace(10,39); 
  delay(2000);
  // Ambiguous movement (between left and right #1 turns)
  botForwardSpeed(12,40);
  delay(2000);
  // Right Turn #1
  botRightInPlace(10, 39);
  delay(2000);
  // Ambiguous movement (between right #1 and right #2 turns)
  botForwardSpeed(30,40);
  delay(2000);
  // Right Turn #2
  botRightInPlace(10, 39);
  delay(2000);
  // Ambiguous movement (between left and right #1 turns)
  botForwardSpeed(10,40);
  delay(2000);
  // Right Turn #3
  botRightInPlace(10, 39);
  delay(2000);

  while(true); 
    analogWrite(leftEnable, 0);
    analogWrite(rightEnable, 0);
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

void botForwardSpeed(int time, int duty) {
  
  digitalWrite(leftRev, LOW);
  digitalWrite(rightRev, LOW);
  digitalWrite(leftFor, HIGH);
  digitalWrite(rightFor, HIGH);

  for (int i = 0; i < time; i++) {

    analogWrite(leftEnable, 255);
    analogWrite(rightEnable, 255);
    delay(duty);
    //delay(255 - duty);

  }
  analogWrite(leftEnable, 0);
  analogWrite(rightEnable, 0);
  digitalWrite(leftFor, LOW);
  digitalWrite(rightFor, LOW);
  
}

void botReverseSpeed(int time, int duty) {
  
  digitalWrite(leftRev, HIGH);
  digitalWrite(rightRev, HIGH);
  digitalWrite(leftFor, LOW);
  digitalWrite(rightFor, LOW);

  for (int i = 0; i < time; i++) {

    analogWrite(leftEnable, 255);
    analogWrite(rightEnable, 255);
    delay(duty);
    //delay(255 - duty);

  }
  analogWrite(leftEnable, 0);
  analogWrite(rightEnable, 0);
  digitalWrite(leftRev, LOW);
  digitalWrite(rightRev, LOW);
  
}

void botRightInPlace(int time, int duty) {
  
  digitalWrite(leftRev, LOW);
  digitalWrite(rightRev, HIGH);
  digitalWrite(leftFor, HIGH);
  digitalWrite(rightFor, LOW);

  for (int i = 0; i < time; i++) {

    analogWrite(leftEnable, 255);
    analogWrite(rightEnable, 255);
    delay(duty);
    //delay(255 - duty);

  }
  analogWrite(leftEnable, 0);
  analogWrite(rightEnable, 0);
  digitalWrite(leftFor, LOW);
  digitalWrite(rightRev, LOW);
  
}

void botLeftInPlace(int time, int duty) {
  
  digitalWrite(leftRev, HIGH);
  digitalWrite(rightRev, LOW);
  digitalWrite(leftFor, LOW);
  digitalWrite(rightFor, HIGH);

  for (int i = 0; i < time; i++) {

    analogWrite(leftEnable, 255);
    analogWrite(rightEnable, 255);
    delay(duty);
    //delay(255 - duty);

  }
  analogWrite(leftEnable, 0);
  analogWrite(rightEnable, 0);
  digitalWrite(leftRev, LOW);
  digitalWrite(rightFor, LOW);
  
}