#include <DirectXMath.h>
#include "header/direct3d.h"
#include "header/shader3d.h"
#include "header/camera.h"
#include "header/key_logger.h"
#include "header/debug_text.h"
#include <sstream>
using namespace DirectX;

static XMFLOAT3 g_cameraPos = { 0.0f, 0.0f, -5.0f };
static XMFLOAT3 g_cameraFront = { 0.0f, 0.0f, 1.0f };
static XMFLOAT3 g_cameraUp = { 0.0f, 1.0f, 0.0f };
static XMFLOAT3 g_cameraRight = { 1.0f, 0.0f, 0.0f };

static constexpr float CAMERA_MOVE_SPEED = 1.0f;
static constexpr float CAMERA_ROTATION_SPEED = XMConvertToRadians(30);

static XMFLOAT4X4 g_cameraMatrix;
static XMFLOAT4X4 g_PerspectiveMatrix;
static hal::DebugText* g_pDT = nullptr;


void Camera_Initialize()
{
    static XMFLOAT3 g_cameraPos = { 0.0f, 0.0f, -5.0f };
	// 必ず直角にする
    static XMFLOAT3 g_cameraFront = { 0.0f, 0.0f, 1.0f };
    static XMFLOAT3 g_cameraUp = { 0.0f, 1.0f, 0.0f };
    static XMFLOAT3 g_cameraRight = { 1.0f, 0.0f, 0.0f };

    XMStoreFloat4x4(&g_cameraMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&g_PerspectiveMatrix, XMMatrixIdentity());

#if defined(DEBUG) || defined(_DEBUG)

   g_pDT = new hal::DebugText(Direct3D_GetDevice(), Direct3D_GetContext(),
        L"consolab_ascii_512.png",
        Direct3D_GetBackBufferWidth(), Direct3D_GetBackBufferHeight(),
        0.0f, 30.0f,
        0, 0,
        0.0f, 16.0f);
#endif
}
void Camera_Initialize(DirectX::XMFLOAT3& position, DirectX::XMFLOAT3& front, DirectX::XMFLOAT3& right, DirectX::XMFLOAT3& up)
{
    Camera_Initialize();
    static XMFLOAT3 g_cameraPos = position;
    XMStoreFloat3(&g_cameraFront, XMVector3Normalize(XMLoadFloat3(&position)));
    XMStoreFloat3(&g_cameraRight, XMVector3Normalize(XMLoadFloat3(&position)));
    XMStoreFloat3(&g_cameraUp, XMVector3Normalize(XMLoadFloat3(&position)));
    // 必ず直角にする
    static XMFLOAT3 g_cameraFront = front;
    static XMFLOAT3 g_cameraUp = up;
    static XMFLOAT3 g_cameraRight = right;

    XMStoreFloat4x4(&g_cameraMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&g_PerspectiveMatrix, XMMatrixIdentity());
}
void Camera_Update(double elapsed_time)
{
	XMVECTOR cameraFront = XMLoadFloat3(&g_cameraFront);
	XMVECTOR cameraUp = XMLoadFloat3(&g_cameraUp);
	XMVECTOR cameraRight = XMLoadFloat3(&g_cameraRight);
	XMVECTOR cameraPos = XMLoadFloat3(&g_cameraPos);

    // 下向き
    if (KeyLogger_IsPressed(KK_DOWN))
    {
        XMMATRIX rotation = XMMatrixRotationAxis(cameraRight, CAMERA_ROTATION_SPEED * elapsed_time);
        cameraFront = XMVector3TransformNormal(cameraFront, rotation);
        cameraFront = XMVector3Normalize(cameraFront);
        cameraUp = XMVector3Normalize(XMVector3Cross(cameraFront, cameraRight));
	}

    // 上向き
    if (KeyLogger_IsPressed(KK_UP))
    {
        XMMATRIX rotation = XMMatrixRotationAxis(cameraRight, -CAMERA_ROTATION_SPEED * elapsed_time);
        cameraFront = XMVector3TransformNormal(cameraFront, rotation);
        cameraFront = XMVector3Normalize(cameraFront);
        cameraUp = XMVector3Normalize(XMVector3Cross(cameraFront, cameraRight));
    }

    // 右向き
    if (KeyLogger_IsPressed(KK_RIGHT))
    {
        // XMMATRIX rotation = XMMatrixRotationAxis(cameraUp, CAMERA_ROTATION_SPEED * elapsed_time);
        XMMATRIX rotation = XMMatrixRotationY(CAMERA_ROTATION_SPEED * elapsed_time);
        cameraUp = XMVector3Normalize(XMVector3TransformNormal(cameraUp, rotation));
        cameraFront = XMVector3TransformNormal(cameraFront, rotation);
        cameraFront = XMVector3Normalize(cameraFront);
        cameraRight = XMVector3Normalize(XMVector3Cross(cameraUp, cameraFront));
    }

    // 左向き
    if (KeyLogger_IsPressed(KK_LEFT))
    {
        // XMMATRIX rotation = XMMatrixRotationAxis(cameraUp, -CAMERA_ROTATION_SPEED * elapsed_time);
        XMMATRIX rotation = XMMatrixRotationY(-CAMERA_ROTATION_SPEED * elapsed_time);
        cameraUp = XMVector3Normalize(XMVector3TransformNormal(cameraUp, rotation));
        cameraFront = XMVector3TransformNormal(cameraFront, rotation);
        cameraFront = XMVector3Normalize(cameraFront);
        cameraRight = XMVector3Normalize(XMVector3Cross(cameraUp, cameraFront));
    }

    // カメラ移動 (前)
    if (KeyLogger_IsPressed(KK_W)) 
    {
        cameraPos += XMVector3Normalize(cameraFront * XMVECTOR{1.0f, 0.0f, 1.0f}) * CAMERA_MOVE_SPEED * elapsed_time;
    } // cameraPos += cameraFront * CAMERA_MOVE_SPEED * elapsed_time;
    // カメラ移動 (左)
	if (KeyLogger_IsPressed(KK_A)) cameraPos += -cameraRight * CAMERA_MOVE_SPEED * elapsed_time;
    // カメラ移動(後ろ)
	if (KeyLogger_IsPressed(KK_S)) {
        cameraPos += XMVector3Normalize(-cameraFront * XMVECTOR{ 1.0f, 0.0f, 1.0f }) * CAMERA_MOVE_SPEED * elapsed_time;
    } // cameraPos += -cameraFront * CAMERA_MOVE_SPEED * elapsed_time;
    // カメラ移動 (右)
	if (KeyLogger_IsPressed(KK_D)) cameraPos += cameraRight * CAMERA_MOVE_SPEED * elapsed_time;
    // カメラ移動 (上昇)
    if (KeyLogger_IsPressed(KK_Q)) {
        cameraPos += XMVECTOR{0.0f, 1.0f, 0.0f} * CAMERA_MOVE_SPEED * elapsed_time;

    } //cameraPos += cameraUp * CAMERA_MOVE_SPEED * elapsed_time;
    if (KeyLogger_IsPressed(KK_E)) {
        cameraPos += XMVECTOR{ 0.0f, -1.0f, 0.0f } * CAMERA_MOVE_SPEED * elapsed_time;
    } // cameraPos += -cameraUp * CAMERA_MOVE_SPEED * elapsed_time;
    

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

    XMStoreFloat4x4(&g_cameraMatrix, mtxView);
    Shader3d_SetViewMatrix(mtxView);
    
    // パースペクティブ行列の作成
    constexpr float fovAngleY = DirectX::XMConvertToRadians(60.0f);
    float aspectRatio = static_cast<float>(Direct3D_GetBackBufferWidth()) / Direct3D_GetBackBufferHeight();
    float nearZ = 0.01f;
    float farZ = 100.0f;
    DirectX::XMMATRIX mtxPerspective = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ); // cast として計算して、float変換する

    // 頂点シェーダにプロジェクション変換
    XMStoreFloat4x4(&g_PerspectiveMatrix, mtxView);
    Shader3d_SetProjectionMatrix(mtxPerspective);
}
void Camera_Finalize()
{
    delete g_pDT;
}

