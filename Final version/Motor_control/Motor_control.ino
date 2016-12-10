#include<Servo.h>
#include <SPI.h>
#include <string.h>
#include <RF24.h>
#include <nRF24L01.h>

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

const int switch_1 = 13; //x = 0
const int switch_2 = 11; //x_max
const int switch_3 = 12; //y = 0
const int switch_4 = 9;  //y_max

const int servo = 3;

bool buttonState_1 = false;
bool buttonState_2 = false;
bool buttonState_3 = false;
bool buttonState_4 = false;

Servo myservo;

String Sbuffer;
int input;
int count = 0;
int x_cur = 0;
int y_cur = 0;
int x_des = 0;
int y_des= 0;

void Reset();
void Eraser(int x1, int x2, int y1, int y2);
void move_x(int x);
void move_y(int y);
bool sw(int SW);
void z_release();
void z_push();
bool sw_toggle(int SW, bool BS);
void data_trans(struct point_data point);

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
//  myservo.attach(servo);
  Serial.begin(9600);
  Serial.println("start to receive data from monitor");
  digitalWrite(stepPin_2, LOW);
  digitalWrite(stepPin_1, LOW);

  radio.begin();
  radio.setRetries(15, 15);
  
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

  radio.startListening();
}

void loop() {
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
        
        radio.stopListening();
        radio.write(&coord1_data, sizeof(point_data));
        radio.write(&coord2_data, sizeof(point_data));
        radio.startListening(); 
         
        data_trans(coord1_data);
        data_trans(coord2_data);

        myservo.attach(servo); 
        Eraser(coord1_data.x*reso,coord1_data.y*reso,coord2_data.x*reso,coord2_data.y*reso);
        myservo.detach();
        Reset();
        
//        Serial.println(coord1_data.x);
//        Serial.println(coord1_data.y);
//        Serial.println(coord2_data.x);
//        Serial.println(coord2_data.y);
    }
}

void move_x(int x_des)
{
   //set up moving direction
  if (x_des > x_cur){
    digitalWrite(dirPin_1, LOW);
     Serial.println("x_dir1");
  }
  else{
    digitalWrite(dirPin_1, HIGH);
     Serial.println("x_dir2");
  }
  
  Serial.print("Current x position: ");
  Serial.println(x_cur);
  
  Serial.print("x movment destination: ");
  Serial.println(x_des);
  
  Serial.print("x movment distance: ");
  Serial.println(abs(x_des-x_cur));
 
   if(x_des > x_cur)
    {
        for (int i = x_cur; i < x_des; i++){
        if (sw_toggle(switch_2, buttonState_2)){
            x_cur = x_l;
            Serial.println("x right switch triggered");
            break;
        }
        buttonState_2 = sw(switch_2);
    
        digitalWrite(stepPin_1, HIGH);
        delayMicroseconds(800);
        digitalWrite(stepPin_1, LOW);
        delayMicroseconds(800);
        x_cur ++;
       }
       Serial.println("Process Finished");
    }
    else
       {
        for (int i = x_cur; i > x_des; i--){
          if (sw_toggle(switch_1, buttonState_1)){
            x_cur = 0;
            Serial.println("x left switch triggered");
            break;
          }
          buttonState_1 = sw(switch_1);
            
          digitalWrite(stepPin_1, HIGH);
          delayMicroseconds(800);
          digitalWrite(stepPin_1, LOW);
          delayMicroseconds(800);  
          x_cur --;
         }
         Serial.println("Process Finished");
       }
   }

void move_y(int y_des)
{
   //set up moving direction
  if (y_des > y_cur){
    digitalWrite(dirPin_2, LOW);
     Serial.println("y_dir1");
  }
  else{
    digitalWrite(dirPin_2, HIGH);
     Serial.println("y_dir2");
  }
  
  Serial.print("Current y position: ");
  Serial.println(y_cur);
  
  Serial.print("y movment destination: ");
  Serial.println(y_des);
  
  Serial.print("y movment distance: ");
  Serial.println(abs(y_des-y_cur));
 
   if(y_des > y_cur)
    {
       for (int i = y_cur; i < y_des; i++){
		    if (sw_toggle(switch_4, buttonState_4)){
		    	  y_cur = y_l;
            Serial.println("y bottom switch triggered");
		      	break;
		    }
		    buttonState_4 = sw(switch_4);
		
		    digitalWrite(stepPin_2, HIGH);
        delayMicroseconds(800);
        digitalWrite(stepPin_2, LOW);
        delayMicroseconds(800);  
        y_cur ++;
       }
       Serial.println("Process Finished");
    }
    else
       {
        for (int i = y_cur; i > y_des; i--){
      		if (sw_toggle(switch_3, buttonState_3)){
      			y_cur = 0;
            Serial.println("y top switch triggered");
      			break;
      		}
      		buttonState_3 = sw(switch_3);
      			
      		digitalWrite(stepPin_2, HIGH);
     			delayMicroseconds(800);
     			digitalWrite(stepPin_2, LOW);
     			delayMicroseconds(800); 
          y_cur --;   
   		   }
          Serial.println("Process Finished");
       }
   }

void Eraser(int x1, int y1, int x2,int y2)
{  
  bool bool_1 = comp(x1, x2);
  move_x(x1);
  move_y(y1);
  switch(bool_1){
        case true:
              z_push();
              for (int i = x1; i > x2; i -= reso*20){
                move_y(y2);
                move_y(y1);
               
                if (i < x2)
                  move_x(x2);
                else
                  move_x(i);
              
              }
        break;
        case false:
              z_push();
              for (int i = x1; i < x2; i += reso*20){
                move_y(y2);
                move_y(y1);
                
                if (i > x2)
                  move_x(x2);
                else 
                move_x(i);
              
              }
        break;   
  }
  z_release();
  delay(100);
}

bool sw(int s) {
  if(digitalRead(s) == HIGH)
	  return true;
  else 
	  return false;
}

void z_release()
{
  myservo.write(0);
}

void z_push()
{
  myservo.write(180);
}

void Reset()
{
  move_x(-5000);
  move_y(-5000);
}

bool sw_toggle(int SW, bool BS)
{
	//if button state is false and now Switch triggered 
  bool temp = sw(SW);
	if (temp != BS){
    Serial.println("Switch Triggered");
    return true;
	}
	else 
		return false;
}

bool comp(int x, int y)
  {
    if (x > y) 
    return true;
    else 
    return false;
  }

void data_trans(struct point_data point)
{
  point.x = point.x * 1.2;
  point.y = point.y * 0.6;
}
