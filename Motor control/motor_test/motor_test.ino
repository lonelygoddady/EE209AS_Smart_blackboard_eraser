const int stepPin_1 = 3; 
const int dirPin_1 = 4; 
const int stepPin_2 = 5; 
const int dirPin_2 = 6;  
const int switch_1=2;
 

void setup() {
  // Sets the four pins as Outputs
  pinMode(switch_1,INPUT);
  pinMode(stepPin_1,OUTPUT); 
  pinMode(dirPin_1,OUTPUT);
   pinMode(stepPin_2,OUTPUT); 
  pinMode(dirPin_2,OUTPUT);
   Serial.begin(9600);
 Serial.println("start to receive data from monitor"); 
 digitalWrite(stepPin_2, LOW);
 digitalWrite(stepPin_1, LOW);
  
}
void loop() {


int x[3];
int y[3];
move_x(4850,0);
move_y(4850,0);
delay(1000);
move_x(4850,1);
move_y(4850,1);
delay(1000);
//move_y(y[0],1);
//Eraser(x[1]-x[0],y[2]-y[0]);
//Reset(x[2],y[2]);



}
void move_x(int x, int dir)
{
  Serial.print("move x!!! x = ");
  Serial.println(x);
  if (dir==1)
  digitalWrite(dirPin_1,HIGH); 
  else
   digitalWrite(dirPin_1,LOW); 
  
  for(int i = 0; i < x; i++) {
    digitalWrite(stepPin_1,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin_1,LOW); 
    delayMicroseconds(500); 
}
}
void move_y(int y, int dir)
{
  if (dir==1)
  digitalWrite(dirPin_2,HIGH); 
  else
   digitalWrite(dirPin_2,LOW); 
  
  for(int i = 0; i < y; i++) {
    digitalWrite(stepPin_2,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin_2,LOW); 
    delayMicroseconds(500); 
}
}
void Eraser(int l,int w)
{int a=1,n=0;
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
  move_x(x,0);
  move_y(y,0);
}


