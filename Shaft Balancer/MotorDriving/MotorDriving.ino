#include <SparkFun_TB6612.h>

//Motor Driver
#define AIN1 2
#define AIN2 4
#define PWMA 3
#define STBY 5

Motor motor1 = Motor(AIN1, AIN2, PWMA, 1, STBY); //MAKE 1 OR NEGATIVE 1 DEPENDING

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
}

void loop() {
  
  // put your main code here, to run repeatedly:
  motor1.drive(255);


}
