  
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include <EEPROM.h>
#include <SparkFun_TB6612.h>

//https://forum.arduino.cc/index.php?topic=52159.0


/* setup stuff for the encoder */
//pins

//Motor Driver
#define AIN1 2
#define AIN2 4
#define PWMA 5
#define STBY 9

//Encoder
#define ENCA 7 //TODO
#define ENCB 6 //TODO

#define THETA_RANGE 5

String inString = "";    // string to hold input

int ENCA_VAL;
int ENCB_VAL;
int pos;
int ticks;
int THETA; //NORMALLY BASE STATION
int TICKS_PER_DEGREE = ;

int off_ticks_addr = 0;
int pos_CW[] = {3, 0, 1, 2};
int pos_CCW[] = {1, 2, 3, 4};

Motor motor1 = Motor(AIN1, AIN2, PWMA, 1, STBY); //MAKE 1 OR NEGATIVE 1 DEPENDING

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

}

void loop() {

    int TARGET_THETA;
    int POWER = 255;
    int old_pos = pos;
  /*Record current position.*/
    ENCA_VAL = digitalRead(ENCA);
    ENCB_VAL = digitalRead(ENCB);
    pos = cyclePosition(ENCA_VAL, ENCB_VAL);
    int dir = rotationDirection(pos, old_pos);
    ticks+=dir;
    EEPROM_writeInteger(off_ticks_addr, ticks);
  

  /*Check for and input new commands.*/
  // Read serial input:
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      // clear the string for new input:
      TARGET_THETA = inString.toInt();
      Serial.println("New Target_Theta Set: ");
      Serial.print(TARGET_THETA);
      inString = "";
    }
  }

  /*Calculate Motor Speed and Direction*/
  THETA = ticks / TICKS_PER_DEGREE;
  if(THETA == TARGET_THETA)
  {
    //arrived
    //Serial.println("Motor at target!");
    POWER = 0;
  }
  else
  {
    //move TODO: MOVE TOWARDS THE TARGET ANGLE
    POWER = 255; //FULL SPEED AHEAD
  }
  
  /*Update Motor Speed and Direction*/
    
    motor1.drive(POWER);

} // Loop

