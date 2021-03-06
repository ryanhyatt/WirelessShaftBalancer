/* By Ryan Hyatt
 * Senior Design 2017-18
 * 
 */
#include <EEPROM.h>

//pins
const int encA_pin = 2; //TODO
const int encB_pin = 3; //TODO

int encA_val;
int encB_val;
int pos;
int off_pos_addr = 0;
int off_ticks_addr = 4; 

int pos_CW[] = {3, 0, 1, 2};
int pos_CCW[] = {1, 2, 3, 4};

int ticks = 0;

void setup() {
  Serial.begin(115200);
  //pos = EEPROM.read(off_pos_addr);
  //ticks = EEPROM.read(off_ticks_addr);
  pinMode(encA_pin, INPUT);  
  pinMode(encB_pin, INPUT);  
  
}
/* Determines the state the encoder is in. Encoder can be in one of any 4 spots
 */
int cyclePosition(int encA_val, int encB_val)
{
    int pos;
    if( encA_val == 0)
    {
      if(encB_val == 0)
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
      if(encB_val == 0)
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

void loop() {
  //TODO move cycleposition update and EEPROM write into interrupt
  //Store previous position.
  int old_pos = pos;

  attachInterrupt(digitalPinToInterrupt(encA_pin), pin_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encB_pin), pin_ISR, CHANGE);
 // EEPROM.write(ticks, ticks_off_addr);
}

void pin_ISR() {
  //Read Encoder Values
  int old_pos = pos;
  encA_val = digitalRead(encA_pin);
  encB_val = digitalRead(encB_pin);
  pos = cyclePosition(encA_val, encB_val);
  int dir = rotationDirection(pos, old_pos);
  ticks+=dir;
   Serial.print(" ticks = ");
  Serial.print(ticks);
  Serial.print(" old_pos = ");
  Serial.print(old_pos);
  Serial.print(" pos = ");
  Serial.print(pos);
  Serial.println("");
}

