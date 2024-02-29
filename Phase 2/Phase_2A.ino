// pin setup for MOSFET Switches
int q1 = 8;
int q2= 9;
int q3 = 10;
int q4 = 11;

void setup() {
  pinMode(q1, OUTPUT); // Switch 1
  pinMode(q2, OUTPUT); // Switch 2
  pinMode(q3, OUTPUT); // Switch 3
  pinMode(q4, OUTPUT); // Switch 4
  Serial.begin(9600); // setup serial input
}

void loop() {
 
  digitalWrite(q1, LOW);
  digitalWrite(q2, LOW);
  digitalWrite(q3, LOW);
  digitalWrite(q4, LOW);
  delay(100);

  while(true) {

    // spin forwards
    digitalWrite(q1, LOW);
    digitalWrite(q2, LOW);
    digitalWrite(q3, HIGH);
    digitalWrite(q4, HIGH);
    delay(5000);

    digitalWrite(q1, LOW);
    digitalWrite(q2, LOW);
    digitalWrite(q3, LOW);
    digitalWrite(q4, LOW);
    delay(5000);

    // spin backwards
    digitalWrite(q1, HIGH);
    digitalWrite(q2, HIGH);
    digitalWrite(q3, LOW);
    digitalWrite(q4, LOW);
    delay(5000);

    digitalWrite(q1, LOW);
    digitalWrite(q2, LOW);
    digitalWrite(q3, LOW);
    digitalWrite(q4, LOW);
    delay(5000);
   
  }

}
