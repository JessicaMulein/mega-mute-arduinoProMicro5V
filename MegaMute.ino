const int LOOP_SLEEP = 500;
const uint8_t pinModes[4] = {INPUT_PULLUP, INPUT, INPUT, INPUT};
const uint8_t digitalPins[4] = {4, 5, 6, 7}; // contain to port D (0-7)
const uint8_t RXLED = 17;

bool pinsChanged = false;
unsigned long lastSendData = 0;
int previousState[4] = {-1,-1,-1,-1};
int currentState[4] = {-1,-1,-1,-1};
unsigned long previousStateTime[4] = {0,0,0,0};
unsigned long currentStateTime[4] = {0,0,0,0};

void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);
  for (int i=0; i<4; i++) {
      pinMode(digitalPins[i], pinModes[i]);
  }
}

void loop() {  
  int newState[4];
  const unsigned long startChangeTime = millis();
  for (int i=0; i<4; i++) {
    newState[i] = digitalRead(digitalPins[i]);
  }
 
  const bool timerElapsed = ((startChangeTime-lastSendData) >= LOOP_SLEEP);
  for (int i=0; i<4; i++) {
    if (currentState[i] != newState[i]) {
      previousState[i] = currentState[i];
      previousStateTime[i] = currentStateTime[i];
      currentState[i] = newState[i];
      currentStateTime[i] = startChangeTime;
      pinsChanged = true;
    }
  }
  
  if (pinsChanged || timerElapsed) {
    char stateString[512];
    int strLen = sprintf(stateString, "{ \"c\":%d, \"t\":%lu, \"s\":[", (pinsChanged ? 1 : 0), startChangeTime);
    for (int i=0; i<4; i++) {
      strLen += sprintf(stateString + strLen, "{ \"p\":%d, \"v\":%d, \"tc\":%lu, \"tp\":%lu}", i, currentState[i], currentStateTime[i], previousStateTime[i]);
      if (i<3) stateString[strLen++] = ',';
    }
    stateString[strLen++] = ']';
    stateString[strLen++] = '}';
    stateString[strLen++] = '\0';
    if (Serial.availableForWrite() > 0) Serial.println(stateString);
    if (Serial1.availableForWrite() > 0) Serial1.println(stateString);
    lastSendData = startChangeTime;
    pinsChanged = false;
  }
  while (Serial1.available() > 0) {
    char command = Serial1.read();
    switch (command) {
      default:
      char response[100];
      sprintf(response, "{ \"time\":%lu, \"command\": \"%c\"}", millis(), command);
      Serial1.println(response);
    }
  }
  while (Serial.available() > 0) {
    char command = Serial.read();
    switch (command) {
      default:
      char response[100];
      sprintf(response, "{ \"time\":%lu, \"command\": \"%c\"}", millis(), command);
      Serial.println(response);
    }
  }
}
