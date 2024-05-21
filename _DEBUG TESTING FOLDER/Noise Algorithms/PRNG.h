#ifndef PRNG_H
#define PRNG_H

#include <stdio.h>
#include <stdint.h>

#define MT19937_N 624
#define MT19937_M 397
#define MT19937_MATRIX_A 0x9908b0dfUL
#define MT19937_UPPER_MASK 0x80000000UL
#define MT19937_LOWER_MASK 0x7fffffffUL

namespace random {
	static uint32_t mt[MT19937_N];
	static int mt_index = MT19937_N + 1;

	void setSeed(uint32_t seed) {
		mt[0] = seed;
		for (int i = 1; i < MT19937_N; i++) {
			mt[i] = (1812433253UL * (mt[i-1] ^ (mt[i-1] >> 30)) + i);
		}
		mt_index = MT19937_N;
	}

	static void _twist() {
		for (int i = 0; i < MT19937_N; i++) {
			uint32_t x = (mt[i] & MT19937_UPPER_MASK) + (mt[(i+1) % MT19937_N] & MT19937_LOWER_MASK);
			uint32_t xA = x >> 1;
			if (x % 2 != 0) {
				xA ^= MT19937_MATRIX_A;
			}
			mt[i] = mt[(i + MT19937_M) % MT19937_N] ^ xA;
		}
		mt_index = 0;
	}

	uint32_t get() {
		if (mt_index >= MT19937_N) {
			if (mt_index == MT19937_N + 1) {
				setSeed(5489UL);  // A default initial seed is used
			}
			_twist();
		}
		
		uint32_t y = mt[mt_index++];
		y ^= (y >> 11);
		y ^= (y << 7) & 0x9d2c5680UL;
		y ^= (y << 15) & 0xefc60000UL;
		y ^= (y >> 18);
		
		return y;
	}
}


#endif