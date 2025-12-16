#include "GameWorld.h"

#include <tge/input/InputManager.h>
#include <tge/scene/Scene.h>
#include <tge/scene/SceneSerialize.h>
#include <tge/settings/settings.h>
#include <tge/error/ErrorManager.h>

#include <Timer.h>
#include <Windows.h>
#include <InputHandler.h>

static InputHandler* gInputHandler = nullptr;

LRESULT WinProc([[maybe_unused]] HWND hWnd, UINT message, [[maybe_unused]] WPARAM wParam, [[maybe_unused]] LPARAM lParam)
{
	(void)hWnd;
	if (gInputHandler)
	{
		gInputHandler->UpdateEvents(message, wParam, lParam);
		gInputHandler->UpdateMouseInput(message, wParam, lParam);
	}

	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	}
	return 0;
}

void Go()
{
	Tga::LoadSettings(TGE_PROJECT_SETTINGS_FILE);

	Tga::EngineConfiguration& cfg = Tga::Settings::GetEngineConfiguration();

	cfg.myWinProcCallback = [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {return WinProc(hWnd, message, wParam, lParam); };
#ifdef _DEBUG
	cfg.myActivateDebugSystems = Tga::DebugFeature::Fps | Tga::DebugFeature::Mem | Tga::DebugFeature::Filewatcher | Tga::DebugFeature::Cpu | Tga::DebugFeature::Drawcalls | Tga::DebugFeature::OptimizeWarnings;
#else
	cfg.myActivateDebugSystems = Tga::DebugFeature::Filewatcher;
#endif

	if (!Tga::Engine::Start())
	{
		ERROR_PRINT("Fatal error! Engine could not start!");
		system("pause");
		return;
	}

	{
		GameWorld gameWorld;
		gameWorld.Init();

		InputHandler inputHandler;
		gInputHandler = &inputHandler;
		gameWorld.SetInputHandler(gInputHandler);

		Tga::Engine& engine = *Tga::Engine::GetInstance();

		while (engine.BeginFrame())
		{
			gInputHandler->UpdateInput();

			gameWorld.Update(engine.GetDeltaTime());
			gameWorld.Render();
			engine.EndFrame();
		}
	}

	gInputHandler = nullptr;
	Tga::Engine::GetInstance()->Shutdown();
}

