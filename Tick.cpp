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

#include "Arduino.h"
#include "Tick.h"

Tick::Tick() {
	
}

/**
* @param clockA - clock pin A
* @param clockB - clock pin B
* @param tickLength - milliseconds
**/	
Tick::Tick(int clockA, int clockB, int tickLength){
	setup(clockA, clockB, tickLength, NULL, NULL);
}


/**
* @param clockA - clock pin A
* @param clockB - clock pin B
* @param tickLength - milliseconds
* @param tickStart - callback
* @param tickComplete - callback
**/	
Tick::Tick(int clockA, int clockB, int tickLength, 
	void (*tickStart)(), void (*tickComplete)()) {
	setup(clockA, clockB, tickLength, tickStart, tickComplete);
}

/** 
* @public
* @param clockA - clock pin A
* @param clockB - clock pin B
* @param tickLength - milliseconds
**/	
void Tick::setup(int clockA, int clockB, int tickLength){
	setup(clockA,clockB,tickLength, NULL, NULL);
}

/**
* @public
* @param clockA - clock pin A
* @param clockB - clock pin B
* @param tickLength - milliseconds
* @param tickStart - callback
* @param tickComplete - callback
**/	
void Tick::setup(int clockA, int clockB, int tickLength,
	void (*tickStart)(), void (*tickComplete)()){
	
	this->tickStart  	= tickStart;
	this->tickComplete  = tickComplete;
	tickPin	 			= clockA;
	totalCount			= 0;
	paused				= false;
	setClockPins(clockA, clockB);
	setTickLength(tickLength);
}



void Tick::stepMillisecondDelta(int delta){
	if(paused) return;
	milliseconds+=delta;
	if(tickPulse){
		if(milliseconds >= PULSE_LENGTH) pulseEnd();		
	} else if(milliseconds >= tickLength){ 
		pulseStart();
	}
}

/**
* 
*
*/
void Tick::stepMillisecond(){
	if(paused) return;
	milliseconds++;
	if(tickPulse){
		if(milliseconds == PULSE_LENGTH) pulseEnd();		
	} else if(milliseconds == tickLength){ 
		pulseStart();
	}
}

/**
*
*
*/
void Tick::pulseStart(){
	milliseconds 	-= tickLength;
	tickPulse 		= true;
	digitalWrite(tickPin, HIGH);
	if(tickStart!=NULL) tickStart();
}

/**
*
*
*/
void Tick::pulseEnd(){
	tickPulse = false;
	digitalWrite(tickPin, LOW);
	// switch field direction
	tickPin = tickPin == clockA ? clockB : clockA;
	
	// tick!
	count++; totalCount++;
	if(tickComplete!=NULL) tickComplete();
}


/**
* @public 
* Pause the tick count - the same as not calling update - update returns without action if pause==true
*/
void Tick::pause(){
	paused = true;
	reset();
}

/**
* @public 
* Resume from a pause() call
*/
void Tick::resume(){
	paused = false;
}

/**
* @public 
* The current tick count. This is reset to zero by calls to Tick::reset
*/
int Tick::getCount(){
	return count;
}

/**
* @public 
* The total tick count since setup. This is not reset by calls to Tick::reset, but is reset in Tick::setup
*/
int Tick::getTotalCount(){
	return totalCount;
}

/**
* @public 
* Set the output pins used for the alternate driver pulses.
* Setting the pins also sets the pinMode to OUTPUT for the selected pins.
* @param a	Pin A
* @param b  Pin B
*/
void Tick::setClockPins(int a, int b){
	clockA = a;
	clockB = b;
	pinMode(a, OUTPUT);
	pinMode(b, OUTPUT);
}


/**
* @public 
* @param value - tick length, milliseconds. Limited to a minimum of MIN_TICK_LENGTH (see Tick.h)
*/
void Tick::setTickLength(int value){
	tickLength = value < MIN_TICK_LENGTH ? MIN_TICK_LENGTH : value;
	reset();
}


/**
* @public 
* Reset the counters and retrigger a tick/pulse
*/
void Tick::reset(){
	count				= 0;
	milliseconds		= tickLength-1;
	tickPulse			= false;
	digitalWrite(clockA, LOW);
	digitalWrite(clockB, LOW);
}