#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define sensor_pin A3
LiquidCrystal_I2C lcd(0x27, 16, 2);
float calibration_value = 21.34;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
const int buttonPin = 2;
int val1 = 0;  // holds the value
int WL_pin1 = A1; // sensor pin used
int val2 = 0;  // holds the value
int WL_pin2 = A2; // sensor pin used
int In1 = 3;
int In2 = 4;
int In4 = 6; 
int In5 = 7;
int In3 = 5;
int Turbidity_Read;
int Turbidity_Value;
int trigPin = 9;    // TRIG pin
int echoPin = 8;    // ECHO pin
float duration_us, distance_cm;
void setup() { 
 
  // start the serial console
  Serial.begin(9600);
  pinMode(In1,OUTPUT);
  pinMode(In2,OUTPUT);
  pinMode(In4,OUTPUT);
  pinMode(In3,OUTPUT);
  pinMode(In5,OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WELCOME TO");
  lcd.setCursor(0,1);
  lcd.print("WATER FOUNTAIN ");
  delay(2000);
  lcd.clear();
  
  
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(In1,HIGH);
  digitalWrite(In2,HIGH);
  digitalWrite(In4,HIGH);
  digitalWrite(In3,HIGH);
  digitalWrite(In5,HIGH);
} 
  
void loop() { 
   
  val1 = analogRead(WL_pin1); //Read data from analog pin and store it to resval variable
   
  if (val1<=100)
  { Serial.println("Water Level: Empty");
  digitalWrite(In1,LOW);
   
  }
  else if (val1>100 && val1<=300)
  { Serial.println("Water Level: Low");
  digitalWrite(In1,LOW);
   
  } else if (val1>300 && val1<=330)
  { Serial.println("Water Level: Medium"); 
  digitalWrite(In1,HIGH);
  
  } else if (val1>330)
  { 
    digitalWrite(In1,HIGH);
   
    Serial.println("Water Level: High"); 
  }
 
  val2 = analogRead(WL_pin2);
  if (val2<=100)
  { Serial.println("Water Level2: Empty");
  digitalWrite(In2,LOW);
    
   
  }
  else if (val2>100 && val2<=300)
  { Serial.println("Water Level2: Low");
  digitalWrite(In2,LOW);
     
  } else if (val2>300 && val2<=330)
  { Serial.println("Water Level2: Medium"); 
  digitalWrite(In2,HIGH);
    
  } else if (val2>330)
  { 
    digitalWrite(In2,HIGH);
    
    Serial.println("Water Level2: High"); 
  } 
 digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(echoPin, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;

  // print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
  if(distance_cm >= 20){
    digitalWrite(In3,HIGH);
    digitalWrite(In4,HIGH);
    digitalWrite(In5,HIGH);
  }else{
    digitalWrite(In3,LOW);
    digitalWrite(In4,LOW);
    digitalWrite(In5,LOW);
  }
  for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(A0);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/6;
 float ph_act = -5.70 * volt + calibration_value;
 lcd.setCursor(0, 0);
 lcd.print("pH Val:");
 lcd.setCursor(8, 0);
 lcd.print(ph_act);
 delay(1000);
 Turbidity_Read = analogRead(sensor_pin);
if(Turbidity_Read>208)Turbidity_Read=208;

Turbidity_Value = map(Turbidity_Read, 0, 208, 300, 0); 
 
lcd.setCursor(0,0);
lcd.print("Turbidity: ");
lcd.print(Turbidity_Value);
lcd.print("  ");

lcd.setCursor(0,1);//set cursor (colum by row) indexing from 0
if(Turbidity_Value<10)            lcd.print("Water Very Clean");
if(Turbidity_Value>=10 && Turbidity_Value<30) lcd.print("Water Norm Clean");
if(Turbidity_Value>=30);           lcd.print("Water Very Dirty");

}
