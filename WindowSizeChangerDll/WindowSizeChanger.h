#pragma once
#include <iostream>
#include <tchar.h>
#include <stdlib.h>
#include <Windows.h>
#include <WinUser.h>

public ref class WindowSizeChanger
{
private:
	float titleBarThreshold = 0.15f;
	LONG borderThreshold = 3;
	bool isExit = false;

public:
	const TCHAR* GUIDE_WINDOW_CLASS_NAME = L"ScreenShareGuideWindow";
	const TCHAR* SCREEN_SHARE_WINDOW_CLASS_NAME = L"kMiScreenShareMainWindow";

	WindowSizeChanger();
	~WindowSizeChanger();
	float GetTitleBarThreshold();
	void SetTitleBarThreshold(float threshold);
	long GetBorderThreshold();
	void SetBorderThreshold(long threshold);
	bool isRunning();
	void Stop();
	void Run();
};