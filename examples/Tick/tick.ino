/**
* @author Mike Almond - https://github.com/mikedotalmond
* */

/*
* Pulse generator for the lavet-type stepper motor found in clock movements
* like this: http://www.amazon.co.uk/gp/product/B0093YX7QI/
*
* http://en.wikipedia.org/wiki/Lavet_type_stepping_motor
* 
* This library is based on the circuit and code presented here:
* http://www.cibomahto.com/2008/03/controlling-a-clock-with-an-arduino/
* It's a good reference for the kind of circuit you'll need to use this, 
* and the code there does basically the same thing; but in the Arduino IDE, using delays.
**/


#include <Tick.h>

// Digital pins used to send pulses to the solenoid
// refer to the article referenced above for an example circuit
const int CLOCK_A = 2; // signal pin a
const int CLOCK_B = 3; // signal pin b

Tick tick;
//Tick tick(CLOCK_A, CLOCK_B, 1000); // initialise in the constructor, or call tick.setup(...) later
//Tick tick(CLOCK_A, CLOCK_B, 1000, onTickStart, onTickComplete);

// somewhere to store the last millis() count for calculating execution delta times in the main loop(){}
unsigned long lastMillis = 0;

void setup() {
  
  Serial.begin(9600); 
  
  // set up the pulse generator using digital pins CLOCK_A and CLOCK_B, with a 1000ms tick duration.
  // You can optionally supply callback functions to handle when a tick pulse starts - onTickStart, 
  // and when it completes - onTickComplete
  // These setup calls are all valid:
  //tick.setup(CLOCK_A, CLOCK_B, 1000);
  //tick.setup(CLOCK_A, CLOCK_B, 1000, onTickStart, NULL);
  //tick.setup(CLOCK_A, CLOCK_B, 1000, NULL, onTickComplete);
  tick.setup(CLOCK_A, CLOCK_B, 1000, onTickStart, onTickComplete);
}
 
void loop() {
  
  // calculate the ms differnce since the last time round   
  unsigned long time = millis();
  unsigned long dt   = time - lastMillis;
  lastMillis 		 = time;
  
  // if time has advanced at least a millisecond: update.
  if(dt > 0) tick.update(dt);
}

/**
*
*/
void onTickStart(){
	// Serial.println("onTickStart");
	// If powering the motor directly from the board, you may want/need 
	// to avoid running other high-current operations while a tick pulse is active.
	// This would be the place to, if needed, deactivate any other loads just before the pulse switches HIGH
	// A symtom of the motor not getting enough power would be it failing to tick properly and getting stuck.
}

/**
* callback - A tick pulse just completed
*/
void onTickComplete(){
	Serial.println("onTickComplete");
	// tick pulse complete...
	// It's safe to draw a bit of current now if you need to read a sensor or something...
   
}