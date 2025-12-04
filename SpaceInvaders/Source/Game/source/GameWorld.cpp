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
	myScoreManager.Reset();
	myScoreManager.SetHighScore(myConfig.defaultHighScore);
	myLives = myConfig.playerLives;

	Engine& engine = *Engine::GetInstance();
	GameTypesInitializer::Init(myTypes, engine, myConfig);
	myFactory = std::make_unique<GameObjectFactory>(myTypes);

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

	myMysterySpawnTimer = static_cast<float>((std::rand() % 1000) / 1000.0) * (myMysterySpawnIntervalMax - myMysterySpawnIntervalMin) + myMysterySpawnIntervalMin;

	UpdateScoreText();
}


void GameWorld::Update(float aTimeDelta)
{
	if (myInput && myInput->IsKeyPressed('P'))
	{
		myIsPaused = !myIsPaused;
		if (myIsPaused)
		{
			myState = EGameState::Paused;
		}
		else
		{
			myState = EGameState::Playing;
		}
	}

	if (myState == EGameState::Paused)
	{
		return;
	}

	if (myState == EGameState::Title)
	{
		if (myInput && myInput->IsKeyPressed(VK_RETURN))
		{
			ResetGame();
			myState = EGameState::Playing;
		}
		if (myInput && myInput->IsKeyPressed(VK_ESCAPE))
		{
			myState = EGameState::Exiting;
		}
		return;
	}

	if (myState == EGameState::LevelCleared)
	{
		if (myInput && (myInput->IsKeyPressed(VK_RETURN) || myInput->IsKeyPressed(VK_SPACE)))
		{
			myCurrentLevel++;
			ResetGame();
			myState = EGameState::Playing;
		}
		return;
	}

	switch (myState)
	{
	case EGameState::Playing:
		UpdatePlaying(aTimeDelta);
		break;
	case EGameState::GameOver:
		if (myInput && (myInput->IsKeyPressed(VK_RETURN) || myInput->IsKeyPressed(VK_SPACE) || myInput->IsKeyPressed(VK_ESCAPE)))
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
	if (myState == EGameState::LevelCleared)
	{
		RenderLevelClearedScreen();
		return;
	}
	if (myState == EGameState::Paused)
	{
		RenderPauseScreen();
		return;
	}

	RenderWorld renderWorld{};

	myPlayer.BuildRenderData(renderWorld);

	for (const auto& enemy : myEnemyFormation.GetEnemies())
	{
		if (enemy && enemy->IsAlive())
		{
			enemy->BuildRenderData(renderWorld);
		}
	}

	if (myMysteryShip)
	{
		myMysteryShip->BuildRenderData(renderWorld);
	}

	for (const auto& p : myProjectileManager.GetProjectiles())
	{
		if (p && p->IsAlive())
		{
			p->BuildRenderData(renderWorld);
		}
	}

	for (const auto& b : myShieldManager.GetBlocks())
	{
		if (b && b->IsAlive())
		{
			b->BuildRenderData(renderWorld);
		}
	}

	for (const auto& explosion : myExplosions)
	{
		if (explosion && explosion->IsAlive())
		{
			explosion->BuildRenderData(renderWorld);
		}
	}

	SpriteDrawer& spriteDrawer = Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer();
	for (const auto& rs : renderWorld.sprites)
	{
		if (rs.shared)
		{
			spriteDrawer.Draw(*rs.shared, rs.instance);
		}
	}

	RenderHUD();
	if (myState == EGameState::GameOver)
	{
		RenderGameOverScreen();
	}
}

void GameWorld::RenderPauseScreen()
{
	Vector2ui intRes = Engine::GetInstance()->GetRenderSize();
	Vector2f res{ static_cast<float>(intRes.x), static_cast<float>(intRes.y) };
	if (myTitleText)
	{
		myTitleText->SetText("PAUSED");
		myTitleText->SetPosition({ res.x * 0.5f - 120.0f, res.y * 0.5f });
		myTitleText->Render();
	}
	if (myStatsText)
	{
		myStatsText->SetText("Press P to resume");
		myStatsText->SetPosition({ res.x * 0.5f - 120.0f, res.y * 0.5f + 60.0f });
		myStatsText->Render();
	}
}

void GameWorld::SetInputHandler(CommonUtilities::InputHandler* anInput)
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
	return myScoreManager.GetScore();
}

void GameWorld::AddScore(int aScore)
{
	myScoreManager.AddScore(aScore);
	if (myScoreManager.GetScore() > myScoreManager.GetHighScore())
	{
		myScoreManager.SetHighScore(myScoreManager.GetScore());
	}
	UpdateScoreText();
}

