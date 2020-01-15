#include "TextureObject.h"

TextureObject::TextureObject() {
    vertexBuf = 0;
    device = 0;
    pointArray = 0;
    texture = 0;
    shader = 0;
}
TextureObject::~TextureObject() {
    DESTROY(vertexBuf);
    delete[] pointArray;
    // DO NOT DESTROY DEVICE, TEXTURE
}
TextureObject::TextureObject(const TextureObject& object) {
    vertexBuf = 0;
    texture = object.texture;
    shader = object.shader;

    pointCount = object.pointCount;

    pointArray = new VertexType[pointCount];
    memcpy(pointArray, object.pointArray, sizeof(VertexType) * pointCount);

    Setup(object.device);
}
TextureObject& TextureObject::operator = (const TextureObject& object) {
    DESTROY(vertexBuf);
    delete[] pointArray;
    // DO NOT DESTROY DEVICE

    pointCount = object.pointCount;

    pointArray = new VertexType[pointCount];
    memcpy(pointArray, object.pointArray, sizeof(VertexType) * pointCount);

    Setup(object.device);
    texture = object.texture;
    shader = object.shader;
    return *this;
}
RESULT TextureObject::Setup(ID3D11Device* device)
{
    this->device = device;

    D3D11_BUFFER_DESC vertexBufDesc;
    D3D11_SUBRESOURCE_DATA vertexData;

    memset(&vertexBufDesc,0, sizeof(D3D11_BUFFER_DESC));
    memset(&vertexData,0, sizeof(D3D11_SUBRESOURCE_DATA));
    // Inefficiency here

    vertexBufDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufDesc.ByteWidth = sizeof(VertexType) * pointCount;
    vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    vertexData.pSysMem = pointArray;

    BLOCKCALL(device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuf));
    return 0;
}
RESULT TextureObject::Initialize(ID3D11Device* device, const CHAR* textureFile, TextureClass* texture, TextureShader* shader)
{
    //refreshRate = rate;
    this->shader = shader;
    this->texture = texture->GetTexture(textureFile);
    if (InitializeData()) return 1;
    if (Setup(device)) return 1;
    return 0;
}
RESULT TextureObject::Release() {
    delete this;
    return 0;
}
RESULT TextureObject::Render(ID3D11DeviceContext* deviceContext,
                                     D3DXMATRIX worldMatrix,
                                     D3DXMATRIX viewMatrix,
                                     D3DXMATRIX projectionMatrix)
{
    D3D11_MAPPED_SUBRESOURCE mappedVertices;
    BLOCKCALL(deviceContext->Map(vertexBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVertices));
    memcpy(mappedVertices.pData, pointArray, sizeof(VertexType) * pointCount);
    deviceContext->Unmap(vertexBuf, 0);

    unsigned int stride = sizeof(VertexType);
    unsigned int offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, &vertexBuf, &stride, &offset);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Shader render
    CALL(shader->Render(deviceContext, pointCount, worldMatrix, viewMatrix, projectionMatrix, texture),
         "warning: shader render failed\n")
    // else cerr << "shader on\n";

    return 0;
}
RESULT TextureObject::InitializeData()
{
    pointCount = 3;

    BLOCKALLOC(VertexType[pointCount], pointArray);

    pointArray[0].position = D3DXVECTOR3(-0.1f, -0.1f, 0.2f);   // Bottom left
    pointArray[1].position = D3DXVECTOR3(0.0f, 0.1f, 0.2f);     // Top middle
    pointArray[2].position = D3DXVECTOR3(0.1f, -0.1f, 0.2f);    // Bottom right

    pointArray[0].texture = D3DXVECTOR2(0.0f, 1.0f);
    pointArray[1].texture = D3DXVECTOR2(0.5f, 0.0f);
    pointArray[2].texture = D3DXVECTOR2(1.0f, 1.0f);
    return 0;
}
RESULT TextureObject::Frame()
{
    return 0;
}
int TextureObject::GetPointCount() {
    return pointCount;}
ID3D11ShaderResourceView* TextureObject::GetTexture() {
    return texture;
}
TextureObject::VertexType& TextureObject::operator[](int x)
/** Denoted counterclockwise */
{   return pointArray[ (x*2 < pointCount) ? x*2 : (pointCount - x)*2 - 1];}
