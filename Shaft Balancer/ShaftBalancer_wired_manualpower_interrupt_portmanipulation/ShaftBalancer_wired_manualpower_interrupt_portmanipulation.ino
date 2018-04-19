  
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include <EEPROM.h>
#include <SparkFun_TB6612.h>

//https://forum.arduino.cc/index.php?topic=52159.0


//pins

//Motor Driver
#define AIN1 6
#define AIN2 7
#define PWMA 5
#define STBY 4

//Encoder
#define ENCA 2 //TODO
#define ENCB 3 //TODO

#define THETA_RANGE 5

// string to hold input

/* setup stuff for the encoder */
volatile int ENCA_VAL;
volatile int ENCB_VAL;
int pos;
int ticks;
int THETA; //NORMALLY BASE STATION
int TICKS_PER_DEGREE = 12 * 986.41;

int off_ticks_addr = 0;
int pos_CW[] = {3, 0, 1, 2};
int pos_CCW[] = {1, 2, 3, 4};

Motor motor1 = Motor(AIN1, AIN2, PWMA, 1, STBY); //MAKE 1 OR NEGATIVE 1 DEPENDING

int POWER;
/* Convenience Functions */

int EEPROM_writeInteger(int ee, int value)
{
    const byte* p = (byte*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);
    return i;
}

int EEPROM_readInteger(int ee, int value)
{
    byte* p = (byte*)&value; //cast as pointer to byte
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);
    return i;
}

int cyclePosition(int ENCA_VAL, int ENCB_VAL)
{
    int pos;
    if( ENCA_VAL == 0)
    {
      if(ENCB_VAL == 0)
      {
        pos = 0;
      }
      else
      {
        pos = 3;
      }
    }
    else
    {
      if(ENCB_VAL == 0)
      {
        pos = 1;
      }
      else
      {
        pos = 2;
      }
    }
    return pos;
}

int rotationDirection(int pos, int old_pos)
{
  if(pos == pos_CW[old_pos])
  {
    return 1;
  }
  //if the position moved to a counterclockwise position
  else if(pos == pos_CCW[old_pos])
  {
    return -1;
  }
  else
  {
    return 0;
  }
}

void setup() {
  Serial.begin(115200);

  //load previous position
  
  pinMode(ENCA, INPUT);  
  pinMode(ENCB, INPUT);  
  EEPROM_readInteger(off_ticks_addr, ticks);
  attachInterrupt(digitalPinToInterrupt(ENCA), pin_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCB), pin_ISR, CHANGE);

}

void loop() {

    //POWER = 255;
  /*Check for and input new commands.*/
  // Read serial input:
  //Serial.println("TEST");
  ///THIS IS IMPORTANT
  //http://forum.arduino.cc/index.php?topic=181536.0

  int old_pos = pos;
  pos = cyclePosition(ENCA_VAL, ENCB_VAL);
  int dir = rotationDirection(pos, old_pos);
  ticks+=dir;
  Serial.print(" ticks = ");
  Serial.print(ticks);
  Serial.print(" old_pos = ");
  Serial.print(old_pos);
  Serial.print(" pos = ");
  Serial.print(pos);
  Serial.println("");
  
  if(Serial.available())
  {
    String inString = Serial.readString();
    POWER = inString.toInt();
    Serial.println("power: ");
    Serial.print(POWER);
  }
  
  /*Update Motor Speed and Direction*/
    
  motor1.drive(POWER);

} // Loop

void pin_ISR() {
  //Read Encoder Values
  ENCA_VAL = digitalRead(ENCA);
  ENCB_VAL = digitalRead(ENCB);
}

