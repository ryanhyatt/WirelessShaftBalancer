
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"

int POSITION = 12;

int POWER;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9,10);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

void setup() {

  Serial.begin(115200);
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  
  // Start the radio listening for data
  radio.startListening();
  POWER = 0;
}

void loop() {
  
  
/****************** Ping Out Role ***************************/  
                                      // First, stop listening so we can talk.
  
  
  radio.stopListening();

  /*Read Serial Input*/
  if(Serial.available())
  {
    String inString = Serial.readString();
    POWER = inString.toInt();
    Serial.println(POWER);
  }
    
  Serial.println(F("Now sending"));
  
  //Send Power level
   if (!radio.write(&POWER, sizeof(int))){
     Serial.println(F("failed"));
   }
   else{
      Serial.println(F("Message sent!"));
   }  
      
  radio.startListening();                                    // Now, continue listening
  
  unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
  boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not

  while ( ! radio.available() ){                             // While nothing is received
    if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
        timeout = true;
        break;
    }      
  }
      
  if ( timeout ){                                             // Describe the results
      Serial.println(F("Failed, response timed out."));
  }else{
      Serial.println("SIGNAL AVAILABLE");
      radio.read( &POSITION, sizeof(int) );
      Serial.println(POSITION);  }

  // Try again .5 later
  delay(500);

} // Loop

