// tutorial followed: https://lastminuteengineers.com/l293d-dc-motor-arduino-tutorial/

// pin setup
#define leftFor 2
#define leftRev 3
#define leftEnable 5
#define rightFor 7
#define rightRev 8
#define rightEnable 9 



int turn_const = 1000; // the amount of milliseconds that it takes for the board to turn 360 degrees
void setup() {
  Serial.begin(9600);
  pinMode(leftFor, OUTPUT);
  pinMode(leftRev, OUTPUT);
  pinMode(rightFor, OUTPUT);
  pinMode(rightRev, OUTPUT); 
  pinMode(leftEnable, OUTPUT);
  pinMode(rightEnable, OUTPUT);

  analogWrite(leftEnable, 255);
  analogWrite(rightEnable, 255);
}

void loop() {
  //Serial.println("eddy was here");
  forward(1000);
 rev(1000);
 right(180);
 left(180);

}

void forward(int duration) // duration in sec
{
  digitalWrite(leftRev, LOW);
  digitalWrite(rightRev, LOW);
  digitalWrite(leftFor, HIGH);
  digitalWrite(rightFor, HIGH);
  delay(duration);
}

void rev(int duration) // duration in sec
{
  digitalWrite(leftRev, HIGH);
  digitalWrite(rightRev, HIGH);
  digitalWrite(leftFor, LOW);
  digitalWrite(rightFor, LOW);
  delay(duration);
}

void right(int angle) // duration in sec
{
  digitalWrite(leftRev, LOW);
  digitalWrite(rightRev, HIGH);
  digitalWrite(leftFor, HIGH);
  digitalWrite(rightFor, LOW);
  int delay_time = (turn_const/360)*angle;
  delay(delay_time);
}

void left(int angle)
{
  digitalWrite(leftRev, HIGH);
  digitalWrite(rightRev, LOW);
  digitalWrite(leftFor, LOW);
  digitalWrite(rightFor, HIGH);
  int delay_time = (turn_const/360)*angle;
  delay(delay_time);
}
