// tutorial followed: https://lastminuteengineers.com/l293d-dc-motor-arduino-tutorial/

// pin setup
#define leftFor 2
#define leftRev 3
#define leftEnable 5
#define rightFor 7
#define rightRev 8
#define rightEnable 9 



double turn_const = 2500; // the amount of milliseconds that it takes for the board to turn 360 degrees
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

  // Serial.println("eddy was here");
  // Serial.println("Forward");
  // forward(700);
  // delay(1000);
  // Serial.println("Reverse");
  // rev(700);
  // delay(1000);
  // Serial.println("Right");
  // pivot_right(180);
  // delay(1000);
  // Serial.println("Left");
  // pivot_left(180);
  // delay(1000);
  // pivot_right(90);
  // delay(1000);
  // pivot_left(90);
  // delay(1000);


  for(int i = 0; i < 4; i ++)
  {
    turn_left(1950);
    delay(500);
    forward(500);
    delay(1000);
  }
  delay(1000);
}

void loop() {
  //Serial.println("eddy was here");
  // Serial.println("Forward");
  // forward(700);
  // delay(1000);
  // Serial.println("Reverse");
  // rev(700);
  // delay(1000);
  // Serial.println("Right");
  // pivot_right(180);
  // delay(1000);
  // Serial.println("Left");
  // pivot_left(180);
  // delay(1000);
  // pivot_right(90);
  // delay(1000);
  // pivot_left(90);
  // delay(1000);
  

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

void rev(int duration) // duration in sec
{
  digitalWrite(leftRev, HIGH);
  digitalWrite(rightRev, HIGH);
  digitalWrite(leftFor, LOW);
  digitalWrite(rightFor, LOW);
  delay(duration);
  digitalWrite(leftRev, LOW);
  digitalWrite(rightRev, LOW);
}

void pivot_right(int angle) // duration in sec
{
  analogWrite(rightEnable, 127);
  analogWrite(leftEnable, 127);
  digitalWrite(leftRev, LOW);
  digitalWrite(rightRev, HIGH);
  digitalWrite(leftFor, HIGH);
  digitalWrite(rightFor, LOW);
  double delay_time = (turn_const/360.0)*angle;
  Serial.println((int)delay_time);
  delay((int)delay_time - 100);
  digitalWrite(rightRev, LOW);
  digitalWrite(leftFor, LOW);
  analogWrite(leftEnable, 255);
  analogWrite(rightEnable, 255);
}

void pivot_left(int angle)
{
  analogWrite(leftEnable, 127);
  analogWrite(rightEnable, 127);
  digitalWrite(leftRev, HIGH);
  digitalWrite(rightRev, LOW);
  digitalWrite(leftFor, LOW);
  digitalWrite(rightFor, HIGH);
  double delay_time = (turn_const/360.0)*angle;
  Serial.println((int)delay_time);
  delay((int)delay_time);
  digitalWrite(leftRev, LOW);
  digitalWrite(rightFor, LOW);
  analogWrite(leftEnable, 255);
  analogWrite(rightEnable, 255);
}

void turn_left(int duration)
{
  analogWrite(leftEnable, 100);
  analogWrite(rightEnable, 255);
  digitalWrite(leftRev, LOW);
  digitalWrite(rightRev, LOW);
  digitalWrite(leftFor, HIGH);
  digitalWrite(rightFor, HIGH);
  delay(duration);
  digitalWrite(leftFor, LOW);
  digitalWrite(rightFor, LOW);
  analogWrite(leftEnable, 255);
  analogWrite(rightEnable, 255);
}
