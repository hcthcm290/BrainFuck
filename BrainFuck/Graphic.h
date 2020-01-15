#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "useful_stuff.h"
#include "D3Dcontroller.h"
#include "TextureObject.h"
#include "ShaderLibrary.h"
#include "CameraClass.h"

const bool FULLSCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const DXGI_SAMPLE_DESC MSAA_SETTING =
{
    1,          // Number of multisample per pixel
    0           // Image quality level
}; // MULTISAMPLING off

class Graphic : private NonCopyable
{
    public:
        Graphic();
        ~Graphic();

        RESULT Initialize(int, int, HWND);
        RESULT Release();

        RESULT draw();
        RESULT DrawSetup();

        inline ID3D11Device* GetDevice() {return controller->GetDevice();}
        inline ID3D11DeviceContext* GetDeviceContext() {return controller->GetDeviceContext();}
        inline ShaderLibrary* GetShaderLibrary() {return shaderLib;}
    protected:
    private:
        D3Dcontroller* controller;
        CameraClass* camera;
        ShaderLibrary* shaderLib;

        int width, height;
        float color[4];
};

#endif // GRAPHIC_H
