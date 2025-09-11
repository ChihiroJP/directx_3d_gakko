/*==============================================================================

	Game –{‘Ì [game.cpp]
														 Author : CheeHow
														 Date   : 2025/06/27
--------------------------------------------------------------------------------

==============================================================================*/
#include "header/game.h"
#include "header/cube.h"
#include "header/grid.h"
#include "header/camera.h"


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
	elapsed_time = elapsed_time + 10 - 10;
}

void Game_Draw()
{
	Camera_Update();
	
	Grid_Draw();
	Cube_Draw();
	
}
