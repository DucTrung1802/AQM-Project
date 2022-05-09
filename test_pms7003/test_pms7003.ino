//Library for UART and PMS7003 sensor
#include <SoftwareSerial.h>
#include "PMS.h"

SoftwareSerial serial_ESP(D2, D3);  // D2 = RX -- D3 = TX

//Variables to store sensors' values
volatile unsigned int pm1_0 = 0;
volatile unsigned int pm2_5 = 0;
volatile unsigned int pm10_0 = 0;
volatile unsigned long long timer = 0;

PMS pms(serial_ESP);
PMS::DATA data;

void setup() {
  // Open serial communications and wait for port to open:
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  Serial.begin(115200);
  serial_ESP.begin(9600);
}

void loop() {
  // run over and over
  // if (serial_ESP.available() > 0) {
  //   float dataRecive;
  //   dataRecive = serial_ESP.parseFloat();
  //   Serial.println(dataRecive);
  //  }
  if (pms.read(data)) {
    pm1_0 = data.PM_AE_UG_1_0;
    pm2_5 = data.PM_AE_UG_2_5;
    pm10_0 = data.PM_AE_UG_10_0;
  }

  if (millis() - timer >= 1000) {
    Serial.println("---");
    Serial.print("PM_1.0: ");
    Serial.println(pm1_0);
    Serial.print("PM_2.5: ");
    Serial.println(pm2_5);
    Serial.print("PM_10.0: ");
    Serial.println(pm10_0);
    Serial.println("---");

    timer = millis();
  };
}