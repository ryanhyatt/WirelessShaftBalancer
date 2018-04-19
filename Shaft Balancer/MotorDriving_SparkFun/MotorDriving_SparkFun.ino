int pinPHASE = 2;
int pinPOWER = 5;
int pinENABLE = 4 ;
int POW;
bool PHASE;
char buf_input[3];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pinPHASE, OUTPUT);
  pinMode(pinPHASE, OUTPUT);
  pinMode(pinPOWER, OUTPUT);

  
}

void loop() {
  
  // put your main code here, to run repeatedly:
  //Serial.println("stuff");

  digitalWrite(pinENABLE, HIGH);
  digitalWrite(pinPHASE, HIGH);
  digitalWrite(pinPOWER, HIGH);

}
