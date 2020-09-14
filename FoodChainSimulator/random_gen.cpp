#include "random_gen.h"
#include <time.h>
#include <random>
#include "utils.h"
namespace EcoSim
{
	std::mt19937 random;
	int seed = -1;

	void ApplySeed()
	{
		if (seed == -1)
		{
			random = std::mt19937((int)time(0));
		}
		else {
			random = std::mt19937(seed); 
		}
	} 

	int Rand()
	{
		return random();
	}

	std::mt19937& Randomizer()
	{
		return random;
	}

}