#include <DirectXMath.h>
#include "header/direct3d.h"
#include "header/shader3d.h"
#include "header/camera.h"

void Camera_Initialize(){}
void Camera_Update()
{
    // ビュー座標へんかんぎょうれつの作成
    DirectX::XMMATRIX mtxView = DirectX::XMMatrixLookAtLH(
        { 2.0f, 2.0f, -5.0f }, // カメラの位置
        { 0.0f, 0.0f, 5.0f }, // 注視点の位置 （カメラの视角）
        { 0.0f, 1.0f, 0.0f }); // カメラの上方向ベクトル

    Shader3d_SetViewMatrix(mtxView);
    
    // パースペクティブ行列の作成
    constexpr float fovAngleY = DirectX::XMConvertToRadians(60.0f);
    float aspectRatio = static_cast<float>(Direct3D_GetBackBufferWidth()) / Direct3D_GetBackBufferHeight();
    float nearZ = 0.01f;
    float farZ = 100.0f;
    DirectX::XMMATRIX mtxPerspective = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ); // cast として計算して、float変換する

    // 頂点シェーダにプロジェクション変換
    Shader3d_SetProjectionMatrix(mtxPerspective);
}
void Camera_Finalize(){}