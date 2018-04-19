int enablePin = 5;

void setup()
{
  pinMode(enablePin, OUTPUT);
}
void loop()
{
  digitalWrite(enablePin, HIGH);
}
