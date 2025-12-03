#include "GameWorld.h"

#include "CollisionSystem.h"
#include "ConfigLoader.h"
#include "GameObjectFactory.h"
#include "RenderTypes.h"
#include "MysteryShip.h"
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>
#include <tge/engine.h>
#include <tge/text/Text.h>
#include <Windows.h>

#include <InputHandler.h>
#include <Timer.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <Vector2.hpp>

using namespace Tga;

GameWorld::GameWorld()
{
	
}

GameWorld::~GameWorld()
{
}

void GameWorld::Init()
{
	myConfig = ConfigLoader::Load("Source/Game/data/config.txt");
	myHighScore = myConfig.defaultHighScore;
	myLives = myConfig.playerLives;

	Engine& engine = *Engine::GetInstance();
	GameTypesInitializer::Init(myTypes, engine, myConfig);
	myFactory = std::make_unique<GameObjectFactory>(myTypes);

	// HUD/Title/GameOver texts
	myPlayerScoreText = std::make_unique<Tga::Text>("Text/arial.ttf", FontSize_24);
	myHighScoreText = std::make_unique<Tga::Text>("Text/arial.ttf", FontSize_24);
	myGameOverText = std::make_unique<Tga::Text>("Text/arial.ttf", FontSize_36);
	myTitleText = std::make_unique<Tga::Text>("Text/arial.ttf", FontSize_48);
	mySubtitleText = std::make_unique<Tga::Text>("Text/arial.ttf", FontSize_24);
	myInstructionsText = std::make_unique<Tga::Text>("Text/arial.ttf", FontSize_24);
	myStatsText = std::make_unique<Tga::Text>("Text/arial.ttf", FontSize_24);

	myPlayerScoreText->SetColor({1,1,1,1});
	myHighScoreText->SetColor({1,1,1,1});
	myGameOverText->SetColor({1,0.5f,0.5f,1});
	myTitleText->SetColor({1,1,1,1});
	mySubtitleText->SetColor({0.8f,0.8f,0.8f,1});
	myInstructionsText->SetColor({0.8f,0.8f,0.8f,1});
	myStatsText->SetColor({1,1,1,1});

	myPlayer.Init(engine);
	myPlayer.SetMoveSpeed(myTypes.player.moveSpeed);

	std::vector<const EnemyType*> enemyTypes;
	for (const auto& type : myTypes.enemyTypes)
	{
		enemyTypes.push_back(&type);
	}
	myEnemyFormation.Init(enemyTypes, { 150.0f, 600.0f });

	myShieldManager.Init(myTypes.shield, 3);

	myEnemyFireTimer = myEnemyFireInterval;
	myState = EGameState::Title;

	// Mystery ship spawn timer
	myMysterySpawnTimer = static_cast<float>((std::rand() % 1000) / 1000.0) * (myMysterySpawnIntervalMax - myMysterySpawnIntervalMin) + myMysterySpawnIntervalMin;

	UpdateScoreText();
}


void GameWorld::Update(float aTimeDelta)
{
	// Title input
	if (myState == EGameState::Title)
	{
		if (myInput && myInput->IsKeyDown(VK_RETURN))
		{
			myState = EGameState::Playing;
		}
		if (myInput && myInput->IsKeyDown(VK_ESCAPE))
		{
			myState = EGameState::Exiting;
		}
		return;
	}

	switch (myState)
	{
	case EGameState::Playing:
		UpdatePlaying(aTimeDelta);
		break;
	case EGameState::GameOver:
		// Any key press resets to title
		if (myInput && (myInput->IsKeyDown(VK_RETURN) || myInput->IsKeyDown(VK_SPACE) || myInput->IsKeyDown(VK_ESCAPE)))
		{
			myState = EGameState::Title;
		}
		break;
	default:
		break;
	}
}

void GameWorld::Render()
{
	if (myState == EGameState::Title)
	{
		RenderTitleScreen();
		return;
	}

	// Collect render data from all game objects
	RenderWorld renderWorld{};

	// Player
	myPlayer.BuildRenderData(renderWorld);

	// Enemies
	for (const auto& enemy : myEnemyFormation.GetEnemies())
	{
		if (enemy && enemy->IsAlive())
		{
			enemy->BuildRenderData(renderWorld);
		}
	}

	// Mystery ship
	if (myMysteryShip)
	{
		myMysteryShip->BuildRenderData(renderWorld);
	}

	// Projectiles
	for (const auto& p : myProjectileManager.GetProjectiles())
	{
		if (p && p->IsAlive())
		{
			p->BuildRenderData(renderWorld);
		}
	}

	// Shields
	for (const auto& b : myShieldManager.GetBlocks())
	{
		if (b && b->IsAlive())
		{
			b->BuildRenderData(renderWorld);
		}
	}

	// Explosions
	for (const auto& explosion : myExplosions)
	{
		if (explosion && explosion->IsAlive())
		{
			explosion->BuildRenderData(renderWorld);
		}
	}

	// Draw all sprites
	SpriteDrawer& spriteDrawer = Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer();
	for (const auto& rs : renderWorld.sprites)
	{
		if (rs.shared)
		{
			spriteDrawer.Draw(*rs.shared, rs.instance);
		}
	}

	// HUD / overlays
	RenderHUD();
	if (myState == EGameState::GameOver)
	{
		RenderGameOverScreen();
	}
}

