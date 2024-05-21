#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include "PRNG.h"

#include <stdio.h>
#include <stdint.h>
#include <math.h>

namespace Noise {
	namespace Perlin {
		// Permutation table
		static int perm[512];
		
		void setSeed(uint32_t seed) {
			random::setSeed(seed);
			for (int i = 0; i < 256; i++) {
				perm[i] = i;
			}
			for (int i = 0; i < 256; i++) {
				int j = random::get() % 256;
				int temp = perm[i];
				perm[i] = perm[j];
				perm[j] = temp;
			}
			for (int i = 0; i < 256; i++) {
				perm[256 + i] = perm[i];
			}
		}

		static inline double fade(double t) {
			return t * t * t * (t * (t * 6 - 15) + 10);
		}

		static inline double lerp(double t, double a, double b) {
			return a + t * (b - a);
		}

		static inline double grad(int hash, double x, double y, double z) {
			int h = hash & 15;
			double u = h < 8 ? x : y;
			double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);

			double gradValue = ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
			
			return gradValue;
		}


		double perlin(double x, double y, double z) {
			int X = (int)floor(x) & 255;
			int Y = (int)floor(y) & 255;
			int Z = (int)floor(z) & 255;

			x -= floor(x);
			y -= floor(y);
			z -= floor(z);

			double u = fade(x);
			double v = fade(y);
			double w = fade(z);

			int A = perm[X] + Y;
			int AA = perm[A] + Z;
			int AB = perm[A + 1] + Z;
			int B = perm[X + 1] + Y;
			int BA = perm[B] + Z;
			int BB = perm[B + 1] + Z;

			double gradAA = grad(perm[AA], x, y, z);
			double gradBA = grad(perm[BA], x-1, y, z);
			double gradAB = grad(perm[AB], x, y-1, z);
			double gradBB = grad(perm[BB], x-1, y-1, z);
			double gradAA1 = grad(perm[AA+1], x, y, z-1);
			double gradBA1 = grad(perm[BA+1], x-1, y, z-1);
			double gradAB1 = grad(perm[AB+1], x, y-1, z-1);
			double gradBB1 = grad(perm[BB+1], x-1, y-1, z-1);

			double lerpU1 = lerp(u, gradAA, gradBA);
			double lerpU2 = lerp(u, gradAB, gradBB);
			double lerpV1 = lerp(v, lerpU1, lerpU2);

			double lerpU3 = lerp(u, gradAA1, gradBA1);
			double lerpU4 = lerp(u, gradAB1, gradBB1);
			double lerpV2 = lerp(v, lerpU3, lerpU4);

			double res = lerp(w, lerpV1, lerpV2);

			return res;
		}
	}
}

#endif
