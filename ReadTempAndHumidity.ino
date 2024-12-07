
#include <DHT11.h>
#define humidityPin 2
// - For Arduino: Connect the sensor to Digital I/O Pin 2.

DHT11 dht11(humidityPin);

void setup() {
    Serial.begin(9600);
    dht11.setDelay(500); 
}

void loop() {
    int temperature = 0;
    int humidity = 0;
    int result = dht11.readTemperatureHumidity(temperature, humidity);
    if (result == 0) {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print(" °C\tHumidity: ");
        Serial.print(humidity);
        Serial.println(" %");
    } 
}