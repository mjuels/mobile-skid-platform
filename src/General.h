// General.h

#ifndef _GENERAL_h
#define _GENERAL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

/*! A class containing several general functions
*/
class General {
public:
    static float saturate(float in, float min, float max); ///< Saturate float between minimum and maximum
    static void SetupTimer0(int Freq);
    static void DisableTimer0(void);
};


#endif
