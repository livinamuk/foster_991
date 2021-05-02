
#include "Common.h"

void Extension::SetRandomSeed(int Seed)
{
	srand(Seed);
}

void Extension::SetRandomSeedToTimer()
{
#ifdef _WIN32
	srand(GetTickCount());
#else
	srand(time(NULL));
#endif
}





