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


#include <Bounce.h>

#include <TimerOne.h> // http://playground.arduino.cc/code/timer1
#include <Tick.h>


// enable compile-time debug trace over Serial...
#define DEBUG false



Tick tick;
//Tick tick(2, 3, 1000);
//Tick tick(2, 3, 1000, onTickPulseStart, onTickPulseComplete);

const int CLOCK_A           = 8;
const int CLOCK_B           = 9;

boolean ticking=false;

void setup() {
  
  #if DEBUG 
    Serial.begin(9600); 
  #endif
  
  // set up the pulse generator using pins 8 and 9, with a 1000ms tick duration
  //tick.setup(8, 9, 1000);
  tick.setup(CLOCK_A, CLOCK_B, 1000, onTickPulseStart, onTickPulseComplete); // also using the tick-complete callback...
 
  Timer1.initialize(1000); // a tick every 1ms
  Timer1.attachInterrupt(onTimer);
  
  // Tick methods:
  // tick.setClockPins(8,9);
  // tick.setTickLength(1000);
  // tick.pause();
  // tick.resume();
  // tick.reset();
  // tick.getCount();
  // tick.getTotalCount();
}

// Timer1 interrupt wrapper - called at 1000Hz - 1ms per timer-tick
void onTimer(){    
    tick.stepMillisecond();
}

void loop() {
  /* if not using an interrupt timer (Timer1 in this example), update tick in the main loop
  // calculate the ms differnce since the last time round
  unsigned long time = millis();
  unsigned long dt   = time - lastMillis;
  lastMillis 	     = time;
  if(dt>0) {
    tick.stepMillisecondDelta(dt);
  }
  //*/
  
  if(!ticking){
    //readSensors
    //checkButtons
    //doStuff
  }
}

void onTickPulseStart(){  
   #if DEBUG
   
   //Serial.println("start");
   // NOTE: you only have PULSE_LENGTH milliseconds to do things here before onTickPulseComplete interrupt fires
    // If powering the motor directly from the board, you may want/need 
	// to avoid running other high-current operations while a tick pulse is active.
	// This would be the place to, if needed, deactivate any other loads just before the pulse switches HIGH
	// A symtom of the motor not getting enough power would be it failing to tick properly and getting stuck.
   #endif
   
   ticking=true;
}

void onTickPulseComplete(){  
   #if DEBUG
     Serial.println("tick");
   #endif
  
  // tick pulse complete...
  // It's safe to draw a bit of current now if you need to read a sensor or something...

   // continuous sensor reading, or reading while the clock tick pulse is active,
   // causes a shotfall in the power available to the clock causing it to stop ticking properly.
   
   ticking=false;
}
