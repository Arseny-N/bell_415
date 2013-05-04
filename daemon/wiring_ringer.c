#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define DEALY_OK(d) (d > 0)

int main (int argc, char *argv[])
{
	if(!argv[1])
		exit(EXIT_FAILURE);
	
	int d = atoi(argv[1]);
	
	if(!DELAY_OK(d)) {
		//fprintf(stderr,"delay errors %d",d);
		exit (EXIT_FAILURE);
	}
	if (wiringPiSetup () == -1) {
		//perror("wirionPiSetup");
		exit (EXIT_FAILURE);
	}
	int pin = pin_init();
	if( pin == -1 ) {
		//perror("pin_init");
		exit (EXIT_FAILURE);
	}
	
	
	digitalWrite(pin, HIGH);

	delay (d * 1000);
	
	digitalWrite(pin, LOW);

	return 0 ;
}
