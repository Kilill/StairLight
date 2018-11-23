/*============================================================================================*\
 * stair led control
 * Listens to 2 motions sensors one up and one down and depending on which one triggers starts
 * a lightning sequence.
 * After the any of the motion sensors is no longer detection motion a countdown is started
 * afterwich the estinguish sequence is started.
 *\===========================================================================================*/

#include <Wire.h>
#define PCA9685_EXCLUDE_SERVO_EVAL
#define PCA9685_EXCLUDE_EXT_FUNC
#include "PCA9685.h"
#include <Bounce2.h>
#include "ledvals.h"

//#define DEBUG
#ifdef DEBUG
// Function that printf and related will use to print
FILE serial_stdout;
int serial_putchar(char c, FILE* f) {
    if (c == '\n') serial_putchar('\r', f);
    return !Serial.write(c);
}
int previous;
#define DBG(fmt) printf(fmt)
#define DBGF(fmt,...) printf(fmt,__VA_ARGS__)
#define DBGP(str) if(previous != State) { printf(str);previous=State;}
#else
#define DBG(fmt)
#define DBGF(fmt,...)
#define DBGP(str)
#endif


//=============================================================================================

PCA9685 pwmController;   // Library using default Wire and default linear phase balancing scheme


const int NumLeds=12;	// Number of led's in the chain
const int LastLed=NumLeds-1;
const int UPPIN=2;		// pin for going up, ie it points downwards
const int DWNPIN=3;		// pin for going down , ie it points upwards

const int TabCnt = (sizeof(LedVals) / sizeof(LedVals[0]));
const int LastTab = TabCnt-1;				// last member of intensity tableo
const int MaxIntens = TabCnt/10*8;
const int MidIntens = TabCnt/10*6;
const int LowIntens = TabCnt/10*4;

const int LongWait = 5000;
const int ShortWait = 100;

int State;				/// In which state is the led chain

enum LightState {
	Off,				// All leds off
	OnDown,				// Someone going down detected, lights are on
	OnUp,				// Someone going up detected, lights are on
	TurningOffUp,		// Turning lights of uppwards
	TurningOffDown		// Turning lights off downwards
};

/// used in rampUpDwn to set direction in the list of leds
enum LedDir {
	Down=false, 		// Stepping downwards in the list
	Up=true				// Stepping upwards in the list
};

/// used in rampUpDwn to set change intensity direction
enum LedChange  {
	Decrease=false, 	// Decrease intensity
	Increase=true		// Increase intensity
};

enum RampStat {Reset, Continue,Motion,Done};

Bounce sensUp,sensDown;


