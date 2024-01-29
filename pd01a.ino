// pin setup
int red = 8; // Digital Pin
int green = 9; // PWM pin
int blue = 10; // PWM Pin
int p0Pin = A0; // Analog IN Pin
int p0Val;
int p1Pin = A1;
int p1Val;
int runStateFlag = 0;
int isTenHzFlag = 0;
int orange = 12;
int interruptFlag = 0;

//interrupt setup
const byte interrupt1Pin = 2;
const byte interrupt2Pin = 1;




void setup() {
  pinMode(red, OUTPUT); //RED LED
  pinMode(green, OUTPUT); //GREEN LED
  pinMode(blue, OUTPUT); //BLUE LED
  pinMode(p0Pin, INPUT); // potentiometer pin
  pinMode(p1Pin, INPUT); // potentiometer pin
  pinMode(orange, OUTPUT); // LED Pin triggered by interrupt
  pinMode(interrupt1Pin, INPUT_PULLUP); // configure interrupt pin
  pinMode(interrupt2Pin, INPUT_PULLUP); // configure interrupt pin
  Serial.begin(9600); // setup serial input
  attachInterrupt(digitalPinToInterrupt(interrupt1Pin), interruptSequence1, FALLING);
  attachInterrupt(digitalPinToInterrupt(interrupt2Pin), interruptSequence2, RISING);
}




void loop() {
  p0Val = analogRead(p0Pin)/205;
  Serial.print("Potentiometer Value: ");
  Serial.println(p0Val);


  switch (p0Val) {
    case 0:
      Serial.println("on state");
      onState();
      break;
    case 1:
      Serial.println("off state");
      offState();
      break;
    case 2:
      Serial.println("run state");
      runState();
      break;
    case 3:
      Serial.println("sleep state");
      sleepState();
      break;
    case 4:
      Serial.println("diagnostic state");
      diaState(5);
      break;




  }
 
}




void onState() {
  //BLIUNK RED 10 TIMES/SEC
  unsigned long initalMilli = millis();
  unsigned long prevMilli = initalMilli;
  int currState = LOW;
  digitalWrite(red, currState);
  while(1)
  {
    unsigned long currMilli = millis();
    if(currMilli - prevMilli >= 50) {
      currState = not currState;
      digitalWrite(red, currState);
      prevMilli = currMilli;
    }
    if(currMilli-initalMilli > 6000)
    {
      break;
    }
  }
}




void offState() {
  //ALL LIGHTS OFF
  digitalWrite(red, LOW);
  analogWrite(green, LOW);
  digitalWrite(blue, LOW);
}




void runState() {
  //DECAY GREEN FOR 6 SECS, FLASH TWICE
  runStateFlag = 1;
  unsigned long initalMilli = millis();
  unsigned long prevMilli = initalMilli;
  while(1)
  {
    unsigned long currMilli = millis();
    int greenValue = 255 - (int((float(currMilli - initalMilli) / float(6000)) * 255.0));
    analogWrite(green, greenValue);

    if(interruptFlag == 1)
    {
      Serial.println("interrupt flag got triggerd");
      interruptState();
    }
    if(currMilli-initalMilli > 6000)
    {
      break;
    }
  }


  // runStateFlag = 1;
  // int output = 255;
  // while(output > 0) {
  //   p1Val = analogRead(p1Pin)/4;
  //   // Serial.print("Potentiometer value: ");
  //   // Serial.println(p1Val);
  //   analogWrite(blue, p1Val);
  //   analogWrite(green, output);
  //   delay(23);
  //   if(interuptOneFlag)
  //   output -= 1;
  // }
  // for(int i = 0; i < 2; i++) {
  //   p1Val = analogRead(p1Pin)/4;
  //   analogWrite(blue, p1Val);
  //   analogWrite(green, 127);
  //   delay(250);
  //   analogWrite(green, 0);
  //   delay(250);
  // }
  runStateFlag = 0;
}
void interruptState()
{
  unsigned long initalMilli = millis();
  unsigned long prevMilli = initalMilli;
  int blueState = LOW;
  digitalWrite(blue, blueState);
  isTenHzFlag = 1;
  while(1)
  {
    unsigned long currMilli = millis();
    if((isTenHzFlag == 0 && currMilli - prevMilli > 250) || (isTenHzFlag == 1 && currMilli - prevMilli > 25) )
    {
      blueState = not blueState;
      prevMilli = currMilli;
    }
    digitalWrite(blue, blueState);
    if(currMilli-initalMilli > 3000 && isTenHzFlag == 1)
    {
      isTenHzFlag = 0;
      digitalWrite(red, isTenHzFlag);
    }
    if(currMilli-initalMilli > 6000)
    {
      break;
    }
  }
  interruptFlag = 0;
}



void sleepState() {
  // blink blue at 4hz for 4 seconds
  unsigned long initalMilli = millis();
  unsigned long prevMilli = initalMilli;
  int currState = LOW;
  digitalWrite(blue, currState);
  while(1)
  {
    unsigned long currMilli = millis();
    if(currMilli - prevMilli >= 125) {
      currState = not currState;
      digitalWrite(blue, currState);
      prevMilli = currMilli;
    }
    if(currMilli-initalMilli > 1000)
    {
      break;
    }
  }

  // fade for 1 second
  int output = 255;
  while(output > 0) {
    analogWrite(blue, output);
    delay(4);
    output -= 1;
  }
  // last blink fades off over
}




void diaState(int numTimes) {
  for(int i = 0; i < numTimes; i++) {
    analogWrite(red, 255);
    delay(500);
    analogWrite(red, 0);
    delay(500);
  }
}




void interruptSequence1() {
  if(digitalRead(interrupt1Pin) == LOW || runStateFlag == 0)
  {
    return;
  }
  interruptFlag = 1;
}



void interruptSequence2() {
  if(digitalRead(interrupt2Pin) == LOW || runStateFlag == 0)
  {
    return;
  }                  
  digitalWrite(red, isTenHzFlag);
}
