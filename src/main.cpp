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

#define DEBUG
#ifdef DEBUG
// Function that printf and related will use to print
FILE serial_stdout;
int serial_putchar(char c, FILE* f) {
    if (c == '\n') serial_putchar('\r', f);
    return !Serial.write(c);
}
int previous;
#define DBG(str) printf(str)
#define DBG1(fmt,...) printf(fmt,__VA_ARGS__)
#define DBGP(str) if(previous != State) { printf(str);previous=State;}
#else
#define DBG(str)
#define DBG1(fmt,...)
#define DBGP(str)
#endif


//=============================================================================================

PCA9685 pwmController;   // Library using default Wire and default linear phase balancing scheme


const int NumLeds=12;	// Number of led's in the chain
const int LastLed=NumLeds-1;
const int UPPIN=2;		// pin for up pointing motions sensor
const int DWNPIN=3;	// pin for down pointing motion sensor

const int TabCnt = (sizeof(LedVals) / sizeof(LedVals[0]));
const int LastTab = TabCnt-1;				// last member of intensity tableo
const int MaxIntens = TabCnt/10*8;
const int MidIntens = TabCnt/10*6;
const int LowIntens = TabCnt/10*4;

const int LongWait = 3000;
const int ShortWait = 100;
int leds[NumLeds];
int State;				/// In which state is the led chain

enum LightState {
	Off,				// All leds off
	OnDwn,				// Someone going down detected, lights are on
	OnUp,				// Someone going up detected, lights are on
	TurningOffUp,		// Turning lights of uppwards
	TurningOffDwn		// Turning lights off downwards
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

// Ir Motion sensor handling class

class MotionSensor {
	unsigned char pin;
	Bounce bouncer;
public:
	int state;
	bool stateFlag;
	int interval;

// States of motion sensors
enum SensorStates {
	Off=false,
	On=true
};
	MotionSensor(){
		state = Off;
		stateFlag=false;
		bouncer=Bounce();
		pin=-1;
		interval=ShortWait;
	};

	void init(unsigned char pin){
		DBG1("MotionSensor Init pin %d\n",pin);
		bouncer.attach(pin,INPUT);
		bouncer.interval(ShortWait);
	};

	int check() {
		if(stateFlag==false) {
			bouncer.update();
			if(bouncer.fell()) {
				state=Off;
			}
			if(bouncer.rose()) {
				state=On;
			}
			stateFlag=true;
		}
		return state;
	}

	int getState() {
		return state;
	}

	void clrFlag() {
		stateFlag=false;
	}

