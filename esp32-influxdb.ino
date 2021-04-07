#include <WiFi.h>
#include <InfluxDbClient.h>
InfluxDBClient client_v1;

// SET WIFI INFO
#define WIFI_SSID "<wifi-name>" // Wifi
#define WIFI_KEY "<password>" // Wifi password
int status = WL_IDLE_STATUS;

// SET INFLUXDB INFO
#define INFLUXDB_URL "http://192.168.1.89:8086"
#define INFLUXDB_DATABASE "<database>"
#define INFLUXDB_USER "<user>"
#define INFLUXDB_PASSWORD "<password>"

// SET DEEP SLEEP
#define deep_sleep_time 30  // Sleep time in seconds.

void setup() {
  
  // START SERIAL PORT
  Serial.begin(9600);
  Serial.print('\n');
  Serial.print('\n');
  Serial.print('\n');
  delay(1000);

  // START WIFI
  int count = 0;
  int countMax = 10; // connection retry limit
  Serial.println("Connecting to Wifi...");
  status = WiFi.begin(WIFI_SSID, WIFI_KEY);
  while (WiFi.status() != WL_CONNECTED) {
    if (count > countMax){
      Serial.println("Failed to connect to Wifi...");
      Serial.println("Going to sleep...");
      esp_sleep_enable_timer_wakeup(deep_sleep_time * 1000000);
      esp_deep_sleep_start();   
    }
    Serial.print("Attempt: ");
    Serial.println(count);
    delay(1000);
    count++;
  }
  Serial.println("Connected to Wifi...");

  // SET DATA POINT
  long rssi = WiFi.RSSI();

  // SET MEASUREMENT(s)
  Point point("measurement");

  // SET FIELD(s) & VALUE(s)
  point.addField("rssi", rssi); // Wifi signal strength in RSSI.

  // SEND DATA
  Serial.println("Sending data to InfluxDB...");
  client_v1.setConnectionParamsV1(INFLUXDB_URL, INFLUXDB_DATABASE, INFLUXDB_USER, INFLUXDB_PASSWORD);
  client_v1.writePoint(point); // Write data to InfluxDB

  // DEEP SLEEP
  Serial.println("Going to sleep...");
  esp_sleep_enable_timer_wakeup(deep_sleep_time * 1000000);
  esp_deep_sleep_start();
  
}

void loop() {
  // nothing
}
