#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

RF24 radio(9,53);
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
String data="";

void setup(void)
{

  Serial.begin(9600);
  printf_begin();
  printf("Getting started");
  radio.begin();
  radio.setRetries(15,15);
  radio.openReadingPipe(1,pipes[1]);
  radio.startListening();
  radio.printDetails();
}

void loop(void)
{
  
   radio.openWritingPipe(pipes[1]);
   radio.openReadingPipe(1,pipes[0]);
  //
  // Pong back role.  Receive each packet, dump it out, and send it back
  //
    // if there is data ready
    if ( radio.available() )
    {
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( &data, sizeof(unsigned long) );

        delay(20);
      }

      // First, stop listening so we can talk
      radio.stopListening();

      // Send the final one back.
      Serial.print(data);
      // Now, resume listening so we catch the next packets.
      radio.startListening();
    }

}
// vim:cin:ai:sts=2 sw=2 ft=cpp
