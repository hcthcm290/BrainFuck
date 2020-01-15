#ifndef INGAME_3D_TEXTURE_OBJECT_H
#define INGAME_3D_TEXTURE_OBJECT_H

#include "useful_stuff.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include "TextureClass.h"
#include "TextureShader.h"

static const CHAR* TEXTURE_FILE = "texture.dds";


class TextureObject
{
    struct VertexType
    {
        D3DXVECTOR3 position;
        D3DXVECTOR2 texture;
    };
    public:
        TextureObject();
        TextureObject(const TextureObject&);
        TextureObject& operator = (const TextureObject&);
        ~TextureObject();

        RESULT Initialize(ID3D11Device*, const CHAR*, TextureClass*, TextureShader*);
        RESULT Release();
        RESULT Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

        RESULT InitializeData();
        RESULT Frame();

        int GetPointCount();
        ID3D11ShaderResourceView* GetTexture();
        VertexType& operator[](int);
    protected:
        RESULT Setup(ID3D11Device*);

        int pointCount;
        VertexType* pointArray;

        ID3D11ShaderResourceView* texture;
    private:
        ID3D11Buffer *vertexBuf;

        ID3D11Device* device;
        CHAR* imageSrc;
        TextureShader* shader;
        //RefreshRate refreshRate;
};

#endif // INGAME_3D_TEXTURE_OBJECT_H
