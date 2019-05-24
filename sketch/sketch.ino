#include "relay.h"
#include "soil_moisture.h"
#include "dht_sensor.h"
#include "ds18b20.h"
#include "read-serial.h"

void setup() {
  while (!Serial);
  Serial.begin(9600);

  Relay::setup();
  SoilMoisture::setup();
  DHT::setup();
  DS18B20::setup();

  ReadSerial::start();
}

void loop() {
}
