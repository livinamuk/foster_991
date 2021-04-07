// DarkEdif extension: allows safe multithreading returns.
#include "Common.h"

#if defined(MULTI_THREADING) && !defined(ALREADY_DEFINED_MT)
#define ALREADY_DEFINED_MT
	/* Make sure any pointers in ExtVariables are free'd in ~SaveExtInfo(). */
	#pragma pack (push, align_to_one_multithreading)
	#pragma pack (1) 
	struct SaveExtInfo
	{
		// Required for DarkEdif
		unsigned char	numEvents;
		unsigned short	condTrig[2];

		SaveExtInfo() : numEvents(0), condTrig{ 0, 0 }
		{
		}
		~SaveExtInfo()
		{
		}
	};
#pragma pack (pop, align_to_one_multithreading)
#endif // MULTI_THREADING