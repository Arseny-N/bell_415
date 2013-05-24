#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define DEALY_OK(d) (d > 0)

#define O_PIN 7

int main( int argc, char *argv[])
{
	if(!argv[1])
		exit(EXIT_FAILURE);
	
	int d = atoi(argv[1]);
	
	if(!DELAY_OK(d)) {
		fprintf(stderr,"delay error %d",d);
		exit (EXIT_FAILURE);
	}
	if (wiringPiSetup () == -1) {
		perror("wirionPiSetup");
		exit (EXIT_FAILURE);
	}
	
	pinMode(O_PIN, OUTPUT);	
	
	digitalWrite(O_PIN, HIGH);
	
	delay (d * 1000);
	
	digitalWrite(O_PIN, LOW);

	return 0;
}
