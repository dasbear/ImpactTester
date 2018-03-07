#include <Adafruit_RGBLCDShield.h>
#include <Wire.h>
#include <Encoder.h>
#include <utility/Adafruit_MCP23017.h>


/*
should be able to select a variable height using the buttons on the new display. that variable height will go into play with the calculation of the energy. Need to also record the max height the mass goes on the follow through */
/**
The entire program is going to need to go into the setup function as the loop will continuously loop until reset. This is not the behaviour that is desired so there will need to bemultiple loops created in the setup function that will handle all the reading. 
Need to find what the call for reset is.
the reset should do a full system reboot or shoot out to the inital init stuff. 

when booting up the screen will be red and change to green when ready for user input. 
the screen will be green until a selection for theta has been pressed which will turn it yellow
when select gets pressed and the test starts the screen will change from yellow to blue to teal to green when it displays the final output.
when reset get pressed then it starts over.

theta will remain on top row
output will be display on row 2
it will remain until reset is pressed. 
**/
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

//setting up colors to use for the screen
#define OFF 0x0
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7
/******************************COLORS ARE NOT WORKING CURRENTLY**************************/

void setup(){  


  long oldPosition  = -999;
  int maxHeight;
  long sensValue = 0;
  double sensor;
  double theta;
  const int Pi = 3.14159; 


  //these are where the variables for h are set. if there are any changes to the impact device are this is used on a different device these will need to be adjusted in order for the calculations to be accurate
  int Hinit = 0;  //h initial
  int h30 = 0.6;  //h variable 30
  int h45 = 0.6828;  //h variable 45
  int h60 = 0.7464;  //h variable 60
  int h75 = 0.7864;  //h variable 75

  //this is the radius/arm length for the mass
  int r = 0.4; //r

  int Hf;
  int Hchange;
  int E;

  //if the mass changes this will need to be adjusted
  double mass = 2.73;

  double gravity = 9.81;

  Encoder myEnc(2, 3); //these are the pins that the encoder is using to communicate with the board. avoid using pins with LEDs attached

  
  //set LCD columns and rows
  lcd.begin(16, 2);
  
  //print a test message to the LCD
  lcd.print("Booting up, Please Hold");
  
  delay(2);
  
  //clear the lcd
  lcd.clear();
  //set the cursor to column 0, line 0
  lcd.setCursor(0, 0); //setting screen to write at the top row
  
  uint8_t buttons = lcd.readButtons(); //setting up buttons
  while(true){
    //if (buttons) { //setting the buttons and waiting for input as to wait value to set Hinit
    lcd.print("inside loop 1");
      if (buttons & BUTTON_UP) {
        //set height to 75
        Hinit = h75;
        lcd.print("Theta = 75");
      }
      if (buttons & BUTTON_RIGHT) {
        //set theta to 60
        Hinit = h60;
        lcd.print("Theta = 60");
      }
      if (buttons & BUTTON_DOWN) {
        //set theta to 45
        Hinit = h45;
        lcd.print("Theta = 45");
      }
      if (buttons & BUTTON_LEFT) {
        //set theta to 30
        Hinit = h30;
        lcd.print("Theta = 30");
      }
      if (buttons & BUTTON_SELECT) {
        false;
      }
   // }
  }
     while (false){
       lcd.setCursor(0,1); //setting cursor to the second line of screen
       lcd.print("inside loop2");
        long newPosition = myEnc.read();    //this is reading in the position from a sensor called myEnc on pins 2 and 3
        if (newPosition != oldPosition and newPosition > oldPosition  ) { //if newposition is actually new
            sensValue = newPosition;  
            oldPosition = newPosition;
            theta = sensValue*360/4096; //setting theta
            // calculates maximum angle (deg) pendulum reaches
            //new calculations follow
            Hf = r * sin(theta);
            Hchange = Hinit - Hf;
            E = mass * gravity * Hchange;
            //new calcs end
        }
        //clearLCD();
        //lcd.print(sensValue);
        lcd.print(E); //print out the answer they are looking for
        //delay(100); //idk if this is still needed
        if (buttons & BUTTON_SELECT){  //trying to escape loop
              true;
        }//endif
      }//end loop 
  
}//endsetup
  


void loop() {} // left because idk if it needs this in order to run. safer to just leave for now





////////////////Everything below this should no longer need to be here. Leaving for now. Will clean after testing. ///////////////////

