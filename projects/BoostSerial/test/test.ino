int LED_PIN = 13;
int BAUD = 9600;
int FREQUENCY = 1; // Hz

// the setup routine runs once when you press reset:
void setup() {        
  Serial.begin(BAUD);  
  pinMode(LED_PIN, OUTPUT);     
}

// the loop routine runs over and over again forever:
void loop() {
  while (Serial.available() > 0) {
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    Serial.read();
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
  }
  digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.println("ping");
  delay(int(1000 / FREQUENCY));
}