const DirectX::XMFLOAT4X4& Camera_GetMatrix()
{
    return g_cameraMatrix;
}

const DirectX::XMFLOAT4X4& Camera_GetPerspectiveMatrix()
{
    return g_PerspectiveMatrix;
}

const DirectX::XMFLOAT3& Camera_GetPosition()
{
    return g_cameraPos;
}

const DirectX::XMFLOAT3& Camera_GetFront()
{
    return g_cameraFront;
}

void Camera_DebugDraw()
{
#if defined(DEBUG) || defined(_DEBUG)
    std::stringstream ss;
    ss << "Camera Position: x = " << g_cameraPos.x << " y = " << g_cameraPos.y << " z = " << g_cameraPos.z << std::endl;

    ss << "Camera Front: x = " << g_cameraFront.x << " y = " << g_cameraFront.y << " z = " << g_cameraFront.z << std::endl;
    ss << "Camera Right: x = " << g_cameraRight.x << " y = " << g_cameraRight.y << " z = " << g_cameraRight.z << std::endl;
    ss << "Camera Up: x = " << g_cameraUp.x << " y = " << g_cameraUp.y << " z = " << g_cameraUp.z << std::endl;
    g_pDT->SetText(ss.str().c_str(), {0.0f, 1.0f, 0.0f ,1.0f});
    g_pDT ->Draw();
    g_pDT->Clear();
#endif
}
