/*==============================================================================

	Game [game.cpp]
														 Author : CheeHow
														 Date   : 2025/06/27
--------------------------------------------------------------------------------

==============================================================================*/
#include "header/game.h"
#include "header/cube.h"
#include "header/grid.h"
#include "header/camera.h"
#include "header/direct3d.h"
#include "header/key_logger.h"
#include <DirectXMath.h>

static float g_x = 0.0f;
static float g_angle = 0.0f;
static float g_scale = 1.0f;
static float g_AccumulatedTime = 0.0f;
static float total = 5.0f;
static DirectX::XMFLOAT3 g_CubePosition{};
static DirectX::XMFLOAT3 g_CubeVelocity{};

static bool g_start = false;

static int g_BgmId = -1;
static bool g_GameStart = false;

void Game_Initialize()
{
	Camera_Initialize({8.0f, 8.0f, -12.0f }, {-0.5f, -0.3f, -0.7f}, {0.8f, 0.0f, 0.5f}, {-0.18f, 0.9f, 0.2f});
}

void Game_Finalize()
{
	Camera_Finalize();
}

void Game_Update(double elapsed_time)
{
	g_AccumulatedTime = g_AccumulatedTime + static_cast<float>(elapsed_time);
	g_x = sinf(g_AccumulatedTime) * 4.5f; // Move left and right
	g_scale = (sinf(g_AccumulatedTime) + 1.0f) * 0.5f + 0.5f; // Scale between 0.0 and 1.5
	g_angle = g_AccumulatedTime * 0.5f; // Rotate over time

	Camera_Update(elapsed_time);

    if (KeyLogger_IsTrigger(KK_SPACE)) 
    {
        g_CubePosition = Camera_GetPosition();
        DirectX::XMStoreFloat3(&g_CubeVelocity, DirectX::XMLoadFloat3(&Camera_GetFront()) * 10.0f * static_cast<float>(elapsed_time));
    }

    DirectX::XMVECTOR cube_position = DirectX::XMLoadFloat3(&g_CubePosition);
    cube_position += DirectX::XMLoadFloat3(&g_CubeVelocity) * elapsed_time;
    DirectX::XMStoreFloat3(&g_CubePosition, cube_position);
}

void Game_Draw()
{
    DirectX::XMMATRIX mtxWorld = DirectX::XMMatrixRotationY(g_angle * 2.0f);
    mtxWorld *= DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&g_CubePosition));
    Cube_Draw(mtxWorld);
	Grid_Draw();
    // ===========================================================================pyramic=========================================================================================
    for (int y = 0; y < total; y++) {
        for (int z = 0; z < total - y; z++) {
            for (int x = 0; x < total - y; x++) {
                // ワールド座標変換行列
                DirectX::XMMATRIX mtxWorld1 = DirectX::XMMatrixIdentity(); 
                DirectX::XMMATRIX mtxPosition1 = DirectX::XMMatrixTranslation(-4.5f + x + 0.5f * y, 0.5f + y, -4.5f + z + 0.5f * y); // move position
                DirectX::XMMATRIX mtxRotateY = DirectX::XMMatrixRotationY(g_angle);
                mtxWorld1 = mtxRotateY * mtxPosition1; // Combine ( Remember need to follow (Rotate -> Scaling -> Move)

                
				Cube_Draw(mtxWorld1);
            }
        }
    }
    // =======================================================================================================================================================
    Camera_DebugDraw();
}
