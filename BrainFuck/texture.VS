cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};


struct VertexType
{
    float4 position : POSITION;
    float2 coord : TEXCOORD;
};


struct PixelType
{
    float4 position : SV_POSITION;
    float2 coord : TEXCOORD;
};


PixelType VShader(VertexType input)
{
    PixelType output;

    input.position.w = 1.0f;

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    // inefficiency here

    output.coord = input.coord;

    return output;
}
