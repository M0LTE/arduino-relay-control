#include <DHT11.h>
#include <DS18B20.h>

// how many relays
const int relays = 8;

// relays connected to this pin number and up
const int startPin = 5; 

// invert relay state
const bool invert = false;

// turn on all outputs at startup
const bool onByDefault = true;

// this sketch supports a DHT11 temperature and humidity sensor and 
// a chain of DS18B20 temp sensors and outputs the value to the serial 
// port regularly
const int tempSensorPin = 2;
const int ds18b20Pin = 3;
const unsigned long TEMP_REFRESH_INTERVAL = 60000; // ms

DHT11 dht11(tempSensorPin);
DS18B20 ds(ds18b20Pin);
int temperature;
int humidity;

void readDs18b20Sensors(bool welcome){
  uint8_t address[8];
  while (ds.selectNext()) {
    ds.getAddress(address);
    if (welcome){
      Serial.print("found temp sensor; ");
    }
    for (int i=0; i<8; i++){
      Serial.print(address[i], HEX);
      if (i < 7){
        Serial.print(":");
      }
    }
    Serial.print(" ");
    Serial.print(ds.getTempC());
    Serial.println("C");
  }
}

void readDht11(bool welcome){
  int result = dht11.readTemperatureHumidity(temperature, humidity);
  if (result == 0){
    if (welcome){
      Serial.print("found temp/humidity sensor; ");
    }
    Serial.print(temperature);
    Serial.print("C ");
    Serial.print(humidity);
    Serial.println("%");
  } else {
    Serial.println(DHT11::getErrorString(result));
  }
}

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

  readDht11(true);
  readDs18b20Sensors(true);
}

void readTemperature()
{
  readDht11(false);
  readDs18b20Sensors(false);
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
