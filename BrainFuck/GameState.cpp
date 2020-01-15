#include "GameState.h"

GameState::GameState()
{
    shaderLib = 0;
    textureLib = 0;
}

GameState::~GameState()
{
    DESTROY(textureLib);
    for (auto i : object)
        DESTROY(i);
}

LONG GameState::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, ShaderLibrary* shaderLib)
{
    this->context = context;
    this->device = device;
    this->shaderLib = shaderLib;
    BLOCKALLOC(TextureClass, textureLib);
    if (textureLib->Initialize(device)) {
        cerr << "failed to setup TextureClass\n";
        return 1;
    }

    NewTextureObject(TEXTURE_FILE);
    // else cerr << "object load success\n";
    return 0;
}

LONG GameState::Frame()
{
    for (auto i : object)
        if (i->Frame()) {
            cerr << "warning: animation failed\n";
        }
    return 0;
}

LONG GameState::Release()
{
    delete this;
    return 0;
}

LONG GameState::Draw()
{
    // Game object render
    for (auto i : object)
        if (i->Render(context, shaderLib->worldMatrix,
                                shaderLib->viewMatrix,
                                shaderLib->projectionMatrix)) {
            cerr << "warning: loading object failed\n";
            return 1;
        }
    // else cerr << "object on\n";

    return 0;
}

TextureObject* GameState::NewTextureObject(const CHAR* filename, TextureObject* target)
{
    if (target == NULL)
        target = new TextureObject;
    if (target == NULL) {
        cerr << "Out of memory";
        return NULL;
    }
    if (target->Initialize(device, filename, textureLib, shaderLib->GetTextureShader())) {
        DESTROY(target);
        return NULL;
    }
    object.insert(target);
    return target;
}
