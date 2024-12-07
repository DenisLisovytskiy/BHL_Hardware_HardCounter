#define panicPin 1
#define maxtimebetweenpresses 10000//10seconds
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(panicPin, INPUT);
}

int timespressed=0;
float lastpressed=0;

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(panicPin))
  {
    timespressed++;
    lastpressed= millis();
    
    Serial.println("pressed");
  }
  if(3<=timespressed)
  {
    Serial.println("Panic!!!");


  }
  if(millis()-lastpressed >=maxtimebetweenpresses)
  {

    timespressed=0;
  }
  delay(200);
}
