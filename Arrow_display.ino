#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int rotationAngle = 0;
int rectWidth = 20; // Rectangle width
int rectHeight = 15; // Rectangle height
int centerX = 102; // Center of the rectangle
int centerY = 32; // Center of the rectangle

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Rotate the screen upside down (180 degrees)
  display.setRotation(2);  // Set rotation to 180 degrees (upside down)
  
  // Set text size and color for printing
  display.setTextSize(3); // Small text size
  display.setTextColor(SSD1306_WHITE); // White text
}

void loop() {
  // Clear display before drawing new frame
  display.clearDisplay();

  // Draw the rotating filled rectangle and arrowhead
  drawFilledRotatingRectangle(rotationAngle);

  // Print the question mark "?" on the screen
  display.setCursor(2, 2);  // Position at the top-left corner
  display.print("I<3U");

  // Update the display with new content
  display.display();

  delay(100); // Show the frame for 1 second //prev 1000

  // Rotate the rectangle and arrowhead by 45 degrees every second
  rotationAngle += 5; //prev 45 
  if (rotationAngle >= 360) {
    rotationAngle = 0; // Reset angle to 0 after completing a full rotation
  }
}

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
