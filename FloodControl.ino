int debug         = 0;
int FloatSensor   = 12;
int OK            = 8;
int Relay         = 6;
int Alarm         = 4;
int SensorState   = 1;
int FloodState    = 0;
int TimerDefault  = 1800;
int Timer;

void setup() 
{
  if (debug == 1) {
    Serial.begin(9600);
  }
  pinMode(OK, OUTPUT);
  pinMode(Alarm, OUTPUT);
  pinMode(Relay, OUTPUT);
  pinMode(FloatSensor, INPUT_PULLUP);
  AutoTest();
  ResetAlarm();
  ResetRelay();
  ResetTimer();
}

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

void ResetAlarm() {
  digitalWrite(Alarm, LOW);
  digitalWrite(OK, HIGH);
  if (debug == 1) {
    Serial.println("Alarm RESET");
  }
}

void ResetRelay() {
  digitalWrite(Relay, LOW);
  if (debug == 1) {
    Serial.println("Relay RESET");
  }
  FloodState = 0;
}

void ResetTimer() {
  Timer = TimerDefault;
  if (debug == 1) {
    Serial.println("Timer RESET");
  }
}

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
  delay(1000);
}