// ===========================================================================================
// Ramp lights up and down,
int ramp (int dir, int incDec,int reset){

	static int curStep;	//index into led intensity tab
	static int curLed;	// index to current led

	if (reset==Reset) { // Are we reseting to start ?
		DBGF("Ramp Reset: %s %s\n",(dir == Up?"Up":"Down"),(incDec==Increase?"Inc":"Dec"));
		if(incDec==Increase) {
			curStep=0;
			curLed=(dir==Up)?0:LastLed;
		} else { // Decrease
			curStep=MaxIntens;
			curLed=(dir==Up)?0:LastLed;
		}
		return Continue;
	}
	// ==== Light each led in sequence to midway intensity, then increase intensity for all of them

	if (dir == Up   && incDec == Increase){
		DBG("Ramp Up Increase\n");
		// We might enter here from an aborted Decrease
		// So curStep and CurLed might be somewhere mid step
		// The ramp up is never interrupted so run it to the end
		if(curStep<MidIntens) { // check in which part we should start
			for(;curLed <NumLeds;curLed++){
				for(;curStep<MidIntens;curStep++){
					pwmController.setChannelPWM(curLed,pgm_read_word_near(LedVals+curStep));
				}
				curStep=0;
			}
			delay(300);
			curLed=0;
			curStep=MidIntens;
		}
		for (;curStep<=MaxIntens;curStep++) {
			for(;curLed <NumLeds;curLed++){
				pwmController.setChannelPWM(curLed,pgm_read_word_near(LedVals+curStep));
			}
			curLed=0;
			delay(1);
		}
		DBG("Done\n");
		return Done;

	// ==== Decrease all led's to mid intensity, then decrease each in sequence upwards

	} else if (dir == Up   && incDec == Decrease){

		DBG("Ramp Up Decrease\n");
		for(curStep=MaxIntens;curStep> MidIntens;curStep--) {
			for (curLed=0;curLed<NumLeds;curLed++) {
				// check for movment and possibly abort
				if (sensUp.update()|| sensDown.update()) {
					DBGF("Ramp Up Dec: 1 Detected  motion CurStep: %d curLed %d\n",curStep,curLed);
					if(sensUp.rose()) return OnUp;
					if(sensDown.rose()) return OnDown;
				}
				pwmController.setChannelPWM(curLed,pgm_read_word_near(LedVals+curStep));
			}
		}
		for (curLed=0;curLed<NumLeds;curLed++) {
			for(curStep=MidIntens;curStep >5;curStep--){
				pwmController.setChannelPWM(curLed,pgm_read_word_near(LedVals+curStep));
				// check for movment and possibly abort
				if (sensUp.update()|| sensDown.update()) {
					DBG("Ramp Up Dec: 1 Detected  motion\n");
					if(sensUp.rose()) return OnUp;
					if(sensDown.rose()) return OnDown;
				}
				delay(1);
			}
			pwmController.setChannelOff(curLed);
			delay(40);
		}
		DBG("Ramp Up Decrease Done\n");
		return Off;

	// ==== Increase downwards

	} else if (dir == Down && incDec == Increase){

		DBG("Ramp Down Increase\n");
		// We might enter here from an aborted Decrease
		// So curStep and CurLed might be somewhere mid step
		// The ramp up is never interrupted so run it to the end
		if(curStep<MidIntens) { // check in which part we should start
			for(;curLed >=0;curLed--){
				for(;curStep<MidIntens;curStep++){
					pwmController.setChannelPWM(curLed,pgm_read_word_near(LedVals+curStep));
				}
				curStep=0;
			}
			delay(300);
			curLed=NumLeds;
			curStep=MidIntens;
		}
		for (;curStep<=MaxIntens;curStep++) {
			for(;curLed >=0;curLed--){
				pwmController.setChannelPWM(curLed,pgm_read_word_near(LedVals+curStep));
			}
			curLed=NumLeds;
			delay(1);
		}
		DBG("Done\n");
		return Done;

	// ==== Decrease downwards

	} else if (dir == Down && incDec == Decrease){

		DBG("Ramp Down Decrease\n");
		for(curStep=MaxIntens;curStep> MidIntens;curStep--) {
			for (curLed=NumLeds;curLed>=0;curLed--) {

				// check for movement and possibly abort
				if (sensUp.update()|| sensDown.update()) {
					DBGF("Ramp Down Dec: 1 Detected  motion CurStep: %d curLed %d\n",curStep,curLed);
					if(sensUp.rose()) return OnUp;
					if(sensDown.rose()) return OnDown;
				}
				pwmController.setChannelPWM(curLed,pgm_read_word_near(LedVals+curStep));
			}
		}
		for (curLed=NumLeds;curLed>=0;curLed--) {
			for(curStep=MidIntens;curStep >5;curStep--){
				pwmController.setChannelPWM(curLed,pgm_read_word_near(LedVals+curStep));
				// check for movement and possibly abort
				if (sensUp.update()|| sensDown.update()) {
					DBG("Ramp Up Dec: 1 Detected  motion\n");
					if(sensUp.rose()) return OnUp;
					if(sensDown.rose()) return OnDown;
				}
				delay(1);
			}
			pwmController.setChannelOff(curLed);
			delay(40);
		}
		DBG("Ramp Up Decrease Done\n");
		return Off;
    }
	return Continue;
}
void setup() {
#ifdef DEBUG
    Serial.begin(115200);
    // Set up stdout
   fdev_setup_stream(&serial_stdout, serial_putchar, NULL, _FDEV_SETUP_WRITE);
   stdout = &serial_stdout;
   previous=-1;
#endif

   DBG("Initalizing i2c\n");

    Wire.begin();                       // Wire must be started first
    Wire.setClock(400000);              // Supported baud rates are 100kHz, 400kHz, and 1000kHz

    DBG("PWMctrl init\n");

    pwmController.resetDevices();       // Software resets all PCA9685 devices on Wire line
    pwmController.init(B000000); 		// Address pins A5-A0 set to B000000 by default
    pwmController.setPWMFrequency(200); // Default is 200Hz, supports 24Hz to 1526Hz

    // Do the startup blink

    DBG("Init Blink0\n");

	for  ( int i=0;i<NumLeds;i++) {
    	pwmController.setChannelOff(i);
	}
	ramp(Up,Increase,Reset);

	ramp(Up,Increase,Continue);
	ramp(Down,Decrease,Continue);
	ramp(Down,Increase,Continue);
	ramp(Up,Decrease,Continue);

	DBG("Setting up sensor pins\n");
	// Setup sensor pins
	sensUp.attach(UPPIN,INPUT);
	sensUp.interval(ShortWait);

	sensDown.attach(DWNPIN,INPUT);
	sensDown.interval(ShortWait);

	State=Off;
	DBG("Setup done\n");
}

