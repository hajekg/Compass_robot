
#include <Wire.h>


//define your register addresses/modes variables

# define device_address 0x1E
# define mode_register 0x02
# define continuous_measurement_mode 0x00  
# define Data_Output_X_MSB 0x03
# define Data_Output_X_LSB 0x04
# define Data_Output_Z_MSB 0x05
# define Data_Output_Z_LSB 0x06
# define Data_Output_Y_MSB 0x07
# define Data_Output_Y_LSB 0x08

//Define your global variables for the motor shield 

// direction pins on the motor shield 

#define direction_A 12 
#define direction_B 13

//pins that control each motor on the motor shield 

#define motor_A 3
#define motor_B 11

//motor parameters

#define speed_motorA 170
#define speed_motorB 170

int heading = 190;

void Stop();
void left();
void right();


void setup(){
  
  
 Wire.begin();

 Wire.beginTransmission(device_address);

 Wire.write(mode_register);
 
 Wire.write(continuous_measurement_mode); 
 
 Wire.endTransmission();
 
 Serial.begin(9600);
 
}


void loop(){
  
// read the raw data from the compass
  
 Wire.beginTransmission(device_address);

 Wire.write(Data_Output_X_MSB); 
 
 Wire.endTransmission();
 
 Wire.requestFrom(device_address,6);

 int X = Wire.read()<< 8 | Wire.read();
 int Z = Wire.read()<< 8 | Wire.read();
 int Y = Wire.read()<< 8 | Wire.read();
 
 
 //Get the heading in radians
 
 float pi = 3.14;
 
 float angle = atan2(-Y, X);
 
 if(angle < 0){
 
 angle += 2 * pi;
 
 }
 
 
 // get heading by converting radians to degrees 
 
 float heading_compass = angle * (360/(2*pi));
 
 Serial.print("robot current heading: ");
 Serial.println(heading_compass);
 Serial.print("defined heading: ");
 Serial.println(heading);

   
  int difference = heading - heading_compass;
  
  Serial.print("discrepancy: ");
  Serial.println(difference);
  
  //Set up which way to turn depending on the difference between 
  //the compass heading and the predefined heading 
  

  
   
  if(difference >= -180 && difference <= -3){
    
    left();
  }
  
  
 
 if(difference >= 3 && difference < 180){
   
  right();
    
 }
  
  
  
  if(difference < 3 && difference >-3){
    
   Stop();
   
   forward();  
   
   delay(5000);
   
   
   for(int t = 0 ; t<1000; t++){
   
  if(difference >= -180 && difference <= -3){
    
  analogWrite(motor_A, 180); 
  digitalWrite(direction_A, LOW);

  analogWrite(motor_B, 150);
  digitalWrite(direction_B, LOW);
  
  }
  
  if(difference >= 3 && difference < 180){
    
  analogWrite(motor_A, 150); 
  digitalWrite(direction_A, LOW);

  analogWrite(motor_B, 180);
  digitalWrite(direction_B, LOW);
    
      }
      
      Stop();
      
      delay(10000);
      
   }
  
    
  }
  

 
}


//functions to control the motors 


void Stop(){
 
analogWrite(motor_A, 0);
analogWrite(motor_B, 0); 

}



void forward(){

analogWrite(motor_A, speed_motorA);
digitalWrite(direction_A, LOW);

analogWrite(motor_B, speed_motorB);
digitalWrite(direction_B, LOW);
  
}




void left(){
  
  analogWrite(motor_A, speed_motorA); 
  digitalWrite(direction_A, LOW);

  analogWrite(motor_B, speed_motorB);
   digitalWrite(direction_B, HIGH);
}



void right(){
    
  analogWrite(motor_A,speed_motorA );  
  digitalWrite(direction_A, HIGH);
  
  analogWrite(motor_B, speed_motorB);
  digitalWrite(direction_B, LOW);
  
}
