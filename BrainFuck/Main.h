#ifndef MAIN_H
#define MAIN_H

#include "useful_stuff.h"
#include "Input.h"
#include "Graphic.h"
#include "GameState.h"

class Main : private NonCopyable
{
    public:
        Main();
        ~Main();
        RESULT Initialize();
        RESULT Run();
        RESULT Release();

        LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

    protected:
        void InitializeWindow(int&, int&);
        void RegisterWindow();
        void ShutdownWindows();
        RESULT frame();
    private:
        LPCSTR applicationName;
        HINSTANCE hinstance;
        HWND hwnd;

        Input* input;
        Graphic* graphic;
        GameState* gameState;
};

static Main* ApplicationHandle = 0;

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{return ApplicationHandle->MessageHandler(hwnd, umsg, wparam, lparam);}

#endif // MAIN_H
