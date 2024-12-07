void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float pulse;
  int sum =0;

  for(int i = 0; i<100;i++)
  {
    sum+= analogRead(A0);
  }
  pulse = sum/100.0;
  Serial.println(pulse);
  delay(1000);
}