void GameWorld::RegisterEnemyKill(const CommonUtilities::Vector2<float>& pos)
{
	++myEnemiesKilled;
	myEnemyFormation.OnEnemyKilled();
	float factor = static_cast<float>(myEnemiesKilled) / 50.0f;
	if (factor > 1.0f) 
	{
		factor = 1.0f;
	}
	myEnemyFireInterval = 2.0f * (1.0f - factor);
	if (myEnemyFireInterval < 0.6f) 
	{
		myEnemyFireInterval = 0.6f;
	}
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
	std::string scoreStr = std::string("Score: ") + std::to_string(myScoreManager.GetScore());
	std::string hiStr = std::string("Hi: ") + std::to_string(myScoreManager.GetHighScore());
	std::string livesStr = std::string("Lives: ") + std::to_string(myLives);
	Vector2ui intRes = Engine::GetInstance()->GetRenderSize();
	Vector2f res{ static_cast<float>(intRes.x), static_cast<float>(intRes.y) };
	if (myPlayerScoreText) 
	{
		myPlayerScoreText->SetText(scoreStr);
		myPlayerScoreText->SetPosition({ 20.0f, 20.0f });
	}
	if (myHighScoreText) 
	{
		myHighScoreText->SetText(hiStr);
		myHighScoreText->SetPosition({ res.x * 0.5f - 100.0f, 20.0f });
	}
	if (myStatsText) 
	{
		myStatsText->SetText(livesStr);
		myStatsText->SetPosition({ res.x - 200.0f, 20.0f });
	}
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
	myPlayer.ResetPosition();
	myPlayer.Init(*Tga::Engine::GetInstance());

	if (myState == EGameState::Title || myState == EGameState::GameOver)
	{
		myScoreManager.Reset();
		myLives = myConfig.playerLives;
		myEnemiesKilled = 0;
	}
	else
	{
		myEnemiesKilled = 0;
	}

	myLeveledTypes.clear();
	for (const auto& baseType : myTypes.enemyTypes)
	{
		EnemyType leveled = baseType;
		leveled.health += myCurrentLevel - 1;
		leveled.baseSpeed += 10.0f * (myCurrentLevel - 1);
		leveled.baseFireRate *= std::max(0.5f, 1.0f - 0.1f * (myCurrentLevel - 1));
		myLeveledTypes.push_back(leveled);
	}
	std::vector<const EnemyType*> enemyTypes;
	for (const auto& type : myLeveledTypes)
	{
		enemyTypes.push_back(&type);
	}
	myEnemyFormation.ResetForNewWave(myCurrentLevel);
	myEnemyFormation.Init(enemyTypes, { 150.0f, 600.0f });

	myShieldManager.Reset();
	myProjectileManager.Reset();
	myExplosions.clear();
	myMysteryShip.reset();
	myEnemyFireTimer = myEnemyFireInterval;
	myMysterySpawnTimer = static_cast<float>((std::rand() % 1000) / 1000.0) * (myMysterySpawnIntervalMax - myMysterySpawnIntervalMin) + myMysterySpawnIntervalMin;
	myState = EGameState::Playing;
	UpdateScoreText();
}

void GameWorld::UpdatePlaying(float aDeltaTime)
{
	CheckGameOver();

	if (myEnemyFormation.GetEnemies().empty())
	{
		myState = EGameState::LevelCleared;
		return;
	}

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

	myMysterySpawnTimer -= aDeltaTime;
	if (myMysterySpawnTimer <= 0.0f && !myMysteryShip)
	{
		CommonUtilities::Vector2<float> startPos{ -50.0f, 800.0f };
		myMysteryShip = myFactory->CreateMysteryShip(startPos);
		myMysterySpawnTimer = static_cast<float>((std::rand() % 1000) / 1000.0) * (myMysterySpawnIntervalMax - myMysterySpawnIntervalMin) + myMysterySpawnIntervalMin;
	}
	if (myMysteryShip)
	{
		myMysteryShip->Update(aDeltaTime);
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
	myPlayerScoreText->Render();
	myHighScoreText->Render();
	myStatsText->Render();
}

void GameWorld::RenderTitleScreen()
{
	Vector2ui intRes = Engine::GetInstance()->GetRenderSize();
	Vector2f res{ static_cast<float>(intRes.x), static_cast<float>(intRes.y) };
	if (myTitleText)
	{
		myTitleText->SetText("SPACE INVADERS");
		myTitleText->SetPosition({ res.x * 0.5f - 300.0f, res.y * 0.75f });
		myTitleText->Render();
	}
	if (mySubtitleText)
	{
		mySubtitleText->SetText("By: Tarik Bergstrom");
		mySubtitleText->SetPosition({ res.x * 0.5f - 150.0f, res.y * 0.3f });
		mySubtitleText->Render();
	}
	if (myInstructionsText)
	{
		myInstructionsText->SetText("Arrows to move, Space to fire, Enter to start, Esc to quit");
		myInstructionsText->SetPosition({ res.x * 0.5f - 410.0f, res.y * 0.55f });
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
			std::string("  Final score: ") + std::to_string(myScoreManager.GetScore());
		myStatsText->SetText(stats);
		myStatsText->SetPosition({ res.x * 0.5f - 300.0f, res.y * 0.5f });
		myStatsText->Render();
	}
}

void GameWorld::RenderLevelClearedScreen()
{
	Vector2ui intRes = Engine::GetInstance()->GetRenderSize();
	Vector2f res{ static_cast<float>(intRes.x), static_cast<float>(intRes.y) };
	if (myTitleText)
	{
		myTitleText->SetText("LEVEL CLEARED!");
		myTitleText->SetPosition({ res.x * 0.5f - 150.0f, res.y * 0.5f });
		myTitleText->Render();
	}
	if (myStatsText)
	{
		std::string stats = std::string("Press Enter or Space to continue to next level\n") +
			"Current Score: " + std::to_string(myScoreManager.GetScore()) +
			"  Level: " + std::to_string(myCurrentLevel);
		myStatsText->SetText(stats);
		myStatsText->SetPosition({ res.x * 0.5f - 200.0f, res.y * 0.5f + 100.0f });
		myStatsText->Render();
	}
}
