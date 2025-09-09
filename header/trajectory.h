#pragma once
/*==============================================================================

   ãOê’ [trajectory.h]
														 Author : Chee How
														 Date   : 2025/03/09
--------------------------------------------------------------------------------

==============================================================================*/

#ifndef TRAJECTORY_H
#define TRAJECTORY_H
#include <DirectXMath.h>

void Trajectory_Initialize();
void Trajectory_Finalize();
void Trajectory_Update(double elapsed_time);
void Trajectory_Draw();

// coordinate
void Trajectory_Create(const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT4& color, float size, double lifetime);


#endif // !TRAJECTORY_H
