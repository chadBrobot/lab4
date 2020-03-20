// include the library code:
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <math.h> 
// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27, 16,2);

 //Arduino PWM Speed Control：
#define lm_pin 5   //PWM on M2
#define lm_dir 4   //direction control on M2
#define rm_pin 6   //PWM for M1
#define rm_dir 7   //direction control for M1 

#define interval 1500

/*left turn sppeds*/
//#define l_speed 128
//#defineint r_speed 100
/*right turn sppeds*/
int l_speed = 110;
int r_speed = 99;

int line_count = 0;

//encoders for wheel turn counting
#define l_enc 44
#define r_enc 45

#define button 25 
#define pot A7
#define line 14

unsigned long previousMillis = 0;    

long intervallcd = 120;
long intervalrun = 4000; 

int oldRenc = digitalRead(r_enc); 
int encR = digitalRead(r_enc);
int oldLenc = digitalRead(l_enc); 
int encL = digitalRead(l_enc);

int count = 0;
int tickL = 0;
int tickR = 0;
bool menu = true;
bool runfor = false;

// dist = r x a x pi/180
float rad = 9;
float pi = M_PI;

void setup() {
    // set up the LCD's number of columns and rows:
    pinMode(lm_dir, OUTPUT);
    pinMode(rm_dir, OUTPUT);
    pinMode(l_enc, INPUT);
    pinMode(r_enc, INPUT);
    pinMode(line, INPUT);
    
    Serial.begin(115200); 
    
    pinMode(button, INPUT);
    pinMode(pot, INPUT);
    
    lcd.init();
    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.backlight();
    Serial.println(pi);
}

void release(int motor){
  analogWrite(motor, 0);
  }

void forward(int motor_dir, int motor_pin, int speedy){
  digitalWrite(motor_dir,HIGH);
  analogWrite(motor_pin, speedy);
  }

void backward(int motor_dir, int motor_pin, int speedy){
  digitalWrite(motor_dir,LOW);
  analogWrite(motor_pin, speedy);
  }

//any angle above 180 turns left
void turnRight( int dist){
   int count = (dist/0.0609);
   boolean go = true;
   while(go){
   if ((tickR <= count) && (tickL <= count)) {
      encR = digitalRead(r_enc);
      encL = digitalRead(l_enc);
      
      //this means left is going faster
     if(encR != oldRenc){
          if(encR == 1){
            tickR++; 
            }
            oldRenc = encR;    
          }
  
      if(encL != oldLenc){
          if (encL == 1){
            tickL++;
            }
          oldLenc = encL;
        }
      //check if the left wheel is going to fast
      if(tickL > tickR){
        release(lm_pin); 
        backward(rm_dir, rm_pin, r_speed);       
      } 
       
       //check if the right wheel is going to fast
       if(tickR > tickL){
          release(rm_pin); 
          forward(lm_dir, lm_pin, l_speed); 
        }
       
      if(tickL == tickR){
         backward(rm_dir, rm_pin, r_speed);
         forward(lm_dir, lm_pin, l_speed);   
      }
    }
    else {
        forward(rm_dir, rm_pin, 200);
        backward(lm_dir, lm_pin, 200);
        delay(50);  
        release(lm_pin);
        release(rm_pin);
        go = false;
    }
  
   }
  
 }




//any angle below 180 turns left
void turnLeft(int dist){
        
   int count = (dist/0.0609);
   boolean go = true;
   while(go){
   if ((tickR <= count) && (tickL <= count)) {
      encR = digitalRead(r_enc);
      encL = digitalRead(l_enc);
      
      //this means left is going faster
     if(encR != oldRenc){
          if(encR == 1){
            tickR++; 
            }
            oldRenc = encR;    
          }
  
      if(encL != oldLenc){
          if (encL == 1){
            tickL++;
            }
          oldLenc = encL;
        }
      //check if the left wheel is going to fast
      if(tickL > tickR){
        release(lm_pin); 
        forward(rm_dir, rm_pin, r_speed);       
      } 
       
       //check if the right wheel is going to fast
       if(tickR > tickL){
          release(rm_pin); 
          backward(lm_dir, lm_pin, l_speed); 
        }
       
      if(tickL == tickR){
         forward(rm_dir, rm_pin, r_speed);
         backward(lm_dir, lm_pin, l_speed);   
      }
    }
    else {
        backward(rm_dir, rm_pin, 200);
        forward(lm_dir, lm_pin, 200);
        delay(50);  
        release(lm_pin);
        release(rm_pin);
        go = false;
    }
  }  
}

void loop() {
 if (runfor == false){
  
 if(menu==true){
    int x = analogRead(pot); //potentiometer reading
    int but = digitalRead(button); //0 = on 1 = off
    lcd.setCursor(0,0);
    lcd.print("Angle: ");
    lcd.setCursor(9,0);
 
    lcd.print("     ");
    lcd.setCursor(10,0);

    x = map(x, 0, 1023, 0, 72);
    x *= 5;
    lcd.setCursor(0,1);
    lcd.print(x);
    lcd.print("        ");
    if(but == 0){  
      
      if(x < 180)  {
       float dist = rad * x * (pi/180);  
       turnLeft(dist);
        }
      else {
        float dist = rad * (360-x) * (pi/180);  
        turnRight(dist);
        }


    }
  }
}
}





  
