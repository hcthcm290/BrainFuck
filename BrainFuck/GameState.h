#ifndef GAMESTATE_H
#define GAMESTATE_H

#undef __in
#include <unordered_set>
#include "useful_stuff.h"
#include "TextureObject.h"
#include "ShaderLibrary.h"
#include "TextureClass.h"

class GameState
{
    public:
        virtual RESULT Frame();
        /** Animations, calculations, etc goes here.*/
        TextureObject* NewTextureObject(const CHAR* filename, TextureObject* target = NULL);
        /** Creates a new TextureObject and add it to list of objects to render.
            You may provide a target and it will initialize that object for you
            (Used for initializing an object inheriting from TextureObject.*/
    public:
        GameState();
        ~GameState();
        RESULT Initialize(ID3D11Device*, ID3D11DeviceContext*, ShaderLibrary*);
        RESULT Release();
        RESULT Draw();
    protected:
        std::unordered_set<TextureObject*> object;
    private:
        TextureClass* textureLib;
        ShaderLibrary* shaderLib;
        ID3D11Device* device;
        ID3D11DeviceContext* context;
};

#endif // GAMESTATE_H
