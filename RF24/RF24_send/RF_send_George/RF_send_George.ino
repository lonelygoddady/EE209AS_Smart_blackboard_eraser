#include <SPI.h>
#include <string.h>
#include "RF24.h"

char *saveptr;
char *tok, *buf;
byte addresses[][6] = {"1Node","2Node"};

/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);

// this node is sending
bool role = 1;

struct myData{
  String msg;
  float value;  
}myData;

struct point_data{
  int x;
  int y;
}point_data;

struct myData feedback;
struct point_data coord1_data; 
struct point_data coord2_data; 
struct point_data coord1_rec; 
struct point_data coord2_rec; 

void setup() {

  Serial.begin(9600);
  Serial.println("RF 24 transfer initialize!");
  Serial.println("Please specify values to be sent");
  radio.begin();
  
  coord1_data.x = 0;
  coord1_data.y = 0;
  coord2_data.x = 0;
  coord2_data.y = 0;

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]); //writing: 2Node
    radio.openReadingPipe(1,addresses[0]); //reading: 1Node
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
 
  // Start the radio listening for data
  radio.startListening();
}


void loop() {
 
if (role == 1)  {
    if ( Serial.available() )
    {
      String coord = Serial.readString(); //should be in this format x1,x2,y1,y2,
      parseString(coord);
      
      Serial.println("First point:") ;
      Serial.println(coord1_data.x);
      Serial.println(coord1_data.y);
       Serial.println("Second point:"); 
      Serial.println(coord2_data.x);
      Serial.println(coord2_data.y);
      radio.stopListening();                                    
      radio.write(&coord1_data, sizeof(point_data));
      radio.write(&coord2_data, sizeof(point_data));
      radio.startListening();                               
    }
    if (radio.available()){
        radio.read( &coord1_rec, sizeof(point_data));
        delay(100);
        radio.read( &coord2_rec, sizeof(point_data));
        delay(100);
        if (coord1_rec.x == coord1_data.x &&  coord1_rec.y == coord1_data.y &&  coord2_rec.x == coord2_data.x && coord2_rec.y == coord2_data.y)
          Serial.println("Data successfully receive!");
         else
          Serial.println("transmission failed!"); 
    }

    // Try again .1s later
    delay(100);
  }
  
if (role == 0)  {
    if (radio.available()){
        radio.read( &coord1_data, sizeof(point_data));
        delay(100);
        radio.read( &coord2_data, sizeof(point_data));
        delay(100);

        radio.write(&coord1_data, sizeof(point_data));
        radio.write(&coord2_data, sizeof(point_data));
    }
    delay(100);
  }  
} 

void parseString(String myString){
 int index1 = myString.indexOf(',');
 int index2 = myString.indexOf(',',index1+1);
 int index3 = myString.indexOf(',',index2+1);
 int index4 = myString.indexOf(',',index3+1);
 
 String value1 = myString.substring(0,index1);
 String value2 = myString.substring(index1+1,index2);
 String value3 = myString.substring(index2+1,index3);
 String value4 = myString.substring(index3+1,index4);
 
 coord1_data.x = value1.toInt();
 coord2_data.y = value2.toInt();
 coord1_data.x = value3.toInt();
 coord2_data.y = value4.toInt();
}

