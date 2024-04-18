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
  // day_detecter_trial();
  movement_trial();
  // color_test();
  // determine_collision();
  // collision_test();
}
void day_detecter_trial() {
  // battery_reading();
  determine_day();
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
      rev(1);
    }
  }
}

bool determine_collision()
{
  int reading = analogRead(IRDector);
  return reading > 70;
}
void color_test() {
  while (true) {

    bool blue = determine_blue();
    bool red = determine_red();
    bool yellow = determine_yellow();
    bool black = determine_black();

    if (blue) {
      Serial.println("Blue");
    } else if (red) {
      Serial.println("Red");
    } else if (yellow) {
      Serial.println("Yellow");
    } else if (black) {
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
    delay(200);
    pivot_left(60);
    while (determine_yellow()) {
      Serial.println("on yellow");
      forward(forward_const);
      delay(20);
    }
    pivot_left(60);
    Serial.println("found red");
    delay(200);
    while (determine_red()) {
      Serial.println("red yellow");
      forward(forward_const);
      delay(20);
    }
    while (true);
  }
}
bool battery_reading() {
  const double battery_fraction = 5.0 / 1023.0;

  int reading = analogRead(batteryPin);
  Serial.print("Raw reading: ");
  Serial.println(reading);

  double voltage = reading * battery_fraction;
  Serial.print("battery voltage: ");
  Serial.println(voltage);

  delay(1000);

  if (voltage < 2) {
    digitalWrite(batteryIndicatorLED, HIGH);
    return false;
  }

  digitalWrite(batteryIndicatorLED, LOW);
  return true;
}

bool determine_day() {
  int threshold = 250;
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

  int blue_reading = take_reading(HIGH, LOW);
  int red_reading = take_reading(LOW, HIGH);
  int difference = red_reading - blue_reading;

  bool output = (difference >= 5) and (difference <= 16);

  // Serial.print("Difference Reading: ");
  // Serial.println(difference);

  return output;

}

bool determine_red() {

  int blue_reading = take_reading(HIGH, LOW);
  int red_reading = take_reading(LOW, HIGH);

  bool blue_condition = (blue_reading >= 0) and (blue_reading <= 20);
  bool red_condition = (red_reading >= 50) and (red_reading <= 70);
  bool output = (blue_condition) and (red_condition);
  
  // Serial.print("Blue and Red Readings: ");
  // Serial.print(blue_reading);
  // Serial.print(", ");
  // Serial.println(red_reading);

  return output;

}
// both-103 ish
bool determine_yellow() {
  
  int blue_reading = take_reading(HIGH, LOW);
  int red_reading = take_reading(LOW, HIGH);

  bool blue_condition = (blue_reading >= 10) and (blue_reading <= 30);
  bool red_condition = (red_reading >= 70) and (red_reading <= 100);
  bool output = (blue_condition) and (red_condition);
  bool diff = red_reading - blue_reading;
  
  Serial.print("Blue and Red Readings: ");
  Serial.print(blue_reading);
  Serial.print(", ");
  Serial.println(red_reading);
  
  return output;

}

bool determine_black() {
  
  int blue_reading = take_reading(HIGH, LOW);
  int red_reading = take_reading(LOW, HIGH);
  int difference = red_reading - blue_reading;

  bool output = (difference >= 17) and (difference <= 35);

  // Serial.print("Difference Reading: ");
  // Serial.println(difference);

  return output;

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

void stop_moving() {

  analogWrite(leftEnable, 0);
  analogWrite(rightEnable, 0);

}
