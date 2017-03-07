/*
 Name:		SwitchWithDelayLib.h
 Created:	3/7/2017 10:58:53 PM
 Author:	RevilS
 Editor:	http://www.visualmicro.com
*/

#ifndef _SwitchWithDelayLib_h
#define _SwitchWithDelayLib_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"

#else
	#include "WProgram.h"
#endif

class SwitchWithDelay
{
public:
	SwitchWithDelay(int pin,
		int rfGoUpCode,
		int rfGoDownCode,
		int rfGoUpDuration,
		int rfGoDownDuration);
	void resetGoUpUpdateTime();
	void resetGoDownUpdateTime();
	void handleCode(int inputCode);
private:
	int _pin;
	// RfCommands
	int _rfGoUpCode; // = 1500;
	int _rfGoDownCode; // = 1600;

	// RfCommands Duration
	int _rfGoUpDuration; // = 1500;
	int _rfGoDownDuration; // = 1600;

	// RfCommands UpdateTime
	unsigned long _rfUpNextUpdateTime; // = 0;
	unsigned long _rfDownNextUpdateTime; // = 0;
};
#endif