void GameWorld::SetInputHandler(InputHandler* anInput)
{
	myInput = anInput;
	myPlayer.SetInput(anInput);
}

Player& GameWorld::GetPlayer()
{
	return myPlayer;
}

const Player& GameWorld::GetPlayer() const
{
	return myPlayer;
}

EnemyFormation& GameWorld::GetEnemyFormation()
{
	return myEnemyFormation;
}

ProjectileManager& GameWorld::GetProjectileManager()
{
	return myProjectileManager;
}

ShieldManager& GameWorld::GetShieldManager()
{
	return myShieldManager;
}

HighScoreManager& GameWorld::GetHighScoreManager()
{
	return myHighScoreManager;
}

int GameWorld::GetScore() const
{
	return myPlayerScore;
}

void GameWorld::AddScore(int aScore)
{
	myPlayerScore += aScore;
	if (myPlayerScore > myHighScore)
	{
		myHighScore = myPlayerScore;
	}
	UpdateScoreText();
}

void GameWorld::RegisterEnemyKill(const CommonUtilities::Vector2<float>& pos)
{
	++myEnemiesKilled;
	myEnemyFormation.OnEnemyKilled();
	// Dynamic difficulty: increase enemy fire frequency (reduce interval) without std::min/max macro issues
	float factor = static_cast<float>(myEnemiesKilled) / 50.0f;
	if (factor > 1.0f) factor = 1.0f;
	myEnemyFireInterval = 2.0f * (1.0f - factor);
	if (myEnemyFireInterval < 0.6f) myEnemyFireInterval = 0.6f;
	myExplosions.push_back(std::make_unique<Explosion>(myTypes.explosion, pos));
}

void GameWorld::OnPlayerHit()
{
	if (myLives > 0)
	{
		--myLives;
	}
	UpdateScoreText();
}

EGameState GameWorld::GetState() const
{
	return myState;
}

void GameWorld::SetState(EGameState aState)
{
	myState = aState;
}

int GameWorld::GetLives() const
{
	return myLives;
}

void GameWorld::UpdateScoreText()
{
	// Update HUD texts: score, hiscore, lives
	std::string scoreStr = std::string("Score: ") + std::to_string(myPlayerScore);
	std::string hiStr = std::string("Hi: ") + std::to_string(myHighScore);
	std::string livesStr = std::string("Lives: ") + std::to_string(myLives);
	if (myPlayerScoreText) myPlayerScoreText->SetText(scoreStr);
	if (myHighScoreText) myHighScoreText->SetText(hiStr);
	if (myStatsText) myStatsText->SetText(livesStr);

	// Position texts
	Vector2ui intRes = Engine::GetInstance()->GetRenderSize();
	Vector2f res{ static_cast<float>(intRes.x), static_cast<float>(intRes.y) };
	if (myPlayerScoreText) myPlayerScoreText->SetPosition({ 20.0f, 20.0f });
	if (myHighScoreText) myHighScoreText->SetPosition({ res.x * 0.5f - 100.0f, 20.0f });
	if (myStatsText) myStatsText->SetPosition({ res.x - 200.0f, 20.0f });
}


void GameWorld::CheckGameOver()
{
	if (myLives <= 0)
	{
		myState = EGameState::GameOver;
	}
}

void GameWorld::ResetGame()
{
}

