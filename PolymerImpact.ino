#include <Wire.h>

#include <Encoder.h>

#include <Adafruit_RGBLCDShield.h>


/*
should be able to select a variable height using the buttons on the new display. that variable height will go into play with the calculation of the energy. Need to also record the max height the mass goes on the follow through. 
*/

/**
ToDo: New display output
      Button variables
      energy calculation and output
      height output
      
**/

int latchPin = 4; // output pin for arduino but an input for the latch
                  // output of latch goes to ground
                  
long oldPosition  = -999;
int maxHeight;
long sensValue = 0;
double sensor;
double theta;
const int Pi = 3.14159; 


//these are where the variables for h are set. if there are any changes to the impact device are this is used on a different device these will need to be adjusted in order for the calculations to be accurate
double Hinit = 0;  //h initial
double h30 = 0.7;  //h variable 30
double h45 = 0;  //h variable 45
double h60 = 0;  //h variable 60
double h75 = 0;  //h variable 75

//this is the radius/arm length for the mass
double r = 0.4; //r

double Hf;
double Hchange;
double E;

//if the mass changes this will need to be adjusted
double mass = 2.16;

double gravity = 9.81;

Encoder myEnc(2, 3); //these are the pins that the encoder is using to communicate with the board
//   avoid using pins with LEDs attached

/**
It looks like this function sets the latch pin to an output and begins the serial connection at a 9600 BAUD
There is also a line for myserial that was never defined and therefore I have no clue what it's for hence why it is commented out.
**/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);           //Sets the pin as an output
  lcd.begin(16, 2);
  //Serial.println("Basic Encoder Test:");
  //mySerial.begin(9600); // set up serial port for 9600 baud
}//end setup

/**
this function sets the value of Hinit based on what was selected (p)
**/
void variableh(int p){
 if p = 30{
   Hinit = h30;
 }//endif
 else if p = 45{
   Hinit = h45;
 }//end elif
 else if p = 60{
   Hinit = h60;
 }//end elif
 else if p = 75{
   Hinit = h75;
 }//end elif
}//end variableh

/**
This function is where the magic happens. It starts by releasing the latch then taking new position from the encoder.
it checks the newposition to the oldposition and if it is a new position then it sets that as the old position to begin the loop again until the newpostion is no longer larger
the next line calculates the final height angle
the last line in the if statement calculates the energy
outside the if it displays the angle and the energy that was calculated
**/
void loop() { 

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

//shouldn't need to touch below this
//SerialLCD Functions
void selectLineOne(){  //puts the cursor at line 0 char 0.
   Serial.write(0xFE);   //command flag
   Serial.write(128);    //position
}
void selectLineTwo(){  //puts the cursor at line 2 char 0.
   Serial.write(0xFE);   //command flag
   Serial.write(192);    //position
}
void selectLineThree(){  //puts the cursor at line 3 char 0.
   Serial.write(0xFE);   //command flag
   Serial.write(148);    //position
}
void selectLineFour(){  //puts the cursor at line 4 char 0.
   Serial.write(0xFE);   //command flag
   Serial.write(212);    //position
}
void goTo(int position) { //position = line 1: 0-19, line 2: 20-39, etc, 79+ defaults back to 0
    if (position<20){ Serial.write(0xFE);   //command flag
                  Serial.write((position+128));    //position
    }
    else if (position<40){Serial.write(0xFE);   //command flag
                  Serial.write((position+128+64-20));    //position 
    }
    else if (position<60){Serial.write(0xFE);   //command flag
                  Serial.write((position+128+20-40));    //position
    }
    else if (position<80){Serial.write(0xFE);   //command flag
                  Serial.write((position+128+84-60));    //position              
    }
    else { goTo(0); }
}
void clearLCD(){
   Serial.write(0xFE);   //command flag
   Serial.write(0x01);   //clear command.
}
void backlightOn(){  //turns on the backlight
    Serial.write(0x7C);   //command flag for backlight stuff
    Serial.write(157);    //light level.
}
void backlightOff(){  //turns off the backlight
    Serial.write(0x7C);   //command flag for backlight stuff
    Serial.write(128);     //light level for off.
}
void backlight50(){  //sets the backlight at 50% brightness
    Serial.write(0x7C);   //command flag for backlight stuff
    Serial.write(143);     //light level for off.
}
void serCommand(){   //a general function to call the command flag for issuing all other commands   
  Serial.write(0xFE);
}





