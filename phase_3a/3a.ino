#define redPin 10
#define bluePin 11
#define batteryIndicatorLED 12
#define dayIndicatorLED 13
#define ambientLightPin A2
#define colorPin A1
#define batteryPin A0

void setup() {
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(batteryIndicatorLED, OUTPUT);
  pinMode(dayIndicatorLED, OUTPUT);
  pinMode(ambientLightPin, INPUT);
  pinMode(colorPin, INPUT);
}
void loop() {
  
}

bool battery_reading()
{
  int reading = analogRead(batteryPin);
  const float battery_fraction = 5/1023;
  float voltage = reading*battery_fraction;
  if(voltage < 1)
  {
    digitalWrite(batteryIndicatorLED, HIGH);
    return false;
  }
  return true;
}

bool determine_day() {
  int reading = analogRead(ambientLightPin);
  if (reading > 100) {
    // blink if day
    for (int i = 0; i < 30; i++) {
      digitalWrite(dayIndicatorLED, HIGH);
      delay(100);
      digitalWrite(dayIndicatorLED, LOW);
      delay(100);
    }
  } else {
    digitalWrite(dayIndicatorLED, HIGH);
  }
  return reading > 100;
}

bool determine_blue() {
  float blue_reading = take_reading(HIGH, LOW);
  int red_reading = take_reading(LOW, HIGH);
  bool output = red_reading > 25 and red_reading < 50 and blue_reading > 15 and blue_reading < 40;
  // Serial.print("blue_reading: ");
  // Serial.println(blue_reading);
  // Serial.print("red_reading:");
  // Serial.println(red_reading);
  // Serial.println(output);
  return output;
}
bool determine_red() {
  float blue_reading = take_reading(HIGH, LOW);
  int red_reading = take_reading(LOW, HIGH);
  bool output = red_reading > 140 and red_reading < 200 and blue_reading < 40;
  Serial.print("blue_reading: ");
  Serial.println(blue_reading);
  Serial.print("red_reading:");
  Serial.println(red_reading);
  Serial.println(output);
  return output;
}
// both-103 ish
bool determine_yellow() {
  // Serial.println("Determining yellow");
  float blue_reading = take_reading(HIGH, LOW);
  int red_reading = take_reading(LOW, HIGH);
  bool output = red_reading > 200 and red_reading < 265 and blue_reading > 10 and blue_reading < 50;
  // Serial.print("blue_reading: ");
  // Serial.println(blue_reading);
  // Serial.print("red_reading:");
  // Serial.println(red_reading);
  // Serial.println(output);
  return output;
}


float take_reading(bool blue, bool red) {
  digitalWrite(bluePin, blue);
  digitalWrite(redPin, red);
  delay(50);
  float sum = 0;
  for (int i = 0; i < 30; i++) {
    sum += analogRead(colorPin);
  }
  delay(50);
  digitalWrite(bluePin, LOW);
  digitalWrite(redPin, LOW);
  return sum / 30;
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
