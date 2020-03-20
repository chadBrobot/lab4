// include the library code:
//Authors: Jack Casey Campbell and Breann Thiessen
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27, 16,2);


#include <SPI.h>
#include <SD.h>




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

const int chipSelect = 53;

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

 File dataFile;
String dataString = "";

void setup() {
    // set up the LCD's number of columns and rows:
    pinMode(lm_dir, OUTPUT);
    pinMode(rm_dir, OUTPUT);
    pinMode(l_enc, INPUT);
    pinMode(r_enc, INPUT);
    pinMode(line, INPUT);
    
    Serial.begin(115200); 

    if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
     }
    dataFile = SD.open("datalog2.txt", FILE_WRITE); //ope file and write to it
    
    pinMode(button, INPUT);
    pinMode(pot, INPUT);
    
    lcd.init();
    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.backlight();
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


void iCanGoTheDistance(int distance){
   count = distance/0.0609;
   boolean go = true;
   while(go) {
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
        forward(lm_dir, lm_pin, l_speed); 
      }
     
    if(tickL == tickR){
       forward(rm_dir, rm_pin, r_speed);
       forward(lm_dir, lm_pin, l_speed);   
    }
       
}
  else {
    backward(rm_dir, rm_pin, 255);
    backward(lm_dir, lm_pin, 255);
    go = false;
    delay(75);
    release(lm_pin);
    release(rm_pin);    
  }
 }
}


void goForwardOption(int driveTime, int choice){
    
    
    driveTime *= 1000; //milliseconds
    unsigned long currentMillis = millis();
    unsigned long startMillis = millis();
    boolean go = true;
    int readLine = digitalRead(line);
    int oldline = digitalRead(line);
    int previousMillis = millis();
    int countLines;
    while(go){
    if (currentMillis - startMillis <= driveTime) {
      currentMillis = millis();
      encR = digitalRead(r_enc);
      encL = digitalRead(l_enc);
      readLine = digitalRead(line); 
      
      dataString=String(readLine); //read data to string for sd
      dataFile.println(dataString); //write to file
     
      if(readLine != oldline){
        countLines++;
        
        oldline = readLine;
      }

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
          forward(lm_dir, lm_pin, l_speed); 
        }
       
      if(tickL == tickR){
         forward(rm_dir, rm_pin, r_speed);
         forward(lm_dir, lm_pin, l_speed);   
      }
    }
    else {
        backward(rm_dir, rm_pin, 255);
        backward(lm_dir, lm_pin, 255);
        delay(75);
        release(lm_pin);
        release(rm_pin);
        go = false;
        if(choice == 1){
          
         
         
         dataFile.close();
         
         lcd.setCursor(0,0);
         lcd.print("                       ");
         lcd.setCursor(0,0);
         lcd.print("lines: ");
         lcd.setCursor(0,1);
         lcd.print("                        ");
         lcd.setCursor(11,0);
         lcd.print(countLines/2);
         
        }

        }
    } //time check loopy
} //end of function

void loop() {
 if (runfor == false){
  
 if(menu==true){
    int x = analogRead(pot); //potentiometer reading
    int but = digitalRead(button); //0 = on 1 = off
    lcd.setCursor(0,0);
    lcd.print("Option: ");
    lcd.setCursor(9,0);
 
    lcd.print("     ");
    lcd.setCursor(10,0);

    x = map(x, 0, 1023, 1, 4);
     if(x==1){
        lcd.setCursor(0,1);
        lcd.print("Go Forward");
        lcd.print("            ");
        }
     else if(x==2){
        lcd.setCursor(0,1);
        lcd.print("Count Lines");
        lcd.print("            ");
        }
    else if(x==3){
        lcd.setCursor(0,1);
        lcd.print("Distance");
        lcd.print("            ");
        }
    else{
       lcd.setCursor(0,1);
       lcd.print("Exit");
       lcd.print("            ");
        }

  //if button pressed take the speed and divide by how many encoder ticks we need
  if(but == 0){
    runfor= true;
    if(x==1){
      boolean pressed = false;
      int y;
      while(!pressed){
        //counts have to be divided by the ticks/cm for the start
        y  = analogRead(pot); //potentiometer reading
        int but = digitalRead(button); //0 = on 1 = off
        
        lcd.setCursor(0,0);
        lcd.print("Seconds: ");
        lcd.setCursor(0,1);  
        lcd.print("           ");
        //distance time is y
        y = map(y, 0, 1023, 3, 7);
        lcd.setCursor(11,0);
        lcd.print(y);
        lcd.print("        ");
        if(but == 0){
          //send the choice from first menu and the distance time
          goForwardOption(y, 0);       
          pressed = true;
         }
      }
    }
    if(x == 2){
      boolean pressed = false;
      int y;
      while(!pressed){
         //counts have to be divided by the ticks/cm for the start
        y  = analogRead(pot); //potentiometer reading
        int but = digitalRead(button); //0 = on 1 = off
              lcd.setCursor(0,0);
        lcd.print("Seconds: ");
        lcd.setCursor(0,1);  
        lcd.print("           ");
        //distance time is y
        y = map(y, 0, 1023, 3, 7);
        lcd.setCursor(11,0);
        lcd.print(y);
        lcd.print("        ");
        if(but == 0){
          //send the choice from first menu and the distance time
          goForwardOption(y, 1);       
          pressed = true;
         }
        }  
     }
     if(x == 3){
      boolean pressed = false;
      int y;
      while(!pressed){
         //counts have to be divided by the ticks/cm for the start
        y  = analogRead(pot); //potentiometer reading
        int but = digitalRead(button); //0 = on 1 = off
        lcd.setCursor(0,0);
        lcd.print("Distance: ");
        lcd.setCursor(0,1);  
        lcd.print("           ");
        //distance time is y
        y = map(y, 0, 1023, 20, 120);
        lcd.setCursor(11,0);
        lcd.print(y);
        lcd.print("        ");
        if(but == 0){
          //send the choice from first menu and the distance time
          iCanGoTheDistance(y);       
          pressed = true;
         }
        } 
      }

      if(x == 4){
        lcd.setCursor(0,0);
        lcd.print("        ");
        lcd.setCursor(0,1);
        lcd.print("        ");
        lcd.setCursor(0,0);
        lcd.print(" GOODBYE FRIEND");
        while(1){}
      }
  
    }
  }
 }
}




  
