#ifndef D3DCONTROLLER_H
#define D3DCONTROLLER_H

#include "useful_stuff.h"
#include <DXGI.h>
#include <D3Dcommon.h>
#include <D3D11.h>
#include <D3DX10math.h>

typedef DXGI_SAMPLE_DESC MultisampleSetting;

const D3D11_DEPTH_STENCILOP_DESC FRONTFACE =
{
    D3D11_STENCIL_OP_KEEP,
    D3D11_STENCIL_OP_INCR,
    D3D11_STENCIL_OP_KEEP,
    D3D11_COMPARISON_ALWAYS
};
const D3D11_DEPTH_STENCILOP_DESC BACKFACE =
{
    D3D11_STENCIL_OP_KEEP,
    D3D11_STENCIL_OP_DECR,
    D3D11_STENCIL_OP_KEEP,
    D3D11_COMPARISON_ALWAYS
};

struct D3Dcontroller_setting
{
    int screenWidth;
    int screenHeight;
    bool vsyncEnable;
    HWND hwnd;
    bool fullScreen;
    MultisampleSetting msaa;
    float screenDepth;
    float screenNear;
};

class D3Dcontroller
{
    public:
        D3Dcontroller();
        ~D3Dcontroller();
        RESULT Initialize(D3Dcontroller_setting&);
        RESULT Release();

        RESULT draw(float red = 0.0f, float green = 0.2f, float blue = 0.4f, float alpha = 1.0f);
        RESULT display();

        // Write-protected member
        ID3D11Device* GetDevice();
        ID3D11DeviceContext* GetDeviceContext();
        RefreshRate GetRefreshRate();

        // Unprotected member
        D3DXMATRIX projectionMatrix;
        D3DXMATRIX worldMatrix;
        D3DXMATRIX orthoMatrix;
        int videoCardMemory;
        char videoCardDescription[128];
    protected:
        RESULT GetVideoCardInfo(int, int);
        RESULT CreateSwapChain(HWND, int, int, bool, MultisampleSetting&);
        RESULT CreateDepthBuffer(int, int, MultisampleSetting&);
        RESULT CreateRasterState(int, int);
        RESULT CreateMatrix(int, int, float, float);
    private:
        bool vsyncEnable;

        IDXGISwapChain* swapchain;
        ID3D11Device* device;
        ID3D11DeviceContext* deviceContext;
        ID3D11RenderTargetView* renderTargetView;
        ID3D11Texture2D* depthStencilBuffer;
        ID3D11DepthStencilState* depthStencilState;
        ID3D11DepthStencilView* depthStencilView;
        ID3D11RasterizerState* rasterState;
        RefreshRate refreshRate;
};

#endif // D3DCONTROLLER_H
