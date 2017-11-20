#include <Encoder.h>

int latchPin = 4; // output pin for arduino but an input for the latch
                  // output of latch goes to ground
                  
long oldPosition  = -999;
int maxHeight;
long sensValue = 0;
double sensor;
double theta;
const int Pi = 3.14159;
double Hinit = 0.3683; //h initial
double r = 0.3683; //r
double Hf;
double Hchange;
double E;
double mass = 7.3;
double gravity = 9.81;

Encoder myEnc(2, 3);
//   avoid using pins with LEDs attached

void setup() {
  // put your setup code here, to run once:
  pinMode(latchPin, OUTPUT);           //Sets the pin as an output
  Serial.begin(9600);
  //Serial.println("Basic Encoder Test:");
  mySerial.begin(9600); // set up serial port for 9600 baud
}
  
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
  Serial.print("Angle (deg):  ");
  Serial.print(theta);
  Serial.print("Energy:  ");
  Serial.print(E);
  delay(100);
}

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
}else if (position<40){Serial.write(0xFE);   //command flag
              Serial.write((position+128+64-20));    //position 
}else if (position<60){Serial.write(0xFE);   //command flag
              Serial.write((position+128+20-40));    //position
}else if (position<80){Serial.write(0xFE);   //command flag
              Serial.write((position+128+84-60));    //position              
} else { goTo(0); }
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





