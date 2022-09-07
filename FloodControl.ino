// Runtime conditions
int debug         = 0;

// Pin attributions
int FloatSensor   = 12;
int OK            = 8;  // OK Led, Green: HIGH while the device is connected and no flood event was detected
int Relay         = 6;  // Load relay, Flood Led, Red: HIGH while a flood event is decurring
int Alarm         = 4;  // Alarm Led, Yellow: HIGH after a flood event has been detected, even if is no longer ocurring

int SensorState   = 1;
int FloodState    = 0;  // Assumes value 1 when a flood event is currently ocurring
int TimerDefault  = 1800;  // Default value for the flood timer, in seconds. 
int Timer;                 // Current Flood Timer value. This timer counts down after the end of a flood event. When if ends, the load relay is reset


void setup() 
{  
  if (debug == 1) {  // Activate serial monitor in case debug mode is on
    Serial.begin(9600);
  }
  
  // Sets up pins for leds/relay as Output
  pinMode(OK, OUTPUT);
  pinMode(Alarm, OUTPUT);
  pinMode(Relay, OUTPUT);
  
  // Sets up pin for float sensor as input using the internal pullup resistor
  pinMode(FloatSensor, INPUT_PULLUP);
  
  // Performs autotest
  AutoTest();
  
  // Resets default values
  ResetAlarm();
  ResetRelay();
  ResetTimer();
}

// AutoTest blinks 5 times leds OK and Alarm to signify operation start
void AutoTest() {
  int i;
  for (i=0; i<5; i++) {
    digitalWrite(Alarm, HIGH);
    delay(100);
    digitalWrite(OK, HIGH);
    delay(100);
    digitalWrite(Alarm, LOW);
    delay(100);
    digitalWrite(OK, LOW);
  }
}

// Resets alarm status to default status, settings the Alarm Led OFF and the OK Led ON.
void ResetAlarm() {
  digitalWrite(Alarm, LOW);
  digitalWrite(OK, HIGH);
  if (debug == 1) {
    Serial.println("Alarm RESET");
  }
}

// Resets load relay and flood led to OFF
void ResetRelay() {
  digitalWrite(Relay, LOW);
  if (debug == 1) {
    Serial.println("Relay RESET");
  }
  FloodState = 0;
}

// Resets timer counter to the default timer value
void ResetTimer() {
  Timer = TimerDefault;
  if (debug == 1) {
    Serial.println("Timer RESET");
  }
}

// Called when a flood event occurs. It deactivates both OK and Alarm leds and activates Flood led and load relay
void Flood() {
  digitalWrite(Alarm, LOW);  
  digitalWrite(OK, LOW);
  if (debug == 1) {
    Serial.println("Alarm ON");
  }  
  digitalWrite(Relay, HIGH);
  if (debug == 1) {
    Serial.println("Relay ON");
  }  
  FloodState = 1;
}

// Counts down the flood timer. When it reaches 0, resets the load relay and flood led and turns on the Alarm led.
void CountDown() {
  Timer = Timer - 1;
  if (debug == 1) {
    Serial.print("Timer: ");
    Serial.println((int)Timer);
  }  
  if (Timer == 0) {
    ResetRelay();
    digitalWrite(Alarm, HIGH);
  }
}

// Main function. It reads the state of the Float sensor and if it's HIGH (flood event occurring) and the timer is different than the timer default, it resets it
// back to the default, keeping it from reaching 0. It then activates the flood functions if they're not active already (floodstate). If the float sensor is LOW, it
// counts down the flood timer
void loop() {  
  SensorState = digitalRead(FloatSensor);
  
  if (SensorState == HIGH) {
    if (Timer != TimerDefault) {
      ResetTimer();
    }
    if (FloodState == 0) {
      Flood();
    }
  }
  else {
    if (FloodState == 1) {
      CountDown();
    }
  }
  delay(1000);  // Waits a second between every loop
}
