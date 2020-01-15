#include "ShaderLibrary.h"

ShaderLibrary::ShaderLibrary()
{
    texShader = 0;
}

ShaderLibrary::~ShaderLibrary()
{
    DESTROY(texShader);
}

LONG ShaderLibrary::Initialize(ID3D11Device* device, HWND hwnd)
{
    BLOCKALLOC(TextureShader, texShader);
    CALL(texShader->Initialize(device, hwnd),"failed to load texture shader\n");
    return 0;
}

LONG ShaderLibrary::Release()
{
    delete this;
    return 0;
}

TextureShader* ShaderLibrary::GetTextureShader()
{
    return texShader;
}
