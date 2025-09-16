/*==============================================================================

Aduio  [audio.h]
                                                         Author : Chee How
                                                         Date   : 2025/07/09
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef CAMERA_H
#define CAMERA_H

void Camera_Initialize();
void Camera_Update(double elapsed_time);
void Camera_Finalize();

const DirectX::XMFLOAT4X4& Camera_GetMatrix();
const DirectX::XMFLOAT4X4& Camera_GetPerspectiveMatrix();


#endif // CAMERA_H