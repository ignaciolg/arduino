// ---------------------------------------------------------------- //
// Arduino Ultrasoninc Sensor HC-SR04 light control
// Based on Arduino Ultrasoninc Sensor HC-SR04
// Re-writed by Arbi Abdul Jabbaar
// Re-re-writed by Ignacio Lopez
// Using Arduino IDE 1.8.13
// Using HC-SR04 Module
// Tested on 22 November 2020
// ---------------------------------------------------------------- //

/* Pin definition*/
#define ECHO_PIN 5 // attach pin D2 Arduino to pin Echo of HC-SR04
#define TRIGGER_PIN 6 //attach pin D3 Arduino to pin Trig of HC-SR04
#define LIGHT_PIN 12

/* loop delays */
#define TRIGGER_DELAY_LOW 2
#define TRIGGER_DELAY_HIGH 10
#define LOOP_DELAY 50
#define LOOP_DELAY_CORRECTED LOOP_DELAY - TRIGGER_DELAY_LOW - TRIGGER_DELAY_HIGH


/* other constants*/
#define AUTO_OFF_SECONDS 10
#define AUTO_OFF_TICKS AUTO_OFF_SECONDS * (1000/LOOP_DELAY)

#define COOLDOWN_DELAY_SECONDS 0.250
#define COOLDOWN_TICKS COOLDOWN_DELAY_SECONDS * (1000/LOOP_DELAY) // get from delay, for now manual 

#define SOUND_SPEED 0.034 / 2 // Speed of sound wave divided by 2 (go and back)

/* debug options */
#define DEBUG true

// defines global variables
int lightOn = LOW;
boolean enabled = false;

int cooldownCounter = 0;

long counter = 0; 



void setSerial() {
    if (DEBUG)    {
        Serial.begin(9600);                               // // Serial Communication is starting with 9600 of baudrate speed
        Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
        Serial.println("with Arduino UNO R3");
    }
}

void printLogs(int distance){
    if (false)    {
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");
        Serial.print("light on:");
        Serial.println(lightOn);
        Serial.print("Counter:");
        Serial.println(counter);
        Serial.print("ticks: ");
        Serial.print(AUTO_OFF_TICKS);
    }
}


int calculateDistance(){
    long duration; // variable for the duration of sound wave travel

    // Clears the trigPin condition
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(TRIGGER_DELAY_LOW);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(TRIGGER_DELAY_HIGH);
    digitalWrite(TRIGGER_PIN, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(ECHO_PIN, HIGH);
    return duration * SOUND_SPEED;     // Calculating the distance
}

boolean checkPresence(int distance){
  return distance < 25; // todo change this for the average to avoid fine tuning due the distances
}

/**
 * Return true when the ligh should be turn on
 * Returns false when it should be turned off
 */
boolean checkEnabled(boolean presence){
  if (enabled == false && presence == true) {
    enabled = true;
    cooldownCounter = 0;
    return true;    
  } else if (enabled == true && presence == true) {
    cooldownCounter = 0; 
  } else if (enabled == true && presence == false) {
    cooldownCounter = cooldownCounter + 1; 
  }

  if (cooldownCounter >= COOLDOWN_TICKS) {
    enabled = false;
  }  

  return false;
}

void handleLight(boolean change){
    if (change == true) {
      lightOn = lightOn ^ 1;
      digitalWrite(LIGHT_PIN, lightOn);
      // resets the number of iterations
      counter = 0; 
   } else {
      counter = counter + 1;
   }

   if (lightOn == HIGH && counter > AUTO_OFF_TICKS) {
      lightOn = LOW;
      digitalWrite(LIGHT_PIN, lightOn);
   } 

}



void setup(){
    pinMode(TRIGGER_PIN, OUTPUT); 
    pinMode(ECHO_PIN, INPUT);  
    pinMode(LIGHT_PIN, OUTPUT);

    setSerial();  
}

void loop(){
    int distance = calculateDistance();
    boolean presence = checkPresence(distance); 
    boolean enabled = checkEnabled(presence); 
    handleLight(enabled);
    printLogs(distance);

    delay(LOOP_DELAY_CORRECTED);
}