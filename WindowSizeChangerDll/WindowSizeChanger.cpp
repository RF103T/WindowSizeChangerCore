#include "WindowSizeChanger.h"

WindowSizeChanger::WindowSizeChanger()
{

}

WindowSizeChanger::~WindowSizeChanger()
{

}

float WindowSizeChanger::GetTitleBarThreshold()
{
	return titleBarThreshold;
}

void WindowSizeChanger::SetTitleBarThreshold(float threshold)
{
	titleBarThreshold = threshold;
}

long WindowSizeChanger::GetBorderThreshold()
{
	return (long)borderThreshold;
}

void WindowSizeChanger::SetBorderThreshold(long threshold)
{
	borderThreshold = (LONG)threshold;
}

bool WindowSizeChanger::isRunning()
{
	return !isExit;
}

void WindowSizeChanger::Stop()
{
	isExit = true;
}

void WindowSizeChanger::Run()
{
	isExit = false;

	::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int fullWindowHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	float rateX = 65536.0f / screenWidth, rateY = 65536.0f / screenHeight;

	TCHAR className[200] = L"";
	POINT cursorPos;
	auto info = std::make_shared<WINDOWINFO>();
	info->cbSize = sizeof(WINDOWINFO);

	while (!isExit)
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x01)
		{
			if (HWND foregroundWindow = GetForegroundWindow())
			{
				if (GetWindowInfo(foregroundWindow, info.get()))
				{
					GetClassName(foregroundWindow, className, 200);
					int guide = !_tcscmp(className, GUIDE_WINDOW_CLASS_NAME), share = !_tcscmp(className, SCREEN_SHARE_WINDOW_CLASS_NAME);
					if (guide || share)
					{
						if (GetCursorPos(&cursorPos))
						{
							LONG windowX = info->rcWindow.left, windowY = info->rcWindow.top, windowWidth = info->rcClient.right - info->rcClient.left, windowHeight = info->rcClient.bottom - info->rcClient.top;
							if ((cursorPos.x >= windowX && cursorPos.x <= windowX + borderThreshold) ||
								(cursorPos.y >= windowY && cursorPos.y <= windowY + borderThreshold) ||
								(cursorPos.x >= windowX + windowWidth - borderThreshold && cursorPos.x <= windowX + windowWidth) ||
								(cursorPos.y >= windowY + windowHeight - borderThreshold && cursorPos.y <= windowY + windowHeight))
							{
								POINT cursorTempPos = cursorPos;
								while (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
								{
									GetCursorPos(&cursorPos);
									SetWindowPos(foregroundWindow, NULL, windowX, windowY, windowWidth + cursorPos.x - cursorTempPos.x, windowHeight + cursorPos.y - cursorTempPos.y, SWP_NOMOVE | SWP_NOZORDER);
								}
								if (share)
								{
									mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN, (INT32)((windowX + (windowWidth * titleBarThreshold)) * rateX), (INT32)((windowY + 30) * rateY), 0, NULL);
									mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP, (INT32)((windowX + (windowWidth * titleBarThreshold)) * rateX), (INT32)((windowY + 30) * rateY), 0, NULL);
									mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN, (INT32)((windowX + (windowWidth * titleBarThreshold)) * rateX), (INT32)((windowY + 30) * rateY), 0, NULL);
									mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP, (INT32)((windowX + (windowWidth * titleBarThreshold)) * rateX), (INT32)((windowY + 30) * rateY), 0, NULL);

									Sleep(10);
									float zoomRate = fullWindowHeight / (float)windowHeight;
									int windowZoomWidth = zoomRate * windowWidth;

									mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN, (INT32)((windowX + (windowZoomWidth * titleBarThreshold)) * rateX), (INT32)(30 * rateY), 0, NULL);
									mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP, (INT32)((windowX + (windowZoomWidth * titleBarThreshold)) * rateX), (INT32)(30 * rateY), 0, NULL);
									mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN, (INT32)((windowX + (windowZoomWidth * titleBarThreshold)) * rateX), (INT32)(30 * rateY), 0, NULL);
									mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP, (INT32)((windowX + (windowZoomWidth * titleBarThreshold)) * rateX), (INT32)(30 * rateY), 0, NULL);

									mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, (INT32)(cursorPos.x * rateX), (INT32)(cursorPos.y * rateY), 0, NULL);
								}
							}
						}
					}
				}
			}
		}
		Sleep(100);
	}
}