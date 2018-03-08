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
  long oldPosition  = -999;
  int maxHeight;
  long sensValue = 0;
  double sensor;
  double theta;
  const int Pi = 3.14159; 
  bool x = true;

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
  int Fa = 0;
  int Ia = 0;

  //if the mass changes this will need to be adjusted
  double mass = 2.73;
  double gravity = 9.81;

  //these are the pins that the encoder is using to communicate with the board. avoid using pins with LEDs attached
  Encoder myEnc(2, 3); 

void setup(){    
  //set LCD columns and rows
  lcd.begin(16, 2);
  
  //print a test message to the LCD
  lcd.print("Booting up, Please Hold");
  
  delay(2);
  
  //clear the lcd
  lcd.clear();
  //set the cursor to column 0, line 0
  lcd.setCursor(0, 0); //setting screen to write at the top row 
}//endsetup
  


void loop() {
    uint8_t buttons = lcd.readButtons(); //setting up buttons
    if (buttons) { //setting the buttons and waiting for input as to wait value to set Hinit
      if (buttons & BUTTON_UP) {
        //set height to 75
        Hinit = h75;
        Ia = 75;
        lcd.print("Theta = 75");
      }
      if (buttons & BUTTON_RIGHT) {
        //set theta to 60
        Hinit = h60;
        Ia = 60;
        lcd.print("Theta = 60");
      }
      if (buttons & BUTTON_DOWN) {
        //set theta to 45
        Hinit = h45;
        Ia = 45;
        lcd.print("Theta = 45");
      }
      if (buttons & BUTTON_LEFT) {
        //set theta to 30
        Hinit = h30;
        Ia = 30;
        lcd.print("Theta = 30");
      }
      if (buttons & BUTTON_SELECT) {
        x = false;
        lcd.clear();
        //lcd.setCursor(0,0);
      }
    }
    while (x == false){
       
       long newPosition = myEnc.read();    //this is reading in the position from a sensor called myEnc on pins 2 and 3
       if (newPosition != oldPosition and newPosition > oldPosition  ) { //if newposition is actually new
           sensValue = newPosition;  
           oldPosition = newPosition;
           theta = sensValue*360/4096; //setting theta
           // calculates maximum angle (deg) pendulum reaches
           //new calculations follow
           Hf = r * sin(theta); // calc final height
           Hchange = Hinit - Hf; // calc for height change
           E = mass * gravity * Hchange; //energy calc
           Fa = theta - (Ia + 90); //calculating the final angle
           //new calcs end
       }//endifcalc
       //clearLCD();
       //lcd.print(sensValue);
       lcd.setCursor(0,0); //setting cursor to the second line of screen
       lcd.print(E); //print out the answer they are looking for
       lcd.setCursor(0,1); //setting cursor to the second line of screen
       lcd.print(Fa);
       //delay(100); //idk if this is still needed
       if(buttons){
          if (buttons & BUTTON_SELECT){  //trying to escape loop
               x = true;
               lcd.clear();
               lcd.setCursor(0,0);
          }//endifinner
       }//endif
    }//end magic loop 
} //endMain loop





