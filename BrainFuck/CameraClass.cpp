#include "CameraClass.h"

CameraClass::CameraClass() {
    position = vector3(0,0,0);
    rotation = vector3(0,0,0);
}

CameraClass::~CameraClass() {}

RESULT CameraClass::Render()
{
    vector3 up = vector3(0, 1, 0);      // define up vector
    vector3 look = vector3(0, 0, 1);    // default look direction
    vector3 pos = position;
    vector3 rot = rotation * D3DX_PI / 180;
    D3DXMATRIX rotationMatrix;

    D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rot.y, rot.x, rot.z); // yaw is y, pitch is x, roll is z

    D3DXVec3TransformCoord(&look, &look, &rotationMatrix);
    D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

    look = pos + look;

    D3DXMatrixLookAtLH(&viewMatrix, &pos, &look, &up);
    return 0;
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
    viewMatrix = this->viewMatrix;
}

RESULT CameraClass::Initialize()
{
    return 0;
}

RESULT CameraClass::Release()
{
    delete this;
    return 0;
}
