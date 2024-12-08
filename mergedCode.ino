// OLED BEGIN INCLUDE
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT11.h>
#include <Arduino_LSM6DS3.h>
#include <WiFiNINA.h>



#define humidityPin 0
#define panicPin 1
#define laserPin  2
#define diodePin 3
#define buzzerPin 4
#define laserButtonPin 5

#define trigPin  9
#define echoPin 10

//arrow : 
int rotationAngle = 0;
int rectWidth = 20; // Rectangle width
int rectHeight = 15; // Rectangle height
int centerX = 64; // Center of the rectangle
int centerY = 32; // Center of the rectangle
bool leftOrRight = true; //f = left 


long duration;
int distance;

int indicator = 0;

const char* ssid = "S24feWIFI";
const char* password = "12345679";

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// OLED END 

#define maxtimebetweenpresses 10000 // 10 seconds

DHT11 dht11(humidityPin);

int timespressed = 0;
unsigned long lastpressed = 0;

void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Infinite loop if display initialization fails
  }
  display.setRotation(2);  // 180 degrees (upside down)
  display.setTextColor(SSD1306_WHITE); // White text
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (true); // Stop execution if IMU initialization fails
  }

    display.setTextSize(1);
    display.clearDisplay();
   if (WiFi.status() == WL_NO_MODULE) {
    display.println("Communication with WiFi module failed!");
    display.display();
    while (true);
  }

  // Attempt to connect to WiFi network
  display.setCursor(0,0);
  display.clearDisplay();
  display.print("Connecting to ");
  display.println(ssid);
  display.display();
  delay(1000);
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(1000);
    display.print(".");
    display.display();
  }
  display.clearDisplay();
  display.println("\nConnected to WiFi");
  display.display();
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(laserPin, OUTPUT);
  pinMode(laserButtonPin, INPUT_PULLUP);
  pinMode(panicPin, INPUT);
  pinMode(diodePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(A0, INPUT);//pulse
  dht11.setDelay(2000);

  display.clearDisplay();
  display.setTextSize(3); // Small text size

}

void InitiatePanic() {
  if (digitalRead(panicPin)) {
    timespressed++;
    lastpressed = millis();
    Serial.println("pressed");
    delay(100);
  }
  if (timespressed >= 3) {
    Serial.println("Panic!!!");
    delay(200);
    LeavePanic();
  }
  if (millis() - lastpressed >= maxtimebetweenpresses) {
    timespressed = 0;
  }
  delay(200);
}

void LeavePanic() {
  while (1) {
    digitalWrite(diodePin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    display.clearDisplay();
    display.setCursor(2, 2);  // Position at the top-left corner
    display.setTextSize(3);
    display.print("DANGER!");
    display.display();
    delay(100);

    if (timespressed <= 0) {
      digitalWrite(diodePin, LOW);
      display.clearDisplay();
      display.display();
      Serial.println("Unpanicked");
      break;
    }
    if (digitalRead(panicPin)) {
      timespressed--;
      lastpressed = millis();
      Serial.println("pressed");
      delay(200);//here
    }
    if (millis() - lastpressed >= maxtimebetweenpresses) {
      timespressed = 3;
    }
    digitalWrite(diodePin, LOW);
    digitalWrite(buzzerPin, LOW);
    indicator = 10;//???
    delay(100);
  }
}

void TempHumidityG() {
  float gs = getG();
  int temperature = 0;
  int humidity = 0;
  int result = dht11.readTemperatureHumidity(temperature, humidity);
  if (result == 0) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print("Temp: ");
    display.println(temperature);
    display.print("Hum: ");
    display.print(humidity);
    display.println(" %");
    display.print("GS: ");
    display.println(gs);
    display.print("Pulse: ");
    GetPulse();
    display.display();
    delay(100);
  }
  if (humidity >= 80 || temperature >= 50 || gs > 5) {
    LeavePanic();
  }
}

float getG() {
  float x, y, z;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    return sqrt(x * x + y * y + z * z);
  }
  return 0;
}

void GetDistance()
{
 while(1){
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2;
    // Prints the distance on the Serial Monitor
    display.clearDisplay();
    display.setTextSize(2); // Small text size
    display.setCursor(0, 0);
    display.print("Distance: ");
    display.print(distance);
    display.display();
    digitalWrite (laserPin, HIGH); // open the laser head
    delay(500);
    if(!digitalRead(laserButtonPin))
    {
      indicator++;
      display.clearDisplay();
      display.display();
      delay(1000);
      break;
    }
    
 }
  
}

float rssiToDistance(float rssi) {
  //algo to calculate distance from signal strength
  float distance = pow(10, (27 - rssi) / 100.0)*10;
  return distance;
}

