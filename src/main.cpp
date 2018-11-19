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
#define DBG1(fmt,str) printf(fmt,str)
#define DBGP(str) if(previous != state) { printf(str);previous=state;}
#else
#define DBG(str)
#define DBG1(fmt,str)
#endif


//=============================================================================================

PCA9685 pwmController;   // Library using default Wire and default linear phase balancing scheme


const int NUMLEDS=12;	// Number of led's in the chain
const int UPPIN=2;		// pin for up pointing motions sensor
const int DWNPIN=3;	// pin for down pointing motion sensor

const int TabCnt = (sizeof(ledvals) / sizeof(ledvals[0]));
const int TabSteps = TabCnt / NUMLEDS;

int state;				/// In which state is the led chain
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

enum Reset {Reset=true, Continue=false};

// Ir Motion sensor handling class

class MotionSensor {
	unsigned char pin;
	Bounce bouncer;
public:
	int state;
	int lastState;
	int interval;

// States of motion sensors
enum SensorStates {
	Off=false,
	On=true
};
	MotionSensor(){
		state = Off;
		lastState = Off;
		bouncer=Bounce();
		pin=-1;
		interval=50;
	};

	void init(unsigned char pin){
		DBG1("MotionSensor Init pin %d\n",pin);
		bouncer.attach(pin,INPUT);
		bouncer.interval(50);
	};

	int check() {
		bouncer.update();
		if(bouncer.fell()) {
				state=Off;
		}
		if(bouncer.rose()) {
			state=On;
		}
		return state;
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
		pwmController.setChannelPWM(led, pgm_read_word_near(ledvals+i) );
	}
	pwmController.setChannelOn(led);
}

/**
 * \brief lightUp : turn on leds going downwards
 */
