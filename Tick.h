/*Copyright (c) 2013 Mike Almond - @mikedotalmond - https://github.com/mikedotalmond

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.*/

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

#ifndef Tick_h
#define Tick_h

#include "Arduino.h"

// Driver pulse length in milliseconds 
// You may need to tweak this value depending on the type of clock being used.
#define PULSE_LENGTH 11 
// the motor needs time to rotate between pulses,
// so the MIN_TICK_LENGTH multiplier sets the minimum tick length based on a multiple of PULSE_LENGTH
// Reduce the multipler if you want to get faster tick speeds, but there will be a limit to how fast 
// the stepper can move into position for the next tick... a 66ms total-tick duration is just about as fast as it will tick for me.
#define MIN_TICK_LENGTH PULSE_LENGTH*6


class Tick {
	
   public:
    Tick();
	
    Tick(int clockA, int clockB, int tickLength);
	void setup(int clockA, int clockB, int tickLength);
	
	Tick(int clockA, int clockB, int tickLength, 
		void (*tickStart)(), void (*tickComplete)());
    
	void setup(int clockA, int clockB, int tickLength, 
		void (*tickStart)(), void (*tickComplete)());
	
	void setClockPins(int a, int b);
	void setTickLength(int value);
	
	void update(int delta);
	void pause();
	void resume();
	void reset();
	int getCount();
	int getTotalCount();
	
  protected:
	int count;
	int totalCount;
    int clockA;
    int clockB;
	int tickLength;
	int tickPin;
	int counter;
	int pulseLength;
	bool tickPulse;
	bool paused;
	
	void (*tickComplete)();
	void (*tickStart)();
};

#endif