void loop()
{
	switch (State) {

	// --- Lights are off ---
	case Off:
		DBGP("State: Off\n");
		if(sensUp.update()||sensDown.update()) {
			if (sensUp.rose()) { // Someone going up
				DBG("Up motion detected:\n");
				ramp(Up,Increase,Reset);
				ramp(Up,Increase,Continue);
				sensDown.interval(LongWait); // wait longer for ramping down again
				sensUp.interval(LongWait);
				State=OnUp;
			} else if(sensDown.rose()) { // Someone going down
				DBG("Down motion detected:\n");
				ramp(Down,Increase,Reset);
				ramp(Down,Increase,Continue);
				sensDown.interval(LongWait); // wait longer for ramping down again
				sensUp.interval(LongWait); // wait longer for ramping down again
				State=OnDown;
			}
		}
		break;

	// --- Turn on  going up ---
	case OnUp:
		DBGP("State: OnUp\n");
		if(sensUp.update() || sensDown.update()) { // Something changed
			/*
			 * We can not distinguish between some one coming down at the same
			 * time as someone going up since the sensor areas overlap in the corner,
			 * so when the up going person turns the corner the down sensor will rise
			 * and the and the up sensor will still be on, this is the exact same as some one
			 * starting to go down after someone goes up, and ofc the opposite is true as well.
			 */

			if (!sensUp.read() && !sensDown.read()) { // both sensors are off , no one in stairs
				DBG("No up motion detected:\n");
					// start turning of leds uppwards
				ramp(Up,Decrease,Reset);
				sensUp.interval(ShortWait); // shorter debounce for movement detection
				sensDown.interval(ShortWait); // shorter debounce for movement detection
				State=TurningOffUp;
			}
		}
		break;

	//  --- Turning off after going up ---
	case TurningOffUp:
		DBGP("State: TurningOffUp\n");
		// Motion detected ?
		if(	((State=ramp(Up,Decrease,Continue)) != Off) ) { // Something changed
			DBG("TurningOffup:1 Motion detected\n");
			// Ramp up the light again
			ramp(Down,Increase,Continue);
			sensUp.interval(LongWait);
			sensDown.interval(LongWait);
		}
		break;

	// --- Turn on going down ---
	case OnDown:
		DBGP("State: OnDwn\n");
		if(sensUp.update() || sensDown.update()) { // Something changed
			/*
			 * We can not distinguish between some one coming down at the same
			 * time as someone going up since the sensor areas overlap in the corner,
			 * so when the up going person turns the corner the down sensor will rise
			 * and the and the up sensor will still be on, this is the exact same as some one
			 * starting to go down after someone goes up, and ofc the opposite is true as well.
			 */

			if (!sensUp.read() && !sensDown.read()) { // both sensors are off , no one in stairs
				DBG("No up motion detected:\n");
					// start turning of leds uppwards
				ramp(Down,Decrease,Reset);
				sensUp.interval(ShortWait); // shorter debounce for movement detection
				sensDown.interval(ShortWait); // shorter debounce for movement detection
				State=TurningOffDown;
			}
		}
		break;


	 // --- Turning off after going down ---
	case TurningOffDown:
		DBGP("State: TurningOffDwn\n");
		// Motion detected ?
		if(	((State=ramp(Down,Decrease,Continue)) != Off) ) { // Something changed
			DBG("TurningOffup:1 Motion detected\n");
			// first ramp up the light again
			ramp(Up,Increase,Continue);
			sensUp.interval(LongWait);
			sensDown.interval(LongWait);
		}
		break;

	// ==== should not happen, invalid state detected, do nothin / reset the hole thing ? ===
	default:
		DBG("Alert!!: Uknown state detected\n");
		State=Off;
		for (int led=0;led<LastLed;led++) {
    			pwmController.setChannelOff(led); // yes turn if fully off
		}
	};
}
