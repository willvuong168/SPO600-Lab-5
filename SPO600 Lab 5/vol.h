/* This is the number of samples to be processed */
#define SAMPLES 9999

/* This is the volume scaling factor to be used */
#define VOLUME 50.0 // Percent of original volume

#include <stdlib.h>
#include <stdint.h>

/* Function prototype to fill an array sample of
 * length sample_count with random int16_t numbers
 * to simulate an audio buffer */
void vol_createsample(int16_t* sample, int32_t sample_count)
{
	int i;
	for (i=0; i<sample_count; i++) {
		sample[i] = (rand()%65536)-32768;
	}
	return;
};