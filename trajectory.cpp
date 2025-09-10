/*==============================================================================

   �O�� [trajectory.cpp]
														 Author : Chee How
														 Date   : 2025/03/09
--------------------------------------------------------------------------------

==============================================================================*/
#include <d3d11.h>
#include "header/trajectory.h"
#include "header/texture.h"
#include "header/sprite.h"
#include "header/direct3d.h"

using namespace DirectX;

struct Trajectory
{
	XMFLOAT2 position;
	XMFLOAT4 color;
	float size;
	double lifetime;
	double birthTime;
};

static constexpr unsigned int MAX_TRAJECTORIES = 4096;

static Trajectory g_Trajectories[MAX_TRAJECTORIES] = {};
static int g_TrajectoryTexId = -1;
static double g_Time = 0.0; // Update time


void Trajectory_Initialize()
{
	for (Trajectory& t : g_Trajectories)
	{
		t.birthTime = 0.0;
	}

	g_Time = 0.0;
	g_TrajectoryTexId = Texture_Load(L"../../effect000.jpg");
}

void Trajectory_Finalize()
{
}

void Trajectory_Update(double elapsed_time)
{
	g_Time += elapsed_time;
	for (Trajectory& t : g_Trajectories)
	{
		if (t.birthTime == 0.0) continue;
		// alive time
		double time = g_Time - t.birthTime;
		if (time >= t.lifetime)
		{
			t.birthTime = 0.0; // dead
		}
	}

	g_Time += elapsed_time; // update time
}

void Trajectory_Draw()
{
	for (const Trajectory& t : g_Trajectories)
	{
		if (t.birthTime == 0.0) continue;
		// alive time
		double time = g_Time - t.birthTime;
		float ratio = (float)(time / t.lifetime);
		float size = t.size * (1.0f - ratio);
		float half_size = size * 0.5f;
		XMFLOAT4 color = t.color;
		color.w = t.color.w * (1.0f - ratio); // only ratio = weak to strong effect, 1.0 - ratio = strong to weak effect

		Sprite_Draw(g_TrajectoryTexId, t.position.x - half_size, size, size, t.position.y - half_size, t.color);
	}

	Direct3D_SetAlphaBlendAdd();
}

// coordinate

void Trajectory_Create(const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT4& color, float size, double lifetime)
{
	
	for (Trajectory& t : g_Trajectories)
	{
		if (t.birthTime != 0.0) continue;

		t.position = position;
		t.color = color;
		t.size = size;
		t.birthTime = g_Time; // alive
		break;		
	}
}
