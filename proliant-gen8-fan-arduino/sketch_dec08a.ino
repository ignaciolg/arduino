#define PWM_PIN_OUT 5
#define PWM_PIN_IN 6

#define ANALOG_PIN A1
#define DEBUG true

int analogValue = 0;

void lg(String msg) {
  if (DEBUG) {
    Serial.println(msg);
  }
}

void lg(int msg) {
  if (DEBUG) {
    Serial.println(msg);
  }
}
void setSerial() {
    if (DEBUG)    {
        Serial.begin(9600);                               // // Serial Communication is starting with 9600 of baudrate speed
        lg("Convert HP Proliant FAN Signal to PWM"); // print some text in Serial Monitor
        lg("with Arduino NANO");
    }

}

void setup() {
   setSerial();
   pinMode(PWM_PIN_IN, INPUT);
   analogWrite(PWM_PIN_OUT, 25);

}

void loop() {
  analogValue = pulseIn(PWM_PIN_IN, HIGH);
  int con = map(analogValue, 0, 166, 0, 255);
  lg(con);
  analogWrite(PWM_PIN_OUT, 25);

  delay(1250);
}
