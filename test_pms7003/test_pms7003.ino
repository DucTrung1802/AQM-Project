//Library for UART and PMS7003 sensor
#include <SoftwareSerial.h>
#include "PMS.h"

SoftwareSerial serial_ESP(D2,D3); // D2 = RX -- D3 = TX

PMS pms(serial_ESP);
PMS::DATA data;
void setup() {
  // Open serial communications and wait for port to open:
  pinMode(D2,INPUT);
  pinMode(D3,OUTPUT);
  Serial.begin(115200);
  serial_ESP.begin(115200);
}

void loop() { // run over and over
  if (serial_ESP.available() > 0) {
    float dataRecive;
    dataRecive = serial_ESP.parseFloat();
    Serial.println(dataRecive);
   }
}