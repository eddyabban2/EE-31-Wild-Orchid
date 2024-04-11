#define redPin 10
#define bluePin 11
#define batteryIndicatorLED 12
#define dayIndicatorLED 13
#define ambientLightPin A2
#define colorPin A1
#define batteryPin A0
#define leftFor 2
#define leftRev 3
#define leftEnable 5
#define rightFor 7
#define rightRev 8
#define rightEnable 9

double turn_const = 2500;

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
}
void loop() {
  // detecter_trial();
  movement_trial();
  // forward(1000000);
  // color_test();
}
void detecter_trial() {
  battery_reading();
  determine_day();
}
void color_test() {
  while (true) {
    bool blue = determine_blue();
    bool red = determine_red();
    bool yellow = determine_yellow();
    bool black = determine_black();
    if (blue) {
      Serial.println("blue");
    }
    if (yellow) {
      Serial.println("Yellow");
    }
    if (red) {
      Serial.println("Red");
    }
    if (black) {
      Serial.println("Black");
    }
  }
}


void movement_trial() {
  const int forward_const = 50;
  while (true) {
    Serial.println("on blue");
    while (determine_blue()) {
      Serial.println("is blue");
      forward(forward_const);
    }
    Serial.println("off blue");
    pivot_left(75);
    delay(200);
    while (determine_yellow()) {
      Serial.println("on yellow");
      forward(forward_const);
      delay(20);
    }
    Serial.println("off yellow");
    delay(200);
    pivot_left(75);
    while (determine_red()) {
      Serial.println("red yellow");
      forward(forward_const);
      delay(20);
    }
    while (true);
  }
}
bool battery_reading() {
  int reading = analogRead(batteryPin);
  Serial.print("Raw reading: ");
  Serial.println(reading);
  const double battery_fraction = 5.0 / 1023.0;
  Serial.print("battery fraction: ");
  Serial.println(battery_fraction);

  double voltage = reading * battery_fraction;
  Serial.print("battery voltage: ");
  Serial.println(voltage);
  if (voltage < 1) {
    digitalWrite(batteryIndicatorLED, HIGH);
    return false;
  }
  return true;
}

bool determine_day() {
  int threshold = 200;
  int reading = analogRead(ambientLightPin);
  Serial.print("ambient light reading:");
  Serial.println(reading);
  if (reading > threshold) {
    // blink if day
    for (int i = 0; i < 10; i++) {
      digitalWrite(dayIndicatorLED, HIGH);
      delay(100);
      digitalWrite(dayIndicatorLED, LOW);
      delay(100);
    }
  } else {
    digitalWrite(dayIndicatorLED, HIGH);
  }
  return reading > threshold;
}

bool determine_blue() {
  float blue_reading = take_reading(HIGH, LOW);
  int red_reading = take_reading(LOW, HIGH);
  bool output = red_reading > 25 and red_reading < 50 and blue_reading > 20 and blue_reading < 60;
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
  bool output = red_reading > 100 and red_reading < 140 and blue_reading < 30;
  // Serial.print("blue_reading: ");
  // Serial.println(blue_reading);
  // Serial.print("red_reading:");
  // Serial.println(red_reading);
  // Serial.println(output);
  return output;
}
// both-103 ish
bool determine_yellow() {
  Serial.println("Determining yellow");
  float blue_reading = take_reading(HIGH, LOW);
  int red_reading = take_reading(LOW, HIGH);
  bool output = red_reading > 100 and red_reading < 130 and blue_reading > 10 and blue_reading < 30;
  Serial.print("blue_reading: ");
  Serial.println(blue_reading);
  Serial.print("red_reading:");
  Serial.println(red_reading);
  Serial.println(output);
  return output;
}

bool determine_black() {
  // Serial.println("Determining black");
  float blue_reading = take_reading(HIGH, LOW);
  int red_reading = take_reading(LOW, HIGH);
  bool output = red_reading > 30 and red_reading < 50 and blue_reading < 20;
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
  analogWrite(leftEnable, 20);
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
  analogWrite(leftEnable, 70);
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
