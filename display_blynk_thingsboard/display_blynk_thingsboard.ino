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

// BLYNK PARAMTERS
#define BLYNK_TEMPLATE_ID "TMPLxxxxxx"
#define BLYNK_DEVICE_NAME "ESP8266"
#define BLYNK_AUTH_TOKEN "JB5gv1liwyq91VWaxzwxcSavHcEYt49-"
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

// DHT11 LIBRARIES
#include "DHT.h"

// PMS7003 LIBRARIES
#include <SoftwareSerial.h>
#include "PMS.h"

// THINGSBOARD LIBRARIES
#include <ESP8266WiFi.h>
#include <ThingsBoard.h>

// BLYNK LIBRARIES (MUST BE INCLUDE AFTER #define BLYNK)
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// WIFI PARAMETERS
// Set WIFI_PASSWORD word to "" for open networks.
char WIFI_SSID[] = "Quang Duc";
char WIFI_PASSWORD[] = "tranquangduc173";
WiFiClient wifiClient;
int status = WL_IDLE_STATUS;

// BLYNK PARAMETERS
char auth[] = BLYNK_AUTH_TOKEN;

// THINSGBOARD PARAMETERS
#define TOKEN "vrQLv6rHQlOMc5bJLPXz"
char thingsboardServer[] = "demo.thingsboard.io";
ThingsBoard tb(wifiClient);

// DHT11 PARAMETERS
#define DHTPIN 5       // what digital pin we're connected to
#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN, DHTTYPE);
volatile float humidity = 0;
volatile float temp_c = 0;
volatile float temp_f = 0;
volatile unsigned long dht11_timer = millis();
const int dht11_period = 1000;  // ms

// PMS7003 PARAMETERS
SoftwareSerial serial_ESP(D2, D3);  // D2 = RX -- D3 = TX
PMS pms(serial_ESP);
PMS::DATA data;
volatile unsigned int pm1_0 = 0;
volatile unsigned int pm2_5 = 0;
volatile unsigned int pm10_0 = 0;
volatile unsigned long pms7003_timer = 0;
const int pms7003_period = 1000;  // ms

void setupBlynk() {
  Blynk.begin(auth, WIFI_SSID, WIFI_PASSWORD);
  // You can also specify server:
  //Blynk.begin(auth, WIFI_SSID, WIFI_PASSWORD, "blynk.cloud", 80);
  //Blynk.begin(auth, WIFI_SSID, WIFI_PASSWORD, IPAddress(192,168,1,100), 8080);
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

void displayBlynkDHT11() {
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

void pushDataDHT11() {
  if (millis() - dht11_timer >= dht11_period) {
    tb.sendTelemetryFloat("humidity", humidity);
    tb.sendTelemetryFloat("temperature_c", temp_c);
    tb.sendTelemetryFloat("temperature_f", temp_f);
    dht11_timer = millis();
  }
}

void sendNotifications() {
  if (humidity >= 75) {
    Blynk.email("{DEVICE_NAME} - High Humidity", "Humidity exceeds 75 %");
    Blynk.notify("{DEVICE_NAME} - High Humidity - Humidity exceeds 75 %");
  }
  if (temp_c >= 30) {
    Blynk.email("{DEVICE_NAME} - High Temperature", "Humidity exceeds 30 °C");
    Blynk.notify("{DEVICE_NAME} - High Temperature - Humidity exceeds 30 °C");
  }
}

void setupPMS7003() {
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  serial_ESP.begin(9600);
}

void readPMS7003() {
  if (millis() - pms7003_timer >= pms7003_period) {
    if (pms.read(data)) {
    pm1_0 = data.PM_AE_UG_1_0;
    pm2_5 = data.PM_AE_UG_2_5;
    pm10_0 = data.PM_AE_UG_10_0;
    }

    pms7003_timer = millis();
  }
}

void pushDataPMS7003() {
  if (millis() - pms7003_timer >= pms7003_period) {
    tb.sendTelemetryFloat("pm_1.0", pm1_0);
    tb.sendTelemetryFloat("pm_2.5", pm2_5);
    tb.sendTelemetryFloat("pm_10.0", pm10_0);
    pms7003_timer = millis();
  }
}

void displayBlynkPMS7003() {
  Blynk.virtualWrite(V4, pm1_0);
  Blynk.virtualWrite(V5, pm2_5);
  Blynk.virtualWrite(V6, pm10_0);
}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  while (!tb.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    if ( tb.connect(thingsboardServer, TOKEN) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED]" );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}

void connectToThingsboard() {
    if ( !tb.connected() ) {
    reconnect();
  }
}

void setup() {
  // Debug console
  Serial.begin(115200);
  InitWiFi();
  setupBlynk();
  setupDHT11();
  setupPMS7003();
}

void loop() {  
  Blynk.run();
  connectToThingsboard();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!

  readDHT11();
  displayBlynkDHT11();
  pushDataDHT11();

  readPMS7003();
  displayBlynkPMS7003();
  pushDataPMS7003();

  sendNotifications();
  
  tb.loop();
}
