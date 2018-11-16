
// PCA9685-Arduino Simple Example

#include <Wire.h>
#include "PCA9685.h"

#define HUE_MAX_RAINBOW 4095
#define HUE_MAX_SPECTRUM 4095

PCA9685 pwmController;                  // Library using default Wire and default linear phase balancing scheme
// Function that printf and related will use to print
FILE serial_stdout;
int serial_putchar(char c, FILE* f) {
    if (c == '\n') serial_putchar('\r', f);
    return !Serial.write(c);
}

void setup() {
    Serial.begin(115200);
    // Set up stdout
   fdev_setup_stream(&serial_stdout, serial_putchar, NULL, _FDEV_SETUP_WRITE);
   stdout = &serial_stdout;

    printf("initalizing i2c\n");

    Wire.begin();                       // Wire must be started first
    Wire.setClock(400000);              // Supported baud rates are 100kHz, 400kHz, and 1000kHz
    printf("reset Devices\n");

    pwmController.resetDevices();       // Software resets all PCA9685 devices on Wire line
    printf("init\n");
    pwmController.init(B000000);
    printf("Pwm = 100");
        // Address pins A5-A0 set to B000000
    pwmController.setPWMFrequency(100); // Default is 200Hz, supports 24Hz to 1526Hz

    	pwmController.setChannelPWM(0, 0);
    	pwmController.setChannelPWM(1, 0);
    	pwmController.setChannelPWM(2, 0);

}

struct HSV {
	int hue;
	int sat;
	int val;
};

struct RGB {
	int r;
	int g;
	int b;
};

char pr[80];
/*
hsv to rgb according to wikipedia formula (https://en.wikipedia.org/wiki/HSL_and_HSV)
C=V*S
H'=H/60
X=C*(1-|H' mod 2-1|)
R1,G1,B1= {
						(0,0,0) if H undefined
						(C,X,0) if 0 <= H' <= 1
						(X,C,0) if 1 <= H' <= 2
						(0,C,X) if 2 <= H' <= 3
						(0,X,C) if 3 <= H' <= 4
						(X,0,C) if 4 <= H' <= 5
						(C,0,X) if 5 <= H' <= 6
m=V-C
(R,G,B)=(R1+m,G1+m,B1+m)

Where
H = 0...360
S = 0...1
V = 0...1

--------

The formula has been modified to take S and V from 0...4096
and to only use integer arithmetic.

but can be modified for anny intervall by changeing the unity #define
take care that for unity = 4096 the computations will exceed 16 bit.
-----
struct definitions

struct HSV {
        unsigned int hue;
        unsigned int sat;
        unsigned int  val;
};

struct RGB {
        unsigned int r;
        unsigned int g;
        unsigned int b;
};

*/

#define unity (4096L)

void hsv2rgb(const struct HSV & hsv, struct RGB & rgb)
{
unsigned long c,x,m,hueSection,reminder;
//unsigned long x1,x2,x3,x4,x5,x6,x7,x8,r1,r2,r3,c1;

	// get the 1...6 section of the color space
	hueSection=hsv.hue/660;
	// compute reminder by scaling up to "unity", substact the non decimal parts
	// by getting rid of the decimal parts by integer  division and remultiplication
	// parentesis to try to force the division to happen before multiplication
	// same as shifting out the lower part and shifting back up, but depending on cpu multiplication might be faster
  // TODO: check that optimizer does not do funny tricks

  // printf(" H %5d, S %5d, V %5d ",hsv.hue,hsv.sat,hsv.val);

	reminder=(((hsv.hue*unity)/660) - (((hsv.hue*unity/660)/unity)*unity));

  c=(long)((long) hsv.sat*(long)hsv.val)/unity;
  // X=C*(1-|H' mod 2-1|) need to add reminder in here
	x = (long)((long) c * (long)((long) unity-((long)abs((long) ((long) hueSection % 2) * (long) unity+(long) reminder-(long)unity))))/(long)unity;

	m=hsv.val-c;
  // printf("Sec %2ld, Rem %5ld, C %5ld x %5ld",hueSection,reminder,c,x);

  c+=m;
	x+=m;

	switch(hueSection) {
		case 0:
			rgb.r=c;
			rgb.g=x;
			rgb.b=m;
			break;
		case 1:
			rgb.r=x;
			rgb.g=c;
			rgb.b=m;
			break;
		case 2:
			rgb.r=m;
			rgb.g=c;
			rgb.b=x;
			break;
		case 3:
			rgb.r=m;
			rgb.g=c;
			rgb.b=x;
			break;
		case 4:
			rgb.r=x;
			rgb.g=m;
			rgb.b=c;
			break;
		default:
			rgb.r=c;
			rgb.g=m;
			rgb.b=x;
			break;
		}
}
void loop() {

int i;
struct RGB rgb;
struct HSV hsv;

	hsv.sat=4096;
	hsv.val=2048;

	for (i=0;i<=3959;i+=1) {
		hsv.hue=i;
		hsv2rgb(hsv,rgb);
    printf(" H %5d, S %5d, V %5d - R %5d G%5d B%5d\n",hsv.hue,hsv.sat,hsv.val,rgb.r,rgb.g,rgb.b);

		pwmController.setChannelPWM(0, rgb.r);
		pwmController.setChannelPWM(1, rgb.g);
		pwmController.setChannelPWM(2, rgb.b);
    delay(5);
	}
  for (i=3959;i>0;i-=1) {
    hsv.hue=i;
    hsv2rgb(hsv,rgb);
    printf(" H %5d, S %5d, V %5d - R %5d G%5d B%5d\n",hsv.hue,hsv.sat,hsv.val,rgb.r,rgb.g,rgb.b);

    pwmController.setChannelPWM(0, rgb.r);
    pwmController.setChannelPWM(1, rgb.g);
    pwmController.setChannelPWM(2, rgb.b);
    delay(5);
  }


}
