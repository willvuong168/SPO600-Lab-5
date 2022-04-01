#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "vol.h"
#include <iostream>
using namespace std;


int16_t scale_sample0(int16_t sample, int volume)
{
	return static_cast<int16_t>((float)(volume / 100.0) * (float)sample);
}

int16_t scale_sample1(int16_t sample, int volume)
{
	return ((static_cast<int32_t>(sample) * (32767 * volume / 100 << 1)) >> 16);
}


int main()
{
	Timer t;
	int x;
	int ttl = 0;
	int16_t* in;
	int16_t* out0;
	int16_t* out1;
	int16_t* out2;
	double t0 = 0;
	double t1 = 0;
	double t2 = 0;
	in = static_cast<int16_t*>(calloc(SAMPLES, sizeof(int16_t)));
	out0 = static_cast<int16_t*>(calloc(SAMPLES, sizeof(int16_t)));
	out1 = static_cast<int16_t*>(calloc(SAMPLES, sizeof(int16_t)));
	out2 = static_cast<int16_t*>(calloc(SAMPLES, sizeof(int16_t)));

	for (int i = 0; i < 10000; i++)
	{
		vol_createsample(in, SAMPLES);
		// Test 1 (vol0.c)
		t.start();
		for (x = 0; x < SAMPLES; x++)
		{
			out0[x] = scale_sample0(in[x], VOLUME);
		}
		t.stop();
		t0 += t.currtime();
		//cout << "vol0.c took " << t.currtime() << " s" << endl;
		for (x = 0; x < SAMPLES; x++)
		{
			ttl = (ttl + out0[x]) % 1000;
		}
		//printf("Result: %d\n", ttl);

		// Test 2 (vol1.c)
		t.start();
		for (x = 0; x < SAMPLES; x++)
		{
			out1[x] = scale_sample1(in[x], VOLUME);
		}
		t.stop();
		t1 += t.currtime();
		//cout << "vol1.c took " << t.currtime() << " s" << endl;
		for (x = 0; x < SAMPLES; x++)
		{
			ttl = (ttl + out1[x]) % 1000;
		}
		//printf("Result: %d\n", ttl);

		// Test 3 (vol2.c)
		static int16_t* precalc;
		precalc = static_cast<int16_t*>(calloc(65536, 2));
		if (precalc == nullptr)
		{
			printf("malloc failed!\n");
			return 1;
		}
		t.start();
		for (x = -32768; x <= 32767; x++)
		{
			precalc[static_cast<uint16_t>(x)] = static_cast<int16_t>((float)x * VOLUME / 100.0);
		}
		for (x = 0; x < SAMPLES; x++)
		{
			out2[x] = precalc[static_cast<uint16_t>(in[x])];
		}
		t.stop();
		t2 += t.currtime();
		//cout << "vol2.c took " << t.currtime() << " s" << endl;
		for (x = 0; x < SAMPLES; x++)
		{
			ttl = (ttl + out2[x]) % 1000;
		}
		//printf("Result: %d\n", ttl);
	}
	cout << "vol0.c took " << t0/10000 << " s average" << endl;
	cout << "vol1.c took " << t1/10000 << " s average" << endl;
	cout << "vol2.c took " << t2/10000 << " s average" << endl;
	return 0;
}
