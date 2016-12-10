#include<Servo.h>
#include <SPI.h>
#include <string.h>
#include <RF24.h>
//#include <nRF24L01.h>

#define x_l 4950
#define y_l 4800
#define reso 10

char *saveptr;
char *tok, *buf;
byte addresses[][6] = {"1Node","2Node"};

/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

// this node is sending
bool role = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);

struct myData{
  String msg;
  float value;  
}myData;

//32 bits in size (max is 32 bit)
struct point_data{
  int x;
  int y;
}point_data;

struct myData feedback;
struct point_data coord1_data; 
struct point_data coord2_data; 
struct point_data coord1_rec; 
struct point_data coord2_rec; 

const int stepPin_1 = 5;
const int dirPin_1 = 6;
const int stepPin_2 = 2;
const int dirPin_2 = 4;
const int switch_1 = 1;
const int switch_2 = 12; //x = 0
const int switch_3 = 13; //y = x_l
const int switch_4 = 10; //y = 0
const int servo = 9;

int buttonState_1 = 0;
int b1_c = 0;
int buttonState_2 = 0;
int b2_c = 0;
int buttonState_3 = 0;
int b3_c = 0;
int buttonState_4 = 0;
int b4_c = 0;
Servo myservo;

String Sbuffer;
int input;
int count = 0;
int x_pre = 0;
int y_pre = 0;
int x_cur;
int y_cur;

void Reset(int x, int y);
void Reset_1();
void Eraser(int x1, int x2, int y1, int y2);
void move_x(int x);
void move_y(int y);
int coord_x_trans(int x);
int coord_y_trans(int y);
bool sw(int switch_1);
void z_release();
void z_push();

void setup() {
  // Sets the four pins as Outputs
  pinMode(switch_1, INPUT);
  pinMode(switch_2, INPUT);
  pinMode(switch_3, INPUT);
  pinMode(switch_4, INPUT);
  pinMode(stepPin_1, OUTPUT);
  pinMode(dirPin_1, OUTPUT);
  pinMode(stepPin_2, OUTPUT);
  pinMode(dirPin_2, OUTPUT);
  myservo.attach(servo);
  Serial.begin(9600);
  Serial.println("start to receive data from monitor");
  digitalWrite(stepPin_2, LOW);
  digitalWrite(stepPin_1, LOW);

  radio.begin();
  //radio.setRetries(15, 15);
  
  coord1_data.x = 0;
  coord1_data.y = 0;
  coord2_data.x = 0;
  coord2_data.y = 0;

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]); //writing to 2Node
    radio.openReadingPipe(1,addresses[0]); //reading on 1Node
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
 
  // Start the radio listening for data
  radio.startListening();
}


void loop() {
//  z_release();
//  delay(1000);
  
//  if (Serial.available() > 0)
//  {
//    count ++;
//    Sbuffer = Serial.readString();
//    input = Sbuffer.toInt();
//    if (count == 1)
//    {
//      Serial.println("Get x value! Wait for y value.\n");
//      x_cur = input;
//      Serial.println(x_cur);
//    }
//    else if (count == 2)
//    {
//      y_cur = input;
//      Serial.println(y_cur);
//      count = 0;
//      Serial.println("Current location: ");
//
//     move_x(x_cur);
//     move_y(y_cur);
//    }
//  }
//  z_push();
// delay(1000);
//  if (Serial.available() > 0){
//    Eraser(3000, 1000, 2000, 500);
//    Reset_1();
//  }
    //move_y(1000);
    //delay(100);
    //Reset_1();
    //delay(2000);
//
    if (radio.available()){
        Serial.println("receive data!\n");
        radio.read( &coord1_data, sizeof(point_data));
        delay(100);
        radio.read( &coord2_data, sizeof(point_data));
        delay(100);
        Serial.println(coord1_data.x);
        Serial.println(coord1_data.y);
        Serial.println(coord2_data.x);
        Serial.println(coord2_data.y);

//        radio.stopListening(); 
//        radio.write(&coord1_data, sizeof(point_data));
//        radio.write(&coord2_data, sizeof(point_data));
//        radio.startListening(); 

        //Eraser(coord1_data.x*reso,coord1_data.y*reso,coord2_data.x*reso,coord2_data.y*reso);
        //Reset_1();
    }
//    delay(100);
}


