#include "TextureShader.h"

TextureShader::TextureShader() {
    pixelShader = 0;
    vertexShader = 0;
    layout = 0;
    matrixBuf = 0;
    sampleState = 0;
}
TextureShader::~TextureShader() {
    DESTROY(sampleState);
    DESTROY(matrixBuf);
    DESTROY(layout);
    DESTROY(pixelShader);
    DESTROY(vertexShader);
}
RESULT TextureShader::Initialize(ID3D11Device* device, HWND hwnd)
{
    if (InitShader(device, hwnd, VSHADER_FILE, PSHADER_FILE))
    {
        cerr << "... SHADER ...\n";
        return 1;
    }

    D3D11_BUFFER_DESC matrixBufDesc = {};

    matrixBufDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    BLOCKCALL(device->CreateBuffer(&matrixBufDesc, NULL, &matrixBuf));

    return 0;
}
RESULT TextureShader::InitShader(ID3D11Device* device,
                                    HWND hwnd,
                                    CHAR* vertexShaderFile,
                                    CHAR* pixelShaderFile) {
    ID3D10Blob* errorMessage = 0;
    ID3D10Blob* VSbuf = 0;
    ID3D10Blob* PSbuf = 0;
    const int pointDescCount = 2;
    D3D11_INPUT_ELEMENT_DESC  pointDesc[pointDescCount] = {};

    // cerr << "prepare to load";
    if(FAILED(D3DX11CompileFromFile(vertexShaderFile, NULL, NULL,       // file, define, include
                                    VSHADER_FUNC_NAME, "vs_5_0",       // names
                                    D3D10_SHADER_ENABLE_STRICTNESS,     // compiler setting
                                    0, NULL,
                                    &VSbuf, &errorMessage,              // output
                                    NULL))) {
        if (errorMessage)
            OutputShaderErrorMessage(errorMessage, hwnd, VSHADER_FUNC_NAME);
        else
            cerr << VSHADER_FUNC_NAME << " does not exist";
        return 1;}
    // else cerr << "Load VShader succeed\n";
    if (FAILED(D3DX11CompileFromFile(pixelShaderFile, NULL, NULL,       // file, define, include
                                     PSHADER_FUNC_NAME, "ps_5_0",       // names
                                     D3D10_SHADER_ENABLE_STRICTNESS,    // compiler setting
                                     0, NULL,
                                     &PSbuf, &errorMessage,             // output
                                     NULL))) {
        if (errorMessage)
            OutputShaderErrorMessage(errorMessage, hwnd, PSHADER_FUNC_NAME);
        else
            cerr << PSHADER_FUNC_NAME << " does not exist";
        return 1;}
    // else cerr << "Load PShader succeed\n";

    BLOCKCALL(device->CreateVertexShader(VSbuf->GetBufferPointer(), VSbuf->GetBufferSize(),
                                         NULL, &vertexShader));
    BLOCKCALL(device->CreatePixelShader( PSbuf->GetBufferPointer(), PSbuf->GetBufferSize(),
                                         NULL, &pixelShader));

    pointDesc[0].SemanticName = "POSITION";
    pointDesc[1].SemanticName = "TEXCOORD";
    pointDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    pointDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;

    pointDesc[0].AlignedByteOffset =
    pointDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    pointDesc[0].InputSlotClass =
    pointDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

    BLOCKCALL(device->CreateInputLayout(pointDesc, pointDescCount,
                                        VSbuf->GetBufferPointer(),
                                        VSbuf->GetBufferSize(),
                                        &layout));

    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;      // D3D11_TEXTURE_ADDRESS_BORDER: outside [0-1] is BorderColor
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;      // D3D11_TEXTURE_ADDRESS_CLAMP: outside [0-1] is 0/1
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;      // D3D11_TEXTURE_ADDRESS_MIRROR: outside [0-1] is mirrored
	samplerDesc.MipLODBias = 0.0f;                          // Blur
	samplerDesc.MaxAnisotropy = 1;                          // For anisotropic filter
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;                                 // biggest, most detailed mipmap
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;                 // smallest, least detailed mipmap

	BLOCKCALL(device->CreateSamplerState(&samplerDesc, &sampleState));
    return 0;
}
RESULT TextureShader::Release() {
    delete this;
    return 0;
}
RESULT TextureShader::Render(ID3D11DeviceContext* deviceContext, int pointCount,
                           D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
                           ID3D11ShaderResourceView* texture) {
    if (LoadDrawData(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture)) {
        cerr << "failed to load draw data\n";
        return 1;
    }//*/
    RenderShader(deviceContext, pointCount);
    return 0;
}
void TextureShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage,
                                           HWND hwnd,
                                           CHAR* shaderFuncName) {
    cerr << "ERROR:  ";
    char* compileError = (char*)errorMessage->GetBufferPointer();
    int bufSize = errorMessage->GetBufferSize();
    for (int i=0; i<bufSize; i++)
        cerr << compileError[i];
    cerr << "in " << shaderFuncName;
    DESTROY(errorMessage);
}

RESULT TextureShader::LoadDrawData(ID3D11DeviceContext* deviceContext,
                                 D3DXMATRIX& worldMatrix,
                                 D3DXMATRIX& viewMatrix,
                                 D3DXMATRIX& projectionMatrix,
                                 ID3D11ShaderResourceView* texture){
    MatrixBufferType* pdata;
    D3D11_MAPPED_SUBRESOURCE mappedResource;

    D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
    D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
    D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);
    // Transpose matrix, required for DX11
    //D3DXMATRIX matrix = worldMatrix * viewMatrix * projectionMatrix;

    BLOCKCALL(deviceContext->Map(matrixBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

    pdata = (MatrixBufferType*)mappedResource.pData;

    pdata->projection = projectionMatrix;
    pdata->view = viewMatrix;
    pdata->world = worldMatrix;

    deviceContext->Unmap(matrixBuf, 0);

    deviceContext->VSSetConstantBuffers(0, 1, &matrixBuf);
    deviceContext->PSSetShaderResources(0, 1, &texture);
    return 0;
}

void TextureShader::RenderShader(ID3D11DeviceContext* deviceContext, int pointCount)
{
    deviceContext->IASetInputLayout(layout);

    deviceContext->VSSetShader(vertexShader, NULL, 0);
    deviceContext->PSSetShader(pixelShader, NULL, 0);

    deviceContext->PSSetSamplers(0, 1, &sampleState);

    deviceContext->Draw(pointCount, 0);
}
