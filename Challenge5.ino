
// pin setup
#define leftFor 2
#define leftRev 3
#define leftEnable A3
#define rightFor 7
#define rightRev 8
#define rightEnable A2



void setup() {
  Serial.begin(9600);
  pinMode(leftFor, OUTPUT);
  pinMode(leftRev, OUTPUT);
  pinMode(rightFor, OUTPUT);
  pinMode(rightRev, OUTPUT); 
  pinMode(leftEnable, OUTPUT);
  pinMode(rightEnable, OUTPUT);

void loop() {
  
  analogWrite(leftEnable, 32);
  analogWrite(rightEnable, 32);

  forward(20000);
  analogWrite(leftEnable, 0);
  analogWrite(rightEnable, 0);
  while(true); 

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
