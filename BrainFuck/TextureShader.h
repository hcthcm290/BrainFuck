#ifndef TextureShader_H
#define TextureShader_H

#include "useful_stuff.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>

static char VSHADER_FUNC_NAME[] = "VShader";
static char PSHADER_FUNC_NAME[] = "PShader";
static char VSHADER_FILE[] = "texture.vs";
static char PSHADER_FILE[] = "texture.ps";

class TextureShader : private NonCopyable
{
    private:
        struct MatrixBufferType
        {
            D3DXMATRIX world;
            D3DXMATRIX view;
            D3DXMATRIX projection;
        };
    public:
        TextureShader();
        ~TextureShader();

        RESULT Initialize(ID3D11Device*, HWND);
        RESULT Release();
        RESULT Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);

    protected:
        RESULT InitShader(ID3D11Device*, HWND, CHAR*, CHAR*);
        void OutputShaderErrorMessage(ID3D10Blob*, HWND, CHAR*);

        RESULT LoadDrawData(ID3D11DeviceContext*, D3DXMATRIX&, D3DXMATRIX&, D3DXMATRIX&, ID3D11ShaderResourceView*);
        void RenderShader(ID3D11DeviceContext*, int);

    private:
        ID3D11PixelShader* pixelShader;
        ID3D11VertexShader* vertexShader;
        ID3D11InputLayout* layout;
        ID3D11Buffer* matrixBuf;
        ID3D11SamplerState* sampleState;
};

#endif // TextureShader_H
