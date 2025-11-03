#include "GameWorld.h"

#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>
#include <tge/engine.h>

using namespace Tga;

GameWorld::GameWorld()
{
	myBall = nullptr;
	myInput = nullptr;
}

GameWorld::~GameWorld()
{
	delete myBall;
}

void GameWorld::Init()
{
	Tga::Engine& engine = *Tga::Engine::GetInstance();
	myBall = new Ball();
	myBall->Init(engine);
}

void GameWorld::Update(float aTimeDelta)
{
	myBall->Update(aTimeDelta);
}

void GameWorld::Render()
{
	auto& engine = *Tga::Engine::GetInstance();
	Tga::SpriteDrawer& spriteDrawer(engine.GetGraphicsEngine().GetSpriteDrawer());
	myBall->Render(spriteDrawer);
}

inline void GameWorld::SetInputHandler(InputHandler* anInput)
{
	myInput = anInput;
}