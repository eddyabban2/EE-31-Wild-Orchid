// pin setup
#define leftFor A0
#define leftRev A1
#define leftEnable 4
#define rightFor A2
#define rightRev A3
#define rightEnable 5



int turn_const = 1000; // the amount of milliseconds that it takes for the board to turn 360 degrees
void setup() {
  pinMode(leftFor, OUTPUT);
  pinMode(leftRev, OUTPUT);
  pinMode(rightFor, OUTPUT);
  pinMode(rightRev, OUTPUT); 
  pinMode(leftEnable, OUTPUT);
  pinMode(rightEnable, OUTPUT);

  digitalWrite(leftEnable, HIGH);
  digitalWrite(rightEnable, HIGH);
}

void loop() {
  forward(1000);
  rev(1000);
  right(180);
  left(180);

}

void forward(int duration) // duration in sec
{
  analogWrite(leftRev, 0);
  analogWrite(rightRev, 0);
  analogWrite(leftFor, 255);
  analogWrite(rightFor, 255);
  delay(duration);
}

void rev(int duration) // duration in sec
{
  analogWrite(leftRev, 255);
  analogWrite(rightRev, 255);
  analogWrite(leftFor, 0);
  analogWrite(rightFor, 0);
  delay(duration);
}

void right(int angle) // duration in sec
{
  analogWrite(leftRev, 0);
  analogWrite(rightRev, 255);
  analogWrite(leftFor, 255);
  analogWrite(rightFor, 0);
  int delay_time = (turn_const/360)*angle;
  delay(delay_time);
}

void left(int angle)
{
  analogWrite(leftRev, 255);
  analogWrite(rightRev, 0);
  analogWrite(leftFor, 0);
  analogWrite(rightFor, 255);
  int delay_time = (turn_const/360)*angle;
  delay(delay_time);
}

