/*==============================================================================

   シェーダー [shader3d.h]
														 Author : CheeHow
														 Date   : 2025/09/09
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef SHADER3D_H
#define	SHADER3D_H

#include <d3d11.h>
#include <DirectXMath.h>

bool Shader_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Shader_Finalize();

void Shader_SetWorldMatrix(const DirectX::XMMATRIX& matrix);
void Shader_SetProjectionMatrix(const DirectX::XMMATRIX& matrix);

void Shader_Begin();

#endif // SHADER3D_H
