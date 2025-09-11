#include "header/grid.h"
#include <DirectXMath.h>
#include "header/direct3d.h"
#include "header/shader3d.h"

static constexpr int GRID_H_COUNT = 10; // grid horizontal total block count
static constexpr int GRID_V_COUNT = 10; // grid vertical total block count
static constexpr int GRID_H_LINE_COUNT = GRID_H_COUNT + 1;
static constexpr int GRID_V_LINE_COUNT = GRID_V_COUNT + 1;
static constexpr int NUM_VERTEX = GRID_H_LINE_COUNT * 2 + GRID_V_LINE_COUNT * 2;

void Grid_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

}

void Grid_Finalize(void)
{
	
}

void Grid_Draw(void)
{
	
}