/*==============================================================================

Aduio  [audio.h]
                                                         Author : Chee How
                                                         Date   : 2025/07/09
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef CAMERA_H
#define CAMERA_H

void Camera_Initialize();
void Camera_Initialize(DirectX::XMFLOAT3& position, DirectX::XMFLOAT3& front, DirectX::XMFLOAT3& right, DirectX::XMFLOAT3& up);
void Camera_Update(double elapsed_time);
void Camera_Finalize();

const DirectX::XMFLOAT4X4& Camera_GetMatrix();
const DirectX::XMFLOAT4X4& Camera_GetPerspectiveMatrix();
const DirectX::XMFLOAT3& Camera_GetPosition();
const DirectX::XMFLOAT3& Camera_GetFront();

void Camera_DebugDraw();

#endif // CAMERA_H