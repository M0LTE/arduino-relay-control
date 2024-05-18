// how many relays
const int relays = 8;

// relays connected to this pin number and up
const int startPin = 5; 

// invert relay state
const bool invert = false;

// turn on all outputs at startup
const bool onByDefault = true;

void setup() {
  for (int i = startPin; i < startPin + relays; i++){
    pinMode(i, OUTPUT);
    if (onByDefault) {
      digitalWrite(i, invert ? LOW : HIGH);
    }
  }
  Serial.begin(9600);
  Serial.println("https://github.com/M0LTE/arduino-relay-control");
  Serial.println("waiting for commands in the form r01, e.g. relay 0 on");
}

int state; // 0 = waiting for r, 1 = waiting for relay number, 2 = waiting for 0/1
int pin;
byte read;

void loop() {
  if (Serial.available()) {
    read = Serial.read();
    if (read == '\r' || read == '\n'){
      return;
    }
    
    if (state == 0){
      if (read != 'r'){
        Serial.println("expected r");
        return;
      }
      state=1;
    } else if (state == 1){
      if (read < '0' || read > '0' + relays - 1) {  // '7'
        Serial.println("expected 0-7");
        state=0;
        return;
      } else {
        pin = read - '0';
        Serial.print(pin);
        Serial.print(" ");
      }
      state=2;
    } else if (state == 2){
      if (read != '0' && read != '1'){
        Serial.println("expected 0 or 1");
        state=0;
        return;
      } else {
        if (read == '0'){
          Serial.println("off");
          digitalWrite(pin + startPin, invert ? HIGH : LOW);
        } else {
          Serial.println("on");
          digitalWrite(pin + startPin, invert ? LOW : HIGH);
        }
        state=0;
      }
    }
  }
}
