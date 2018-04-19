
/*
  Getting Started example sketch for nRF24L01+ radios
  This is a very basic example of how to send data from one node to another
  Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"
#include <EEPROM.h>
#include <SparkFun_TB6612.h>

//https://forum.arduino.cc/index.php?topic=52159.0


/* setup stuff for the encoder */
//pins

//Motor Driver
#define AIN1 6
#define AIN2 7
#define PWMA 5
#define STBY 4

//Transceiver


//Encoder
#define ENCA 2
#define ENCB 3
volatile int ENCA_VAL;
volatile int ENCB_VAL;
int pos;
int ticks;
int TICKS_PER_DEGREE = 12 * 986.41 / 360;
int off_ticks_addr = 0;                   //store ticks on encoder

int pos_CW[] = {3, 0, 1, 2};
int pos_CCW[] = {1, 2, 3, 4};


/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9, 10);
/**********************************************************/
byte addresses[][6] = {"1Node", "2Node"};

int POWER;
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
  if ( ENCA_VAL == 0)
  {
    if (ENCB_VAL == 0)
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
    if (ENCB_VAL == 0)
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
  if (pos == pos_CW[old_pos])
  {
    return 1;
  }
  //if the position moved to a counterclockwise position
  else if (pos == pos_CCW[old_pos])
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

  /*Encoder*/
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  //EEPROM_readInteger(off_ticks_addr, ticks);
  int THETA; //NORMALLY BASE STATION
  int TICKS_PER_DEGREE = 12 * 986.41 / 360;
  ticks = 0;

  attachInterrupt(digitalPinToInterrupt(ENCA), pin_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCB), pin_ISR, CHANGE);
  /*Transceiver*/
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);

  // Open a writing and reading pipe on each radio, with opposite addresses

  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);

  // Start the radio listening for data
  radio.startListening();
}

void loop() {

  /*Encoder*/
  int old_pos = pos;
  pos = cyclePosition(ENCA_VAL, ENCB_VAL);
 // Serial.println(ENCA_VAL);
  int dir = rotationDirection(pos, old_pos);
  ticks += dir;
  EEPROM_writeInteger(off_ticks_addr, ticks);

  /*Handle communication with base station.*/
  if ( radio.available()) {
    // Variable for the received timestamp
    while (radio.available()) {                                   // While there is data ready
      radio.read(&POWER, sizeof(int));             // Get the payload
      radio.stopListening();                                        // First, stop listening so we can talk
      if (!radio.write(&ticks, sizeof(int))){
        Serial.println(F("failed"));
      }
      else{
        Serial.println(F("ticks sent!"));
      }
      radio.startListening();                                       // Now, resume listening so we catch the next packets.
      Serial.println(ticks);
    }
  }


  /*Update Motor Speed and Direction*/

  motor1.drive(POWER);

} // Loop

void pin_ISR() {
  //Read Encoder Values
  if(PIND & (1<<PD2)){
    ENCA_VAL = 1;
  }
  else
  {
    ENCA_VAL = 0;
  }
  if(PIND & (1<<PD3)){
    ENCB_VAL = 1;
  }
  else
  {
    ENCB_VAL = 0;
  } 
}

