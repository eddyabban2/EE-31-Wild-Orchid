
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

#define reading_9volt A5
#define reading_6volt A4



void setup() {
  Serial.begin(9600);
  pinMode(leftFor, OUTPUT);
  pinMode(leftRev, OUTPUT);
  pinMode(rightFor, OUTPUT);
  pinMode(rightRev, OUTPUT); 
  pinMode(leftEnable, OUTPUT);
  pinMode(rightEnable, OUTPUT);
  pinMode(redSig, OUTPUT);
  pinMode(yellowSig, OUTPUT);
  pinMode(reading_9volt, INPUT);
  pinMode(reading_6volt, INPUT);
}


void loop() {
  
  take_9volt_reading();

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

void take_9volt_reading() {

  int reading = analogRead(reading_9volt);
  double voltage = (reading * 4.5) / 1023 + 0.4;

  if (voltage > 4.0) {

    digitalWrite(redSig, LOW);

  } else if (voltage <= 4.0 && voltage > 3.5) {

    digitalWrite(redSig, HIGH);
    delay(500);
    digitalWrite(redSig, LOW);
    delay(500);

  }

  else {

    digitalWrite(redSig, HIGH);

  }

  Serial.println(voltage);

}