void GameWorld::UpdatePlaying(float aDeltaTime)
{
	if (myInput && myInput->IsKeyDown(VK_SPACE) && myProjectileManager.CanPlayerShoot())
	{
		CommonUtilities::Vector2<float> pos;
		pos.x = myPlayer.GetPosition().x;
		pos.y = myPlayer.GetPosition().y;
		myProjectileManager.SpawnPlayerShot(pos, myTypes.playerShot);
	}

	myPlayer.Update(aDeltaTime);
	myEnemyFormation.Update(aDeltaTime);
	myProjectileManager.Update(aDeltaTime);
	myShieldManager.Update(aDeltaTime);

	// Mystery ship update/spawn
	myMysterySpawnTimer -= aDeltaTime;
	if (myMysterySpawnTimer <= 0.0f && !myMysteryShip)
	{
		// Spawn at top, off-screen left, moving to right
		CommonUtilities::Vector2<float> startPos{ -50.0f, 800.0f };
		myMysteryShip = myFactory->CreateMysteryShip(startPos);
		// Reset next spawn time
		myMysterySpawnTimer = static_cast<float>((std::rand() % 1000) / 1000.0) * (myMysterySpawnIntervalMax - myMysterySpawnIntervalMin) + myMysterySpawnIntervalMin;
	}
	if (myMysteryShip)
	{
		myMysteryShip->Update(aDeltaTime);
		// Despawn if off-screen right
		if (myMysteryShip->GetPosition().x > Engine::GetInstance()->GetRenderSize().x + 50.0f || myMysteryShip->IsDead())
		{
			myMysteryShip.reset();
		}
	}

	myEnemyFireTimer -= aDeltaTime;
	if (myEnemyFireTimer <= 0.0f)
	{
		static bool seeded = false;
		if (!seeded)
		{
			std::srand(static_cast<unsigned>(time(nullptr)));
			seeded = true;
		}

		std::vector<Enemy*> shooters;
		for (const auto& enemy : myEnemyFormation.GetEnemies())
		{
			if (enemy->IsAlive())
			{
				shooters.push_back(enemy.get());
			}
		}

		if (!shooters.empty())
		{
			Enemy* shooter = shooters[std::rand() % shooters.size()];
			CommonUtilities::Vector2<float> pos = shooter->GetPosition();
			myProjectileManager.SpawnEnemyShot(pos, myTypes.enemyShot);
		}

		myEnemyFireTimer = myEnemyFireInterval;
	}

	for (auto& explosion : myExplosions)
	{
		if (explosion->IsAlive())
		{
			explosion->Update(aDeltaTime);
		}
	}
	myExplosions.erase(std::remove_if(myExplosions.begin(), myExplosions.end(),
		[](const std::unique_ptr<Explosion>& e) { return !e->IsAlive(); }), myExplosions.end());

	CollisionSystem::HandleCollisions(*this);
	myEnemyFormation.RemoveDead();

	for (const auto& enemy : myEnemyFormation.GetEnemies())
	{
		if (enemy->IsAlive() && enemy->GetPosition().y <= myPlayer.GetPosition().y)
		{
			myState = EGameState::GameOver;
			break;
		}
	}
}

void GameWorld::RenderHUD()
{
	if (myPlayerScoreText) myPlayerScoreText->Render();
	if (myHighScoreText) myHighScoreText->Render();
	if (myStatsText) myStatsText->Render();
}

void GameWorld::RenderTitleScreen()
{
	Vector2ui intRes = Engine::GetInstance()->GetRenderSize();
	Vector2f res{ static_cast<float>(intRes.x), static_cast<float>(intRes.y) };
	if (myTitleText)
	{
		myTitleText->SetText("SPACE INVADERS");
		myTitleText->SetPosition({ res.x * 0.5f - 220.0f, res.y * 0.3f });
		myTitleText->Render();
	}
	if (mySubtitleText)
	{
		mySubtitleText->SetText("By: Tarik Bergstrom");
		mySubtitleText->SetPosition({ res.x * 0.5f - 120.0f, res.y * 0.38f });
		mySubtitleText->Render();
	}
	if (myInstructionsText)
	{
		myInstructionsText->SetText("Arrows to move, Space to fire, Enter to start, Esc to quit");
		myInstructionsText->SetPosition({ res.x * 0.5f - 360.0f, res.y * 0.55f });
		myInstructionsText->Render();
	}
}

void GameWorld::RenderGameOverScreen()
{
	Vector2ui intRes = Engine::GetInstance()->GetRenderSize();
	Vector2f res{ static_cast<float>(intRes.x), static_cast<float>(intRes.y) };
	if (myGameOverText)
	{
		myGameOverText->SetText("GAME OVER");
		myGameOverText->SetPosition({ res.x * 0.5f - 120.0f, res.y * 0.4f });
		myGameOverText->Render();
	}
	if (myStatsText)
	{
		std::string stats = std::string("Level: ") + std::to_string(myCurrentLevel) +
			std::string("  Enemies killed: ") + std::to_string(myEnemiesKilled) +
			std::string("  Final score: ") + std::to_string(myPlayerScore);
		myStatsText->SetText(stats);
		myStatsText->SetPosition({ res.x * 0.5f - 300.0f, res.y * 0.5f });
		myStatsText->Render();
	}
}
