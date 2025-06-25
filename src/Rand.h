#ifndef _Rand
#define _Rand
#include "j_framework.h"

class Rand
{
	public:
		Rand(const Rand &r) { *this = r; }
		Rand(jreal mn=0.0, jreal mx=1.0, jint seed=0) { set(mn, mx, seed); }

		void set(jreal mn=0.0, jreal mx=1.0, jint seed=0)
		{
			_seed = seed;
			_min = mn;
			_max = mx;
			std::srand(_seed);
			_g1 = std::default_random_engine(_seed);
			_g2 = std::mt19937(_seed);
		}

		Rand& operator= (const Rand& b)
		{
			if(this != &b)
			{
				_seed = b._seed;
				_min = b._min;
				_max = b._max;
				_g1 = b._g1;
				_g2 = b._g2;
			}
			return(*this);
		}

		float rndf()
		{
			std::uniform_real_distribution<double> distreal(_min, _max);
			return distreal(_g1);
		}

		int rndi()
		{
			std::uniform_int_distribution<int> dist(_min, _max);
			return dist(_g1);
		}

		// STATIC FUNCS
		// 0..RAND_MAX
		static uint		irand	() { return std::rand(); }
		static float	frand	() { return ((float)std::rand()/(float)RAND_MAX); }
		static double	drand	() { return ((double)std::rand()/(double)RAND_MAX); }

	private:
		jint _seed;
		jreal _min, _max;
		std::default_random_engine _g1;
		std::mt19937 _g2;
};

#endif