	void setInterval(int newinterval) {
		bouncer.interval(newinterval);
	}

};

MotionSensor upSens,dwnSens;


// ===========================================================================================
/*
 * rampUp : slowly turn on a single led using table values
 */
void rampUp(int led)
{
	for(int i=0;i<TabCnt;i++) {
		pwmController.setChannelPWM(led, pgm_read_word_near(LedVals+i) );
	}
	pwmController.setChannelOn(led);
}

/**
 * \brief lightUp : turn on leds going downwards
 */
void turnOnDwn()
{
	DBG("Turning leds On Down\n");
	for (int led=LastLed;led>=0;led--)	{
		rampUp(led);
	}
}

/**
 * \brief lightUp : turn on leds going upward
 */
void turnOnUp()
{
	DBG("Turning leds On Up\n");
	int led = 0;

	for(int i=0;i<TabCnt;i++) {
		pwmController.setChannelPWM(led, pgm_read_word_near(LedVals+i) );

	}
	pwmController.setChannelOn(led);
	for (int led=0;led<LastLed;led++)	{
		rampUp(led);
	}
}
int slowramp (int dir, int incDec,int reset){

	static int curStep;	//index into led intensity tab
	static int curLed;

	if (reset==Reset) { // Are we reseting to start ?
		DBG1("Ramp Reset: %s %s\n",(dir == Up?"Up":"Down"),(incDec==Increase?"Inc":"Dec"));
		if(incDec==Increase) {
			curStep=0;
			curLed=(dir==Up)?0:LastLed;
		} else { // Decrease
			curStep=MaxIntens;
			curLed=(dir==Up)?0:LastLed;
		}
		return Continue;
	}
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

	} else if (dir == Up   && incDec == Decrease){
		DBG("Ramp Up Decrease\n");
		for(curStep=MaxIntens;curStep> MidIntens;curStep--) {
			for (curLed=0;curLed<NumLeds;curLed++) {
				pwmController.setChannelPWM(curLed,pgm_read_word_near(LedVals+curStep));
				if (upSens.check() == MotionSensor::On || dwnSens.check() == MotionSensor::On) {
					DBG("Ramp Up Dec: 1 Detected  motion\n");
					return Motion;
				}
			}
		}
		for (curLed=0;curLed<NumLeds;curLed++) {
			for(curStep=MidIntens;curStep >5;curStep--){
				pwmController.setChannelPWM(curLed,pgm_read_word_near(LedVals+curStep));
				if (upSens.check() == MotionSensor::On || dwnSens.check() == MotionSensor::On) {
					DBG("Ramp Up Dec: 2 Detected  motion\n");
					return Motion;
				}
				delay(1);
			}
			pwmController.setChannelOff(curLed);
			delay(40);
		}
		DBG("Ramp Up Decrease Done\n");
		return Done;
	} else if (dir == Down && incDec == Increase){

	} else if (dir == Down && incDec == Decrease){

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

   DBG("initalizing i2c\n");

    Wire.begin();                       // Wire must be started first
    Wire.setClock(400000);              // Supported baud rates are 100kHz, 400kHz, and 1000kHz

    DBG("PWMctrl init\n");
    pwmController.resetDevices();       // Software resets all PCA9685 devices on Wire line
    pwmController.init(B000000);
    // Address pins A5-A0 set to B000000
    pwmController.setPWMFrequency(100); // Default is 200Hz, supports 24Hz to 1526Hz

    // Initalisation blink
    DBG("Init Blink0\n");

	for  ( int i=0;i<NumLeds;i++) {
    	pwmController.setChannelOff(i);
	}
    DBG("Init leds full on\n");
	for  ( int i=0;i<NumLeds;i++) {
		for (int j=0;j<=i;j++) {
			pwmController.setChannelPWM(j, 4096);
			delay(10);
		}
	}
    DBG("Init Ramping down\n");
	delay(1000);
	for (int j=TabCnt;j>0;j--) {
		for  ( int i=LastLed;i>=0;i--) {
	    	pwmController.setChannelPWM(i, pgm_read_word_near(LedVals+j));
		}
	}
    DBG("Init Blink leds off\n");

	for  (int i=LastLed;i>=0;i--) {
	    	pwmController.setChannelOff(i);
	    	leds[i]=0;
	}
	DBG("Setting up sensor pins\n");
	// Setup sensor pins
	upSens.init(UPPIN);
	dwnSens.init(DWNPIN);
	State=Off;
	DBG("Setup done\n");
}

void loop()
{
	int upState,dwnState;

	upState=upSens.check();
	dwnState=dwnSens.check();
	// Someone going up or have gone up
	switch (State) {

	// --- Lights are off ---
	case Off:
		upSens.clrFlag();
		DBGP("State: Off\n");
		if (upState == MotionSensor::On) { // Someone going up
			DBG("Up motion detected:\n");
			slowramp(Up,Increase,Reset);
			slowramp(Up,Increase,Continue);
			State=OnUp;
			upSens.setInterval(LongWait); // wait longer for ramping down again
		} else if(dwnState == MotionSensor::On) { // Someone going down
			DBG("Down motion detected:\n");
			slowramp(Down,Increase,Reset);
			slowramp(Down,Increase,Continue);
			State=OnDwn;
			upSens.setInterval(LongWait); // wait longer for ramping down again
		}
		break;

	// --- Lights turned on by going up ---
	case OnUp:
		upSens.clrFlag();
		DBGP("State: OnUp\n");
		if (upState == MotionSensor::Off && dwnState == MotionSensor::Off) { // both sensors are off , no one in stairs
			DBG("No up motion detected:\n");
				// start turning of leds uppwards
			State=TurningOffUp;
			slowramp(Up,Decrease,Reset);
			upSens.setInterval(ShortWait); // shorter debounce for movement detection
		} else if (upState == MotionSensor::On && dwnState == MotionSensor::On ) {	// someone going down, switch direction
			// TODO: this might be wrong ,down sensor will trigger when turning corner...
			State=OnDwn;
		}
		// else UpState Still on or DwnState On, Sensors should flips midway
		break;

	//  --- Lights turning off after going up ---
	case TurningOffUp:
		upSens.clrFlag();
		DBGP("State: TurningOffUp\n");
		// Motion detected ?
		if((upState == MotionSensor::On) || (dwnState == MotionSensor::On)) {
			DBG("TurningOffup:1 Motion detected\n");
			// Ramp back up
			slowramp(Up,Increase,Continue);
			State = (upState==MotionSensor::On)?OnUp:OnDwn; // go back to onUp/onDown State
			upSens.setInterval(LongWait); 						// Long detect interval for no movment
			} else {
				//Slowramp might return Motion, we let the next iteration wory about that
				if(slowramp(Up,Decrease,Continue)==Done){			//
					State=Off; 										// if we are done turning off leds go to off state
				} else {
					DBG("TurningOffup:2 Motion detected\n");
				}
			}
		break;
	// --- light turned on by going down ---

	case OnDwn:
		upSens.clrFlag();
		DBGP("State: OnDwn\n");
		if (dwnState == MotionSensor::Off && upState == MotionSensor::Off) { // Both motions sensors are off
			DBG("No Down motion detected:\n");
			State=TurningOffDwn;
			slowramp(Down,Decrease,Reset); // start turning of leds Downwards
			upSens.setInterval(ShortWait); 		// shorter debounce for movement detection
		} else if (dwnState == MotionSensor::On && upState == MotionSensor::On) {	// someone going up, switch direction
			State=OnUp;
		}
		break;


	 // --- Lights turning off after going down ---
	case TurningOffDwn:
		upSens.clrFlag();
		DBGP("State: TurningOffDwn\n");
			if((upState == MotionSensor::On) || (dwnState == MotionSensor::On)) { // someone started going up or down
				DBG("Detected motion\n");
				slowramp(Up,Increase,Continue);
				State = (upState==MotionSensor::On)?OnUp:OnDwn; // go back to onUp State
				upSens.setInterval(LongWait); 	 					// Long detect intervall for no movment detect
				upSens.clrFlag();
			} else {
				if(slowramp(Down,Decrease,Continue)==Done){ 			// step down lights 1 step untill done
					State=Off; 						 			// if we are done turning off led's go to off state
					upSens.setInterval(ShortWait); 		 			// Short detect interval for movement detection
				}
			}
		break;

	// ==== should not happen, invalid state detected, do nothrin / reset the hole thing ? ===
	// TODO: what to do here ? for now just ignore it
	default:
		upSens.clrFlag();
		DBG("Alert!!: Uknown state detected\n");
		State=Off;
		for (int led=0;led<LastLed;led++) {
    			pwmController.setChannelOff(led); // yes turn if fully off
		}
	};
}
