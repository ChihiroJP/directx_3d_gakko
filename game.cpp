/*==============================================================================

	Game 本体 [game.cpp]
														 Author : CheeHow
														 Date   : 2025/06/27
--------------------------------------------------------------------------------

==============================================================================*/
#include "header/game.h"
#include "header/cube.h"
#include "header/grid.h"
#include "header/camera.h"
#include "header/direct3d.h"
#include <DirectXMath.h>

static float g_x = 0.0f;
static float g_angle = 0.0f;
static float g_scale = 1.0f;
static float g_AccumulatedTime = 0.0f;
static float total = 5.0f;


static bool g_start = false;
void Hit_judgementBulletVsEnemy();
void Hit_judgementPlayerVsEnemy();

static int g_BgmId = -1;
static bool g_GameStart = false;

void Game_Initialize()
{
	Camera_Initialize();
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
}

void Game_Draw()
{
	Grid_Draw();
    // =========================================================================== pyramic =========================================================================================
    for (int y = 0; y < total; y++) {
        for (int z = 0; z < total - y; z++) {
            for (int x = 0; x < total - y; x++) {
                // ワールド座標変換行列
                DirectX::XMMATRIX mtxWorld1 = DirectX::XMMatrixIdentity(); // 単位行列の作成
                DirectX::XMMATRIX mtxPosition1 = DirectX::XMMatrixTranslation(-4.5f + x + 0.5f * y, 0.5f + y, -4.5f + z + 0.5f * y); // move position (平行移動)
                DirectX::XMMATRIX mtxRotateY = DirectX::XMMatrixRotationY(g_angle);
                mtxWorld1 = mtxRotateY * mtxPosition1; // Combine ( Remember need to follow (Rotate -> Scaling -> Move)

                // 頂点シェーダにワールド座標変換行列を設定
				Cube_Draw(mtxWorld1);
            }
        }
    }
    // =======================================================================================================================================================
	
}
