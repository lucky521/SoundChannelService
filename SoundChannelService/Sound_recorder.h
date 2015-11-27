#pragma once
#include <Windows.h>
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")

class Sound_recorder
{
public:
	Sound_recorder(void);
	~Sound_recorder(void);
	void run(void);
	void stop(void);
private:
	BOOL isRunning;
	HANDLE eventh;

};

