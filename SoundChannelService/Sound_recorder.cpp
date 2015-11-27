#include "Sound_recorder.h"
#define s2
#define RATE 1 //8000

Sound_recorder::Sound_recorder(void)
{
	isRunning = TRUE;
}


Sound_recorder::~Sound_recorder(void)
{
}


void Sound_recorder::run(void)
{
		const int NUMPTS = RATE * 10;
		int sampleRate = RATE;
		short int waveIn[NUMPTS];

	#ifdef s2
		eventh = CreateEvent( 
			NULL,               // default security attributes
			FALSE,               // 
			FALSE,              // initial state is nonsignaled
			TEXT("sound_event")  // object name
			);
	#endif

		HWAVEIN hWaveIn;
		WAVEHDR WaveInHdr;
		MMRESULT result;

		WAVEFORMATEX pFormat;
		pFormat.wFormatTag = WAVE_FORMAT_PCM;
		pFormat.nChannels = 1;  // 1: mono, 2, stereo
		pFormat.nSamplesPerSec = sampleRate;  //11025,22050,44100
		pFormat.nAvgBytesPerSec = 2 * sampleRate;
		pFormat.nBlockAlign = 2;
		pFormat.wBitsPerSample = 16;
		pFormat.cbSize = 0;

	#ifdef s1
		 result = waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat, 0, 0, WAVE_FORMAT_DIRECT);
	#endif
	#ifdef s2
		result = waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat, (DWORD)eventh, 0, CALLBACK_EVENT);
	#endif
		if(result)
		{
			char fault[256];
			waveInGetErrorTextA(result, fault, 256);
			MessageBoxA(NULL, fault, "Failed to open waveform input device.", MB_OK | MB_ICONEXCLAMATION);
			//WriteEventLogEntry(fault, EVENTLOG_ERROR_TYPE);
			return;
		}

		WaveInHdr.lpData = (LPSTR)waveIn;
		WaveInHdr.dwBufferLength = 2 * NUMPTS;
		WaveInHdr.dwBytesRecorded = 0;
		WaveInHdr.dwUser = 0;
		WaveInHdr.dwFlags = 0;
		WaveInHdr.dwLoops = 0;

		waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

		result = waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
		if(result)
		{
			MessageBoxA(NULL, "Failed to read block from device", NULL, MB_OK | MB_ICONEXCLAMATION);
			//WriteEventLogEntry(L"Failed to read block from device", EVENTLOG_ERROR_TYPE);
			return;
		}

		result = waveInStart(hWaveIn);
		if(result)
		{
			MessageBoxA(NULL, "Failed to start recording", NULL, MB_OK | MB_ICONEXCLAMATION);
			//WriteEventLogEntry(L"Failed to start recording", EVENTLOG_ERROR_TYPE);
			return;
		}

		//cout << "Recording..." << endl;
		//WriteEventLogEntry(L"Recording...", EVENTLOG_ERROR_TYPE);
		//Sleep(1000*60);


	#ifdef s1
		// Solution 1:  loop check, bad method
		do {
		} while(waveInUnprepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);
		while(1)
		{
				result = waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
				result = waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
				do {
				} while(waveInUnprepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);
			
		}
	#endif

	#ifdef s2
		// Solution 2:  wait for event, good method
		ResetEvent(eventh);
		isRunning = TRUE;
		do
		{
			WaitForSingleObject(eventh, INFINITE);
			do {
			} while((waveInUnprepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING) && (isRunning));
			result = waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
			result = waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
			ResetEvent(eventh);
		} while(isRunning);
	#endif

	#ifdef s3
		// Solution 3: better method
	#endif

		//cout << "Close..." << endl;
		//WriteEventLogEntry(L"Close...", EVENTLOG_ERROR_TYPE);
		waveInClose(hWaveIn);
		return;
}


void Sound_recorder::stop(void)
{
	isRunning = FALSE;
	SetEvent(eventh);
}