void move_x(int x_cur)
{
  Serial.print("move x_pre= ");
  Serial.println(x_pre);
  
  int b_1 = buttonState_1;
  int b_2 = buttonState_2;  
  
  Serial.print("move x_cur = ");
  Serial.println(x_cur);
  Serial.println(x_cur-x_pre);

  if (x_cur > x_pre)
    digitalWrite(dirPin_1, HIGH);
  else
    digitalWrite(dirPin_1, LOW);
 
  for (int i = 0; i < abs(x_cur-x_pre); i++) {
   if(x_cur>x_pre)
    {
      if(i < x_l-x_pre){
        digitalWrite(stepPin_1, HIGH);
        delayMicroseconds(500);
        digitalWrite(stepPin_1, LOW);
        delayMicroseconds(500);
       }
       else
       {
        if(b_2  == LOW)
          {
            digitalWrite(stepPin_1,HIGH);
            delayMicroseconds(500);
            digitalWrite(stepPin_1,LOW);
            delayMicroseconds(500);
            Serial.println(i);
           
            b_2=sw(switch_2);
          }
         else{
            x_cur = 0;
            x_pre = x_cur;
            return;
          }
        }
     }
   else if(x_cur<x_pre)
    {
      if(i<x_pre-100){
        digitalWrite(stepPin_1, HIGH);
        delayMicroseconds(500);
        digitalWrite(stepPin_1, LOW);
        delayMicroseconds(500);
     }
     else
     {
      if(b_1  == LOW)
        {
          digitalWrite(stepPin_1,HIGH);
          delayMicroseconds(500);
          digitalWrite(stepPin_1,LOW);
          delayMicroseconds(500);
          Serial.println(i);
     
          b_1=sw(switch_1);
          }
       else{
        x_cur = x_l;
        x_pre = x_cur;
       return;  
       }
     }
   }
   delay(1);
 }
 x_pre = x_cur;
}

void move_y(int y_cur)
{
  Serial.print("move y_pre= ");
  Serial.println(y_pre);
  
  int b_3 = buttonState_3;
  int b_4 = buttonState_4;  
  
  Serial.print("move y_cur = ");
  Serial.println(y_cur);
  Serial.println(y_cur-y_pre);

  if (y_cur > y_pre)
    digitalWrite(dirPin_2, LOW);
  else
    digitalWrite(dirPin_2, HIGH);
 
  for (int i = 0; i < abs(y_cur-y_pre); i++) {
   if(y_cur>y_pre)
    {
      if(i < y_l-y_pre){
        digitalWrite(stepPin_2, HIGH);
        delayMicroseconds(500);
        digitalWrite(stepPin_2, LOW);
        delayMicroseconds(500);
       }
       else
       {
        if(b_3  == LOW)
          {
            digitalWrite(stepPin_2,HIGH);
            delayMicroseconds(500);
            digitalWrite(stepPin_2,LOW);
            delayMicroseconds(500);
            Serial.println(i);
           
            b_3 = sw(switch_3);
          }
          else{
            y_cur = y_l;
            y_pre = y_cur;
            return;
          }
        }
     }
    else if(y_cur<y_pre)
    {
      if(i<y_pre-100){
        digitalWrite(stepPin_2, HIGH);
        delayMicroseconds(500);
        digitalWrite(stepPin_2, LOW);
        delayMicroseconds(500);
     }
     else
     {
      if(b_4  == LOW)
        {
          digitalWrite(stepPin_2,HIGH);
          delayMicroseconds(500);
          digitalWrite(stepPin_2,LOW);
          delayMicroseconds(500);
          Serial.println(i);
     
          b_4=sw(switch_4);
          }
       else{
        y_cur = 0;
        y_pre = y_cur;
       return;  
       }
     }
   }
   delay(1);
 }
 y_pre = y_cur;
}

//void move_y(int y_cur)
//{
//  Serial.print("move y_pre= ");
//  Serial.println(y_pre);
//  int b_3=buttonState_3;
//  int b_4=buttonState_4;  
//  Serial.print("move y_cur= ");
//  Serial.println(y_cur);
//  Serial.println(y_cur-y_pre);
//
//      if (y_cur > y_pre)
//        digitalWrite(dirPin_2, HIGH);
//      else
//        digitalWrite(dirPin_2, LOW);
//     
//      //move_x(abs(coord_x_trans(x_cur - x_pre)), dirx);
//
//for (int i = 0; i < abs(y_cur-y_pre); i++) {
//   
//    digitalWrite(stepPin_2, HIGH);
//    delayMicroseconds(500);
//    digitalWrite(stepPin_2, LOW);
//    delayMicroseconds(500);
//   }
// y_pre = y_cur;
//
//}

void Eraser(int x1, int y1, int x2,int y2)
{  
  bool bool_1 = comp(y1, y2);
  move_x(x1);
  move_y(y1);
  switch(bool_1){
        case true:
              z_push();
              for (int i = y1; i > y2; i -= reso*20){
                move_x(x2);
                delay(100);
                move_x(x1);
                delay(100);
                if (i < y2)
                  move_y(y2);
                else
                  move_y(i);
              }
        break;
        case false:
              z_push();
              for (int i = y1; i < y2; i += reso*20){
                move_x(x2);
                delay(100);
                move_x(x1);
                delay(100);
                if (i > y2)
                  move_y(y2);
                else 
                move_y(i);
              }
        break;   
  }
  z_release();
}

void Reset( int x, int y)
{ myservo.write(30);
  move_x(0);
  move_y(0);
}

int coord_x_trans(int x) {
  return x * 10;
}

int coord_y_trans(int y) {
  return y * 10;
}

bool sw(int s) {
  return digitalRead(s);
}

void z_release()
{
  myservo.write(180);
}

void z_push()
{
  myservo.write(0);
}

void Reset_1()
{
  move_x(0);
  move_y(0);
}

bool comp(int x, int y)
  {
    if (x > y) 
    return true;
    else 
    return false;
  }

bool sw_toggle(int SW, bool BS)
{
  //if button state is false and now Switch triggered 
    if (!BS && sw(SW))
      return true;
    else 
      return false;
}