void turnOnDwn()
{
	DBG("Turning leds On Down\n");
	for (int led=NUMLEDS;led>=0;led--)	{
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
	int step = TabCnt / NUMLEDS;

	for(int i=0;i<TabCnt;i++) {
		pwmController.setChannelPWM(led, pgm_read_word_near(ledvals+i) );

	}
	pwmController.setChannelOn(led);
	for (int led=0;led<NUMLEDS;led++)	{
		rampUp(led);
	}
}

/**
 * \brief rampUpDwn : increase / decrease intensity
 * stepping up or down the list of leds
 *
 * the intensity values are taken from the ledvals table since intensity values are
 * logarithmic to be pleasing.
 *
 * \param dir direction to go Up or Down
 * \param incDec increase or decrease itensity
 * \param reset restart from top or bottom
 */
bool rampUpDwn(int dir,int incDec ,bool reset)
{
	static int step = TabCnt; // current step
	static int currentLed = 0;


	if (reset){ // Are we reseting to start ?
		DBG("Ramp Reset:");
		step		=(incDec==Increase)?0:TabCnt;
		currentLed	=(dir	==Up)?0:NUMLEDS-1;
#ifdef DEBUG
		printf("Dir:%s, IncDec: %s, Step: %d, CurrentLed: %d\n",
					(dir==Up?"Up":"Down"),(incDec==Increase?"Inc":"Dec"),step, currentLed);
#endif
		return false;
	} else if ((dir == Down && currentLed <0) || (dir == Up && currentLed >=NUMLEDS)) {
			return true;
	}

	// check for boundaries of the intensity table
    if (step <0 || step>TabCnt) {
    		if (incDec == Decrease) {
    			DBG1("Ramp: Led %d Off ",currentLed);
    			pwmController.setChannelOff(currentLed); // yes turn if fully off
    		} else  {
    			DBG1("Ramp: Led %d On ",currentLed);
    			pwmController.setChannelOn(currentLed); // yes turn it fully on
    		}
			currentLed=(dir==Down)?currentLed-1:currentLed+1;		// and step to the next leds
			step=(incDec==Decrease)?TabCnt:0;						// restart sequence
			DBG1(" Current Led : %d\n",currentLed);
    } else {
		pwmController.setChannelPWM(currentLed, pgm_read_word_near(ledvals+step));
		step=(incDec==Decrease)?step-1:step+1;		// step up/down to next intensity value
    }
	return false; 				// not done yet
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

	for  ( int i=0;i<NUMLEDS;i++) {
    	pwmController.setChannelOff(i);
	}
    DBG("Init leds full on\n");
	for  ( int i=0;i<NUMLEDS;i++) {
		for (int j=0;j<=i;j++) {
			pwmController.setChannelPWM(j, 4096);
			delay(20);
		}
	}
    DBG("Init Ramping down\n");
	delay(1000);
	for (int j=TabCnt;j>0;j--) {
		for  ( int i=NUMLEDS;i>=0;i--) {
	    	pwmController.setChannelPWM(i, pgm_read_word_near(ledvals+j));
		}
	}
    DBG("Init Blink leds off\n");

	for  (int i=NUMLEDS;i>=0;i--) {
	    	pwmController.setChannelOff(i);
	}
	DBG("Setting up sensor pins\n");
	// Setup sensor pins
	upSens.init(UPPIN);
	dwnSens.init(DWNPIN);
	state=Off;
	DBG("Setup done\n");
}

void loop()
{
	int upState,dwnState;

	upState=upSens.check();
	dwnState=dwnSens.check();
	// Someone going up or have gone up
	switch (state) {

	// --- Lights are off ---
	case Off:
		DBGP("State: Off\n");
		if (upState == MotionSensor::On) { // Someone going up
			turnOnUp();
			state=OnUp;
			upSens.setInterval(300); // wait longer for going down again
		} else if(dwnState == MotionSensor::On) { // Someone going down
			turnOnDwn();
			state=OnDwn;
			upSens.setInterval(300); // wait longer for going down again
		}
		break;

	// --- Lights turned on by going up ---
	case OnUp:
		DBGP("State: OnUp\n");
		if (upState == MotionSensor::Off && dwnState == MotionSensor::Off) { // both sensors are off , no one in stairs
			state=TurningOffUp;
			rampUpDwn(Up,Decrease,Reset); // start turning of leds uppwards
			upSens.setInterval(50); // shorter debounce for movement detection
		} else if (dwnState == MotionSensor::On) {	// someone going down, switch direction
			state=OnDwn;
		}
		// else UpState == On so do nothing
		break;

	// --- light turned on by going down ---
	case OnDwn:
		DBGP("State: OnDwn\n");
		if (dwnState == MotionSensor::Off && upState == MotionSensor::Off) { // Both motions sensors are off
			state=TurningOffDwn;
			rampUpDwn(Down,Decrease,Reset); // start turning of leds Downwards
			upSens.setInterval(50); 		// shorter debounce for movement detection
		} else if (upState == MotionSensor::On) {	// someone going up, switch direction
			state=OnUp;
		}
		// else DwnState == On or lastDwnState == Off -> dont care
		break;

	//  --- Lights turning off after going up ---
	case TurningOffUp:
		DBGP("State: TurningOffUp\n");
			// Motion detected ?
			if((upState == MotionSensor::On) || (dwnState == MotionSensor::On)) {
				DBG("Detected motion\n");
				while (!rampUpDwn(Down,Increase,Continue)) {
					// Quickly turn led's back on again going downwards
				}
				state = (upState==MotionSensor::On)?OnUp:OnDwn; // go back to onUp/onDown State
				upSens.setInterval(300); 						// Long detect interval for no movment
			} else {
				if(rampUpDwn(Up,Decrease,Continue)){			// step down lights 1 step untill done
					state=Off; 									// if we are done turning off leds go to off state
					upSens.setInterval(100); 					// Short detect  interval for movement detection
					DBG("Ramp Up Decrese done\n");
				}
			}
		break;

	 // --- Lights turning off after going down ---
	case TurningOffDwn:
		DBGP("State: TurningOffDwn\n");
			if((upState == MotionSensor::On) || (dwnState == MotionSensor::On)) { // someone started going up or down
				DBG("Detected motion\n");
#ifdef DEBUG
				printf("Up: %d, Down: %d \n", upState, dwnState);
#endif
				while (!rampUpDwn(Up,Increase,Continue)) {
					// Turn Leds back on again going downwards
				}
				upSens.setInterval(300); 						// Long detect intervall for no movment detect
				state = (upState==MotionSensor::On)?OnUp:OnDwn; // go back to onUp State
				DBG1("New state: %s",(upState==MotionSensor::On)?"OnUp":"OnDwn")
			} else {
				if(rampUpDwn(Down,Decrease,Continue)){ 			// step down lights 1 step untill done
					state=Off; 						 			// if we are done turning off led's go to off state
					upSens.setInterval(100); 		 			// Short detect interval for movement detection
					DBG("Ramp Done Decrese done\n");
				}
			}
		break;

	// ==== should not happen, invalid state detected, do nothrin / reset the hole thing ? ===
	// TODO: what to do here ? for now just ignore it
	default:
		DBG("Alert!!: Uknown state detected\n");
		state=Off;
		for (int led=0;led<NUMLEDS;led++) {
    			pwmController.setChannelOff(led); // yes turn if fully off
		}
	};
}
