const int trigPin = 9;
const int echoPin = 10;
const int laserPin = 12;
const int buttonPin = 2;

// defines variables
long duration;
int distance;
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(laserPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600); // Starts the serial communication
}
void loop() {
  if(digitalRead(buttonPin))
  {
    // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  digitalWrite (laserPin, HIGH); // open the laser head
  delay(1000);
  }
  else
  {
    digitalWrite (laserPin, LOW);
      //not measuring dist
  }
    delay(100);

  
}