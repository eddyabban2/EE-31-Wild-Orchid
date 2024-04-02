#define redPin 10 
#define bluePin 11
#define ambientLightPin A1



void setup() {
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(ambientLightPin, INPUT);
  Serial.println("setup complete");

}

void loop() {
  if(determine_day())
  {
    Serial.println("day");
  }
  else
  {
    Serial.println("night");
  }

}

bool determine_day()
{
  int reading = analogRead(ambientLightPin);
  if(reading > 4) return true;
  return false;
  }
