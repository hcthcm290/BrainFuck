#ifndef SHADERLIBRARY_H
#define SHADERLIBRARY_H

#include "TextureShader.h"

class ShaderLibrary
{
    public:
        ShaderLibrary();
        ~ShaderLibrary();

        RESULT Initialize(ID3D11Device*, HWND);
        RESULT Release();

        TextureShader* GetTextureShader();
        D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
    protected:

    private:
        TextureShader* texShader;
};

#endif // SHADERLIBRARY_H
