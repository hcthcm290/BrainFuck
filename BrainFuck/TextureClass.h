#ifndef TEXTURECLASS_H
#define TEXTURECLASS_H

#undef __in
#include <map>
#include "useful_stuff.h"
#include <D3D11.h>
#include <D3DX11tex.h>

class TextureClass : NonCopyable
{
    public:
        TextureClass();
        ~TextureClass();

        RESULT Initialize(ID3D11Device*);
        RESULT Release();

        ID3D11ShaderResourceView* GetTexture(const CHAR*);

    private:
        std::map<const CHAR*, ID3D11ShaderResourceView*> texture;
        ID3D11Device* device;
};

#endif // TEXTURECLASS_H
