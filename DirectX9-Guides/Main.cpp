#include<windows.h>
#include "stdafx.h"


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	//register the window class
	const wchar_t CLASS_NAME[] = L"DirectX9 Guides Window Class";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	//create the window
	HWND hwnd = CreateWindowEx
	(
		0,
		CLASS_NAME,
		L"Learn to program windows",
		WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		0,0,600,800,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	//run the message loop
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
		return 0;
	}
	default:
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}
