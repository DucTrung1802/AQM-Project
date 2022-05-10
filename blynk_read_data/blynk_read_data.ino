/*************************************************************

  You’ll need:
   - Blynk IoT app (download from App Store or Google Play)
   - ESP8266 board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings



// DHT11 LIBRARIES
#include "DHT.h"

// PMS7003 LIBRARIES

// BLYNK PARAMTERS
#define BLYNK_TEMPLATE_ID "TMPLxxxxxx"
#define BLYNK_DEVICE_NAME "ESP8266"
#define BLYNK_AUTH_TOKEN "JB5gv1liwyq91VWaxzwxcSavHcEYt49-"
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

// BLYNK LIBRARIES (MUST BE INCLUDE AFTER #define BLYNK)
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Quang Duc";
char pass[] = "tranquangduc173";

// DHT11 PARAMETERS
#define DHTPIN 5       // what digital pin we're connected to
#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN, DHTTYPE);
long dht11_timer = millis();
const int dht11_period = 1000;  // ms
float humidity = 0;
float temp_c = 0;
float temp_f = 0;

void setupBlynk() {
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void setupDHT11() {
  dht.begin();
}

void readDHT11() {
  if (millis() - dht11_timer >= dht11_period) {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    humidity = dht.readHumidity();
    // Read temperature as Celsius (the default)
    temp_c = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    temp_f = dht.readTemperature(true);
    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temp_c) || isnan(temp_f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    dht11_timer = millis();
  }
}

void displayDHT11() {
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, temp_c);
  Blynk.virtualWrite(V3, temp_f);
  // Serial.print("Humidity: ");
  // Serial.print(humidity);
  // Serial.print(" %\t");
  // Serial.print("Temperature: ");
  // Serial.print(temp_c);
  // Serial.print(" *C ");
  // Serial.println();
}

void setup() {
  // Debug console
  Serial.begin(115200);
  setupBlynk();
  setupDHT11();
}

void loop() {
  Blynk.run();
  readDHT11();
  displayDHT11();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
