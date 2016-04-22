#include <stdafx.h>
#include "Stopwatch.h"


Stopwatch::Stopwatch()
{
	QueryPerformanceFrequency(&nFreq);

}


void Stopwatch::Start()
{
	if (nFreq.QuadPart == NULL)
	{
		QueryPerformanceFrequency(&nFreq);
	}

	QueryPerformanceCounter(&nBeginTime);

}

double Stopwatch::Stop()
{
	if (nBeginTime.QuadPart == 0)
	{
		return 0;
	}

	QueryPerformanceCounter(&nEndTime);

	auto time = (nEndTime.QuadPart - nBeginTime.QuadPart)*1000.0 / nFreq.QuadPart;

	

	nBeginTime.QuadPart = 0;

	return time;



}

Stopwatch::~Stopwatch()
{


}
