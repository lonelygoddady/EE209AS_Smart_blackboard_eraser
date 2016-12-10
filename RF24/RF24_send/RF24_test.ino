#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
//Mega 2560, 9,53,52,51,50

RF24 radio(9,53);
String data="hello world";
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };


void setup(void)
{
  //
  // Print preamble
  //

  Serial.begin(9600);
  printf_begin();
  printf("Getting started");

  //
  // Setup and configure rf radio
  //

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);

 
    radio.openReadingPipe(1,pipes[1]);
    radio.startListening();

    radio.printDetails();
}

void loop(void)
{
  //
  // Ping out role.  Repeatedly send the current time
  //
    // First, stop listening so we can talk.
    radio.stopListening();
    
    bool ok = radio.write( &s, sizeof(unsigned long) );
    
    if (ok)
      printf("ok...");
    else
      printf("failed.\n\r");

    // Now, continue listening
    radio.startListening();

    // Try again 1s later
    delay(1000);
 

      radio.openWritingPipe(pipes[0]);
      radio.openReadingPipe(1,pipes[1]);
    
  }
// vim:cin:ai:sts=2 sw=2 ft=cpp
