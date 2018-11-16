
// PCA9685-Arduino Simple Example

#include <Wire.h>
#include "PCA9685.h"

#define NUMLEDS 12
#include "table.h"

PCA9685 pwmController;                  // Library using default Wire and default linear phase balancing scheme

int leds[NUMLEDS];

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

		for  ( int i=0;i<NUMLEDS;i++) {
    	pwmController.setChannelPWM(i, 0);
		}
		delay(1000);
		for  ( int i=0;i<NUMLEDS;i++) {
    	pwmController.setChannelPWM(i, 1024);
			delay(50);
		}
		delay(1000);
		for  ( int i=0;i<NUMLEDS;i++) {
    	pwmController.setChannelPWM(i, 2048);
			delay(50);
		}
		delay(1000);
		for  ( int i=0;i<NUMLEDS;i++) {
    	pwmController.setChannelPWM(i, 0);
			leds[i]=0;
			delay(50);
		}

}


void loop() 
{

	int i,led;
	const int TabCnt = sizeof(table) / sizeof(table[0]);

	printf("light up \n");

	for (led=0;led<NUMLEDS;led++)	{
		printf("l:%2.2d\r",led);
		for(i=0;i<=TabCnt/8*6;i++) {
			leds[led]=pgm_read_word_near(table+i);
			pwmController.setChannelPWM(led, leds[led]);
		}
	}

	for(i=TabCnt/8*6;i<TabCnt/8*7;i++) {
		for (led=0;led<NUMLEDS;led++)	{
			printf("l:%2.2d\r",led);
			leds[led]=pgm_read_word_near(table+i);
			pwmController.setChannelPWM(led, leds[led]);
		}
	}
	delay(5000);
	printf("dimming up\n");

	for (led=0;led<NUMLEDS;led++)	{
		printf("l:%2.2d\r",led);
		for(i=TabCnt/8*7;i>=100;i--) {
			leds[led]=pgm_read_word_near(table+i);
			pwmController.setChannelPWM(led, leds[led]);
			delay(2);
		}
		pwmController.setChannelPWM(led, 0);
	}


	delay(5000);
	printf("estinguish\n");
	for (led=0;led<NUMLEDS;led++)	{
		pwmController.setChannelPWM(led,0);
	}
}
