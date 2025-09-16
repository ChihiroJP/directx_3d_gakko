#include <DirectXMath.h>
#include "header/direct3d.h"
#include "header/shader3d.h"
#include "header/camera.h"
#include "header/key_logger.h"
using namespace DirectX;
static XMFLOAT3 g_cameraPos = { 0.0f, 0.0f, -5.0f };
static XMFLOAT3 g_cameraFront= { 0.0f, 0.0f, 1.0f };
static XMFLOAT3 g_cameraUp = { 0.0f, 1.0f, 0.0f };
static XMFLOAT3 g_cameraRight = { 1.0f, 0.0f, 0.0f };

static constexpr float CAMERA_MOVE_SPEED = 0.1f;
static constexpr float CAMERA_ROTATION_SPEED = XMConvertToRadians(30);


void Camera_Initialize()
{
    static XMFLOAT3 g_cameraPos = { 0.0f, 0.0f, -5.0f };
	// 必ず直角にする
    static XMFLOAT3 g_cameraFront = { 0.0f, 0.0f, 1.0f };
    static XMFLOAT3 g_cameraUp = { 0.0f, 1.0f, 0.0f };
    static XMFLOAT3 g_cameraRight = { 1.0f, 0.0f, 0.0f };
}
void Camera_Update(double elapsed_time)
{
	XMVECTOR cameraFront = XMLoadFloat3(&g_cameraFront);
	XMVECTOR cameraUp = XMLoadFloat3(&g_cameraUp);
	XMVECTOR cameraRight = XMLoadFloat3(&g_cameraRight);
	XMVECTOR cameraPos = XMLoadFloat3(&g_cameraPos);

    // カメラ移動 (前)
	if (KeyLogger_IsPressed(KK_W)) cameraPos += cameraFront * CAMERA_MOVE_SPEED * elapsed_time;
	if (KeyLogger_IsPressed(KK_A)) cameraPos += -cameraRight * CAMERA_MOVE_SPEED * elapsed_time;
	if (KeyLogger_IsPressed(KK_S)) cameraPos += -cameraFront * CAMERA_MOVE_SPEED * elapsed_time;
	if (KeyLogger_IsPressed(KK_D)) cameraPos += cameraRight * CAMERA_MOVE_SPEED * elapsed_time;

	// 各種更新結果の保存
	XMStoreFloat3(&g_cameraPos, cameraPos);
	XMStoreFloat3(&g_cameraFront, cameraFront);
	XMStoreFloat3(&g_cameraUp, cameraUp);
	XMStoreFloat3(&g_cameraRight, cameraRight);

    // ビュー座標へんかんぎょうれつの作成
    DirectX::XMMATRIX mtxView = DirectX::XMMatrixLookAtLH(
        cameraPos, // カメラの位置
        cameraPos + cameraFront, // 注視点の位置 （カメラの视角）
        cameraUp); // カメラの上方向ベクトル

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
void Camera_Finalize()
{

}