void WifiDistance()
{
   display.clearDisplay();
  display.display();
  while(1){
    // Sets the trigPin on HIGH state for 10 micro seconds
      display.clearDisplay();
    int rssi = WiFi.RSSI();
  float WIFIdistance = rssiToDistance(rssi);
 
  display.setCursor(0,0);
  display.print("Distance: ");
  display.print(WIFIdistance);
  display.println(" cm");
  display.display();
  delay(100);
    if(!digitalRead(laserButtonPin))
    {
      indicator++;
      display.clearDisplay();
      display.display();
      delay(1000);
      break;
    }
 }
}

void GetPulse()
{
  float pulse;
  int sum =0;

  for(int i = 0; i<100;i++)
  {
    sum+= analogRead(A0);
  }
  pulse = sum/100.0;
  Serial.println(pulse);
  pulse = abs(pulse);
  pulse = min(114.6, pulse);
  pulse = max(57.2, pulse);
  display.print(pulse);
}

void loop() {
  digitalWrite(diodePin, LOW);
  digitalWrite(buzzerPin, LOW);
  digitalWrite (laserPin, LOW);
  
  if(!digitalRead(laserButtonPin))indicator++;
  indicator = indicator % 4;
  InitiatePanic();
  switch (indicator){
    default:
    indicator = 0;
    break;
  case 0:
  TempHumidityG();
  break;
  case 1:
  GetDistance();
  break;
  case 2:
  WifiDistance();
  break;
  case 3:
   while(1){
    display.clearDisplay();
   // Draw the rotating filled rectangle and arrowhead
   drawFilledRotatingRectangle(rotationAngle);
  display.display();
  delay(100); // Show the frame for 1 second //prev 1000
  // Rotate the rectangle and arrowhead by 45 degrees every second
  if(leftOrRight)
  rotationAngle += 5; //prev 45 
  else rotationAngle -= 5;

  if(rotationAngle>90)leftOrRight = false; 
  if(rotationAngle< -90)leftOrRight = true; 
  if (rotationAngle >= 360) {
    rotationAngle = 0; // Reset angle to 0 after completing a full rotation
  }
    if(!digitalRead(laserButtonPin))
    {
      indicator++;
      display.clearDisplay();
      display.display();
      delay(1000);
      break;
    }
    
 }
  break;
  }
  
}

// arrow : 
void drawFilledRotatingRectangle(int angle) {
  // Convert angle to radians
    // Adjust the angle to rotate the arrowhead
  angle = angle - 90;  // Rotate by 90 degrees counterclockwise
  float rad = angle * 3.14159 / 180;

  // Calculate the rectangle's corners
  int x0 = centerX + rectWidth / 2 * cos(rad) - rectHeight / 2 * sin(rad);
  int y0 = centerY + rectWidth / 2 * sin(rad) + rectHeight / 2 * cos(rad);

  int x1 = centerX - rectWidth / 2 * cos(rad) - rectHeight / 2 * sin(rad);
  int y1 = centerY - rectWidth / 2 * sin(rad) + rectHeight / 2 * cos(rad);

  int x2 = centerX - rectWidth / 2 * cos(rad) + rectHeight / 2 * sin(rad);
  int y2 = centerY - rectWidth / 2 * sin(rad) - rectHeight / 2 * cos(rad);

  int x3 = centerX + rectWidth / 2 * cos(rad) + rectHeight / 2 * sin(rad);
  int y3 = centerY + rectWidth / 2 * sin(rad) - rectHeight / 2 * cos(rad);

  // Fill the rectangle (polygon) by connecting the corners
  display.fillTriangle(x0, y0, x1, y1, x2, y2, SSD1306_WHITE);
  display.fillTriangle(x0, y0, x2, y2, x3, y3, SSD1306_WHITE);

  // Draw the arrowhead (head part)
  int headLength = 20; // Arrowhead length


  

  // Calculate the base of the arrowhead (starting point)
  int X1 = centerX + headLength * cos(rad);
  int Y1 = centerY + headLength * sin(rad);

  // Calculate the two points of the arrowhead (forming the triangle)
  int hX2 = X1 - headLength * sin(rad + 3.14159 / 4);
  int hY2 = Y1 + headLength * cos(rad + 3.14159 / 4);

  int hX3 = X1 + headLength * sin(rad - 3.14159 / 4);
  int hY3 = Y1 - headLength * cos(rad - 3.14159 / 4);

  // Draw the arrowhead using a triangle
  display.fillTriangle(X1, Y1, hX2, hY2, hX3, hY3, SSD1306_WHITE);
}
