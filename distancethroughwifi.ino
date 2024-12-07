#include <WiFiNINA.h>

const char* ssid = "S24feWIFI";
const char* password = "12345679";

void setup() {
  Serial.begin(9600);

  // Check if the WiFi module is present
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  // Attempt to connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
}

void loop() {
  int rssi = WiFi.RSSI();
  float distance = rssiToDistance(rssi);
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  //Serial.println(rssi);
  delay(1000);
}


float rssiToDistance(float rssi) {
  //algo to calculate distance from signal strength
  float distance = pow(10, (27 - rssi) / 100.0)*10;
  return distance;
}