/**
this function sets the value of Hinit based on what was selected (p)
**/
//void variableh(int p){
// if p = 30{
//   Hinit = h30;
// }//endif
// else if p = 45{
//   Hinit = h45;
// }//end elif
// else if p = 60{
//   Hinit = h60;
// }//end elif
// else if p = 75{
//   Hinit = h75;
// }//end elif
//}//end variableh

/**
This function is where the magic happens. It starts by releasing the latch then taking new position from the encoder.
it checks the newposition to the oldposition and if it is a new position then it sets that as the old position to begin the loop again until the newpostion is no longer larger
the next line calculates the final height angle
the last line in the if statement calculates the energy
outside the if it displays the angle and the energy that was calculated
**/

/*
  digitalWrite(latchPin, HIGH);   //looks like this line releases the latch. This is where it will output current on the latchpin 
  
  long newPosition = myEnc.read();    //this is reading in the position from a sensor called myEnc on pins 2 and 3
  if (newPosition != oldPosition and newPosition > oldPosition  ) { //if newposition is actually new
      sensValue = newPosition;  
    oldPosition = newPosition;
    theta = sensValue*360/4096; //setting theta
    // calculates maximum angle (deg) pendulum reaches
    //new calculations follow
    Hf = r - r * cos(theta);
    Hchange = Hinit - Hf;
    E = mass * gravity * Hchange;
    //new calcs end
  }
  clearLCD();
  backlightOn();
  selectLineOne();
  delay(100);
  //Serial.print("Angle (deg):  ");
  //Serial.print(theta);
  Serial.print(sensValue);
  //Serial.print("Energy:  ");
  //Serial.print(E);
  delay(100);
}//end loop

//handles the buttons. 
uint8_t buttons = lcd.readButtons();
 
  if (buttons) {
    if (buttons & BUTTON_UP) {
      lcd.setBacklight(RED);
    }
    if (buttons & BUTTON_DOWN) {
      lcd.setBacklight(YELLOW);
    }
    if (buttons & BUTTON_LEFT) {
      lcd.setBacklight(GREEN);
    }
    if (buttons & BUTTON_RIGHT) {
      lcd.setBacklight(TEAL);
    }
    if (buttons & BUTTON_SELECT) {
      lcd.setBacklight(VIOLET);
    }*/
  //}//

//
////shouldn't need to touch below this
////SerialLCD Functions
//void selectLineOne(){  //puts the cursor at line 0 char 0.
//   Serial.write(0xFE);   //command flag
//   Serial.write(128);    //position
//}
//void selectLineTwo(){  //puts the cursor at line 2 char 0.
//   Serial.write(0xFE);   //command flag
//   Serial.write(192);    //position
//}
//void selectLineThree(){  //puts the cursor at line 3 char 0.
//   Serial.write(0xFE);   //command flag
//   Serial.write(148);    //position
//}
//void selectLineFour(){  //puts the cursor at line 4 char 0.
//   Serial.write(0xFE);   //command flag
//   Serial.write(212);    //position
//}
//void goTo(int position) { //position = line 1: 0-19, line 2: 20-39, etc, 79+ defaults back to 0
//    if (position<20){ Serial.write(0xFE);   //command flag
//                  Serial.write((position+128));    //position
//    }
//    else if (position<40){Serial.write(0xFE);   //command flag
//                  Serial.write((position+128+64-20));    //position 
//    }
//    else if (position<60){Serial.write(0xFE);   //command flag
//                  Serial.write((position+128+20-40));    //position
//    }
//    else if (position<80){Serial.write(0xFE);   //command flag
//                  Serial.write((position+128+84-60));    //position              
//    }
//    else { goTo(0); }
//}
//void clearLCD(){
//   Serial.write(0xFE);   //command flag
//   Serial.write(0x01);   //clear command.
//}
//void backlightOn(){  //turns on the backlight
//    Serial.write(0x7C);   //command flag for backlight stuff
//    Serial.write(157);    //light level.
//}
//void backlightOff(){  //turns off the backlight
//    Serial.write(0x7C);   //command flag for backlight stuff
//    Serial.write(128);     //light level for off.
//}
//void backlight50(){  //sets the backlight at 50% brightness
//    Serial.write(0x7C);   //command flag for backlight stuff
//    Serial.write(143);     //light level for off.
//}
//void serCommand(){   //a general function to call the command flag for issuing all other commands   
//  Serial.write(0xFE);
//}





