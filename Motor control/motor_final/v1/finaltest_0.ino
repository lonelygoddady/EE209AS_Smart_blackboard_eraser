#include<Servo.h>
const int stepPin_1 = 3; 
const int dirPin_1 = 4; 
const int stepPin_2 = 5; 
const int dirPin_2 = 6;  
const int switch_1 = 2;
const int switch_2 = 7;
const int switch_3 = 8;
const int switch_4 = 9;
const int servo = 10;
int buttonState_1 =0; 
int buttonState_2 =0; 
int buttonState_3 =0; 
int buttonState_4 =0; 

Servo myservo;
void setup() {
  // Sets the four pins as Outputs
  pinMode(switch_1,INPUT);
  pinMode(switch_2,INPUT);
  pinMode(switch_3,INPUT);
  pinMode(switch_4,INPUT);
  pinMode(stepPin_1,OUTPUT); 
  pinMode(dirPin_1,OUTPUT);
  pinMode(stepPin_2,OUTPUT); 
  pinMode(dirPin_2,OUTPUT);
  myservo.attach(servo);
 Serial.begin(9600);
 Serial.println("start to receive data from monitor"); 
 digitalWrite(stepPin_2, LOW);
 digitalWrite(stepPin_1, LOW);
 
}
void loop() {
int x[3];
int y[3];
/*move_x(4850,0);
move_y(4850,0);
delay(1000);
move_x(4850,1);
move_y(4850,1);
delay(1000);*/
move_x_y(x[0],1,y[0],1);

Eraser(x[1]-x[0],y[2]-y[0]);
Reset(x[2],y[2]);

move_x_y(3000,1,3000,1);

Eraser(200,201);
Reset(400,400);
}

void move_x_y(int x, int dirx, int y, int diry)
{myservo.write(30);
  int b_1=buttonState_1;
  int b_2=buttonState_2;
  int b_3=buttonState_3;
  int b_4=buttonState_4;
  if (dirx==1)
    digitalWrite(dirPin_1,HIGH); 
  else
    digitalWrite(dirPin_1,LOW); 
  if (diry==1)
    digitalWrite(dirPin_2,HIGH); 
  else
    digitalWrite(dirPin_2,LOW); 
  if(x<y)
  {
    for(int i = 0; i < x; i++) 
    {
      if(b_1==LOW&&b_2==LOW&&b_3==LOW&&b_4==LOW)
    {
      digitalWrite(stepPin_1,HIGH); 
      digitalWrite(stepPin_2,HIGH);
      delayMicroseconds(500); 
      digitalWrite(stepPin_1,LOW); 
      digitalWrite(stepPin_2,LOW);
      delayMicroseconds(500); 
      b_1 =digitalRead(switch_1);
      b_2 =digitalRead(switch_2);
      b_3 =digitalRead(switch_3);
      b_4 =digitalRead(switch_4);
    }
 else if(b_1==HIGH||b_2==HIGH&&b_3==LOW&&b_4==LOW)
    {
      digitalWrite(stepPin_2,HIGH);
      delayMicroseconds(500); 
      digitalWrite(stepPin_2,LOW);
      delayMicroseconds(500); 
      b_3 =digitalRead(switch_3);
      b_4 =digitalRead(switch_4);
    }
  else if(b_3==HIGH||b_4==HIGH&&b_1==LOW&&b_2==LOW)
    {
      digitalWrite(stepPin_1,HIGH);
      delayMicroseconds(500); 
      digitalWrite(stepPin_1,LOW);
      delayMicroseconds(500); 
      b_1 =digitalRead(switch_1);
      b_2 =digitalRead(switch_2);
    }
 }
 move_y(y-x,diry);
}
else
{
for(int i = 0; i < x; i++) 
    {
      if(b_1==LOW&&b_2==LOW&&b_3==LOW&&b_4==LOW)
      {
        digitalWrite(stepPin_1,HIGH); 
        digitalWrite(stepPin_2,HIGH);
        delayMicroseconds(500); 
        digitalWrite(stepPin_1,LOW); 
        digitalWrite(stepPin_2,LOW);
        delayMicroseconds(500); 
        b_1 =digitalRead(switch_1);
        b_2 =digitalRead(switch_2);
        b_3 =digitalRead(switch_3);
        b_4 =digitalRead(switch_4);
      }
      else if(b_1==HIGH||b_2==HIGH&&b_3==LOW&&b_4==LOW)
      {
        digitalWrite(stepPin_2,HIGH);
        delayMicroseconds(500); 
        digitalWrite(stepPin_2,LOW);
        delayMicroseconds(500); 
        b_3 =digitalRead(switch_3);
        b_4 =digitalRead(switch_4);
      }
     else if(b_3==HIGH||b_4==HIGH&&b_1==LOW&&b_2==LOW)
      {
        digitalWrite(stepPin_1,HIGH);
        delayMicroseconds(500); 
        digitalWrite(stepPin_1,LOW);
        delayMicroseconds(500); 
        b_1 =digitalRead(switch_1);
        b_2 =digitalRead(switch_2);
      }
    }
  move_x(x-y,dirx);
  }
}

void move_x(int x, int dir)
{
  myservo.write(30);
  int b_1=buttonState_1;
  int b_2=buttonState_2;
  
  Serial.print("move x!!! x = ");
  Serial.println(x);
  
  if (dir==1)
    digitalWrite(dirPin_1,HIGH); 
  else
    digitalWrite(dirPin_1,LOW); 
  
  for(int i = 0; i < x; i++) {
    if(b_1==LOW&&b_2==LOW)
    {
      digitalWrite(stepPin_1,HIGH); 
      delayMicroseconds(500); 
      digitalWrite(stepPin_1,LOW); 
      delayMicroseconds(500); 
      x=x-1;
      Serial.println(x);

      b_1 =digitalRead(switch_1);
      b_2 =digitalRead(switch_2);
    }
  }
}

void move_y(int y, int dir)
{
  myservo.write(30);
  int b_3=buttonState_3;
  int b_4=buttonState_4;
  if (dir==1)
    digitalWrite(dirPin_2,HIGH); 
  else
    digitalWrite(dirPin_2,LOW); 
  
  for(int i = 0; i < y; i++) {
    if(b_3==LOW&&b_4==LOW)
    {
      digitalWrite(stepPin_2,HIGH); 
      delayMicroseconds(500); 
      digitalWrite(stepPin_2,LOW); 
      delayMicroseconds(500); 
      b_3 =digitalRead(switch_3);
      b_4 =digitalRead(switch_4);
    }
  }
}

void Eraser(int l,int w)
{
  myservo.write(80);
  int a=1,n=0;
  for(; n<w-100;n=n+100)
  {
    move_x(l,a);
    move_y(100,1);
    if (a==1)
    a=0;
    else
    a=1;
  }
  move_y(w-n,1);
  if(n/100%2==1)
  move_x(l,0);
  else
  {
    move_x(l,1);
    move_x(l,0);
  }
  
}

void Reset( int x, int y)
{
  myservo.write(30);
  move_x(x,0);
  move_y(y,0);
}


