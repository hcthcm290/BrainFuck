#ifndef CAMERACLASS_H
#define CAMERACLASS_H

#include "useful_stuff.h"
#include <D3DX10math.h>

typedef D3DXVECTOR3 vector3;

class CameraClass
{
    public:
        CameraClass();
        ~CameraClass();

        // Unprotected member
        vector3 position;
        vector3 rotation;
        ///////////////////////

        RESULT Render();
        void GetViewMatrix(D3DXMATRIX&);

        RESULT Initialize();
        RESULT Release();

        D3DXMATRIX viewMatrix;
};

#endif // CAMERACLASS_H
