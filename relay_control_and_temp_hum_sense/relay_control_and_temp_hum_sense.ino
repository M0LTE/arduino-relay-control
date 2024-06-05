#include <DHT11.h>

// how many relays
const int relays = 8;

// relays connected to this pin number and up
const int startPin = 3; 

// invert relay state
const bool invert = false;

// turn on all outputs at startup
const bool onByDefault = true;

// this sketch supports a DHT11 temperature and humidity sensor and 
// outputs the value to the serial port regularly
const int tempSensorPin = 2;
const unsigned long TEMP_REFRESH_INTERVAL = 60000; // ms

DHT11 dht11(tempSensorPin);
int temperature;
int humidity;

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

  int result = dht11.readTemperatureHumidity(temperature, humidity);
  if (result == 0){
    Serial.print(temperature);
    Serial.print("C ");
    Serial.print(humidity);
    Serial.println("%");
  } else {
    Serial.println(DHT11::getErrorString(result));
  }
}

void readTemperature()
{
  int result = dht11.readTemperatureHumidity(temperature, humidity);
  if (result == 0){
    Serial.print(temperature);
    Serial.print("C ");
    Serial.print(humidity);
    Serial.println("%");
  } else {
    Serial.println(DHT11::getErrorString(result));
  }
}

void doTemperature()
{
  static unsigned long lastRefreshTime = 0;

  if(millis() - lastRefreshTime >= TEMP_REFRESH_INTERVAL) {
    lastRefreshTime += TEMP_REFRESH_INTERVAL;
    readTemperature();
  }
}

void loop() {
  
  doTemperature();

  static int state; // 0 = waiting for r, 1 = waiting for relay number, 2 = waiting for 0/1
  static int pin;
  static byte read;

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
    } else if (state == 1) {
      if (read < '0' || read > '0' + relays - 1) {  // '7'
        Serial.println("expected 0-7");
        state=0;
      } else {
        pin = read - '0';
        state = 2;
      }
      return;
    } else if (state == 2) {
      if (read != '0' && read != '1'){
        Serial.println("expected 0 or 1");
        state=0;
        return;
      } else {
        Serial.print("relay ");
        Serial.print(pin);
        if (read == '0'){
          Serial.println(" off");
          digitalWrite(pin + startPin, invert ? HIGH : LOW);
        } else {
          Serial.println(" on");
          digitalWrite(pin + startPin, invert ? LOW : HIGH);
        }
        state=0;
      }
    }
  }
}
