#include "Main.h"

LRESULT Main::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch(umsg)
	{
		case WM_KEYDOWN:
			return input->keydown(wparam, lparam);

		case WM_KEYUP:
			return input->keyup(wparam, lparam);

        case WM_MOUSEMOVE:
            return input->mousemove(wparam, lparam);

		case WM_DESTROY:{
			PostQuitMessage(0);
			return 0;}

		case WM_CLOSE:{
            PostQuitMessage(0);
            return 0;}

		default:
			return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

Main::Main()
{ input = NULL; graphic = NULL; gameState = NULL;}

Main::~Main()
{ DESTROY(input)
  DESTROY(graphic)
  DESTROY(gameState)}

RESULT Main::Initialize()
{
    int screenWidth = 0;
    int screenHeight = 0;
    RegisterWindow();
    InitializeWindow(screenWidth, screenHeight);

    if (!(input = new Input)) {
        cerr << "Out of memory";
        return 1;}
    if (input->Initialize()) {
        cerr << "*** Input ***\n";
        return 1;}

    if (!(graphic = new Graphic)) {
        cerr << "Out of memory\n";
        return 1;}
    if (graphic->Initialize(screenWidth, screenHeight, hwnd)) {
        cerr << "*** Graphic ***\n";
        return 1;}

    BLOCKALLOC(GameState, gameState);
    if (gameState->Initialize(graphic->GetDevice(), graphic->GetDeviceContext(), graphic->GetShaderLibrary())) {
        cerr << "*** GameState ***\n";
        return 1;}

    return 0;
}

RESULT Main::Release()
{
    if (graphic->Release()){
        cerr << "Graphic release failed\n";
        delete this;
        return 1;}
    graphic = NULL;

    if (input->Release()){
        cerr << "Input release failed\n";
        delete this;
        return 1;}
    input = NULL;

    ShutdownWindows();

    delete this;
    return 0;
}

RESULT Main::Run()
{
    MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	while(true)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
            if(msg.message == WM_QUIT)
                break;
		}
		else
			if (frame())
                return 1;
	}

    return 0;
}

RESULT Main::frame()
{
    if (gameState->Frame()) {
        cerr << "FRAME CALCULATION FAILED";
        return 1;
    }

    if (graphic->DrawSetup()) return 1;

    if (gameState->Draw()) {
        cerr << "OBJECT DRAW FAILED";
        return 1;
    }

    if (graphic->draw()){
        cerr << "RENDER FAILED\n";
        return 1;
    }
    // else cerr << "FRAME COMPLETE\n";
    return 0;
}
