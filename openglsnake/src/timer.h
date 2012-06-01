#pragma once

#include <Windows.h>
/*

	Windows timer based on winapi functions 
		QueryPerformanceCounter
		QueryPerformanceFrequency

	twinlarge - LARGE_INTEGER alias. This class uses twinlarge.QuadPart field to store values;
	start() - starts the timer. returns initial number of the ticks passed after the program start;
	getTime() - returns the number of ticks passed after the start() method call. 
		returned value is LARGE_LONG windows structure;
	getTimeSec()- returns the time in seconds passed after the start() method call.
	getTicksRate() - returns number of ticks per second in a LARGE_INTEGER structure;
	reset() - reset the time passed after  the start() method call;

*/


typedef LARGE_INTEGER twinlarge;

class Timer
{
private:
	twinlarge tick1, tick2;
	twinlarge tickPerSecond;

public:

	Timer(void)
	{
		tick1.QuadPart = 0;
		tick2.QuadPart = 0;
	}
	~Timer(void){};

	twinlarge getTime()
	{
		QueryPerformanceCounter(&tick2);
		tick2.QuadPart -= tick1.QuadPart;
		return tick2;
	}

	float getTimeSec()
	{
		twinlarge ticks;
		ticks =	getTime();
		return (float)ticks.QuadPart/tickPerSecond.QuadPart;
	}

	twinlarge getTicksRate()
	{
		return tickPerSecond;
	}

	void reset()
	{
		tick1.QuadPart = 0;
		tick2.QuadPart = 0;
	}

	twinlarge start()
	{
		QueryPerformanceFrequency(&tickPerSecond);
		QueryPerformanceCounter(&tick1);
		return tick1;
	}

};

