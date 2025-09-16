// ---------------------------------------------------------------------------------------------------
// 3D Cube (cube.h)
// ---------------------------------------------------------------------------------------------------
#ifndef CUBE_H
#define CUBE_H

#include <d3d11.h>
#include <DirectXMath.h>;

void Cube_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Cube_Finalize(void);
void Cube_Draw(DirectX::XMMATRIX mtxWorld);
void Cube_Update(double elapsed_time);

#endif // CUBE_H
