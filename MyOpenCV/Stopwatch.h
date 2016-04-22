#pragma once
#include <stdafx.h>
#include <Windows.h>

class Stopwatch
{
private:

	LARGE_INTEGER  nFreq;

	LARGE_INTEGER nBeginTime;

	LARGE_INTEGER nEndTime;

	unsigned long time;

public:

	void Start();

	double Stop();

	~Stopwatch();

	Stopwatch();
};

