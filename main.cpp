#include <Windows.h>
#include <sstream>
#include "header/debug_ostream.h"
#include <SDKDDKVer.h>
#include "header/GameWindow.h"
#define WIN32_LEAN_AND_MEAN
#include "header/direct3d.h"
#include "header/shader.h"
#include "header/shader3d.h"
#include "header/sprite.h"
#include "header/texture.h"
#include "header/sprite_anim.h"
#include "header/fade.h"
#include "header/collision.h"
#include "header/debug_text.h"
#include "header/system_timer.h"
#include "header/polygon.h"
#include <math.h>
#include <DirectXMath.h>
#include "header/key_logger.h"
#include "header/mouse.h"
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")
#include "header/scene.h"
#include "header/Audio.h"
#include "header/score.h"
#include "header/cube.h"
#include "header/grid.h"

using namespace DirectX;

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int nCmdShow)
{
	(void) CoInitializeEx(nullptr, COINIT_MULTITHREADED); 

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	HWND hWnd = GameWindow_Create(hInstance);

	SystemTimer_Initialize();
	KeyLogger_Initialize();
	Mouse_Initialize(hWnd);
	InitAudio();

	Direct3D_Initialize(hWnd);
	Shader_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
	Shader3d_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
	Texture_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
	Sprite_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
	SpriteAnim_Initialize();
	Fade_Initialize();
	Mouse_SetVisible(false);
	Scene_Initialize();
	Cube_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
	Grid_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());


#if defined(DEBUG) || defined(_DEBUG)

	hal::DebugText dt(Direct3D_GetDevice(), Direct3D_GetContext(),
		L"consolab_ascii_512.png",
		Direct3D_GetBackBufferWidth(), Direct3D_GetBackBufferHeight(),
		0.0f, 0.0f,
		0, 0,
		0.0f, 16.0f);

	Collision_DebugInitialize(Direct3D_GetDevice(), Direct3D_GetContext());
#endif

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//Frame Count 
	double exec_last_time = SystemTimer_GetTime();
	double fps_last_time = exec_last_time;
	double current_time = 0.0;
	ULONG frame_count = 0;
	double current_fps = 0.0;

	/*Message Loop and Game Loop*/
	MSG msg;

	do {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) { // ウィンドウメッセージが来ていたら 
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else { 	//Game Logic

			// fps case
			current_time = SystemTimer_GetTime();
			double elapsed_time = current_time - fps_last_time;

			if (elapsed_time >= 1.0)
			{
				current_fps = frame_count / elapsed_time;
				fps_last_time = current_time;
				frame_count = 0;
			}
			// 1/60 秒ことに実行
			elapsed_time = current_time - exec_last_time;

			if (elapsed_time >= (1.0 / 60.0)) {
				exec_last_time = current_time;
			}

			KeyLogger_Update();
			Scene_Update(elapsed_time);
			SpriteAnim_Update(elapsed_time);
			Fade_Update(elapsed_time);

			Direct3D_Clear();
			Sprite_Begin();
			Scene_Draw();
			Fade_Draw();

#if defined(DEBUG) || defined(_DEBUG)
			std::stringstream ss;
			ss << "Fps: " << current_fps;
			dt.SetText(ss.str().c_str());
			dt.Draw();
			dt.Clear();
#endif
			Direct3D_Present();

			Scene_Refresh();
			frame_count++;
		}

	} while (msg.message != WM_QUIT);

#if defined(DEBUG) || defined(_DEBUG)
	Collision_DebugFinalize();
#endif

	Cube_Finalize();
	Grid_Finalize();
	Scene_Finalize();
	Fade_Finalize();
	SpriteAnim_Finalize();
	Texture_Finialize();
	Sprite_Finalize();
	Shader_Finalize();
	Shader3d_Finalize();
	Direct3D_Finalize();
	UninitAudio();
	Mouse_Finalize();

	return(int)msg.wParam;

}