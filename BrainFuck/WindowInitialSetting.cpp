#include "Main.h"

void Main::InitializeWindow(int& screenWidth, int& screenHeight)
{
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int posX, posY;

	if (FULLSCREEN){
	    DEVMODE dmScreenSettings = {};
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;}
	else{
		screenWidth  = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
                          applicationName,
                          applicationName,
                          WS_OVERLAPPEDWINDOW,//WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
                          posX,
                          posY,
                          screenWidth,
                          screenHeight,
                          NULL,
                          NULL,
                          hinstance,
                          NULL);

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	//ShowCursor(false);
}

void Main::RegisterWindow()
{
    ApplicationHandle = this;
    applicationName = "Game of the hour, not game of the year";
    hinstance = GetModuleHandle(NULL);

    // Window class register
    WNDCLASSEX wc = {};  // zero initialization except first member.

    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);
}

void Main::ShutdownWindows()
{
	ShowCursor(true);

	if(FULLSCREEN)
		ChangeDisplaySettings(NULL, 0);

	DestroyWindow(hwnd);
	hwnd = NULL;

	UnregisterClass(applicationName, hinstance);
	hinstance = NULL;

	ApplicationHandle = NULL;
}

