#include "GameWorld.h"

#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>
#include <tge/engine.h>
#include <tge/text/Text.h>
#include <Windows.h>

#include <InputHandler.h>
#include <Timer.h>

using namespace Tga;

GameWorld::GameWorld()
{
	myBall = nullptr;
	myInput = nullptr;
	myPlayerPaddle = nullptr;
	myAIPaddle = nullptr;
}

GameWorld::~GameWorld()
{
	delete myBall;
	delete myPlayerPaddle;
	delete myAIPaddle;
}

void GameWorld::Init()
{
	Engine& engine = *Engine::GetInstance();
	myBall = new Ball();
	myBall->Init(engine);

	myPlayerPaddle = new Paddle();
	myPlayerPaddle->Init(engine, PaddleSide::Left);

	myAIPaddle = new Paddle();
	myAIPaddle->Init(engine, PaddleSide::Right);
	myAIPaddle->SetIsAI(true);

	myPlayerScoreText = std::make_unique<Text>("Text/arial.ttf", FontSize_36);
	myAIScoreText = std::make_unique<Text>("Text/arial.ttf", FontSize_36);
	myGameOverText = std::make_unique<Text>("Text/arial.ttf", FontSize_48);

	myPlayerScoreText->SetColor({ 1,1,1,1 });
	myAIScoreText->SetColor({ 1,1,1,1 });
	myGameOverText->SetColor({ 1,0.9f,0.2f,1 });

	const Vector2ui rsz = engine.GetRenderSize();
	const Vector2f res = { (float)rsz.x,(float)rsz.y };
	myPlayerScoreText->SetPosition({ res.x * 0.46f, res.y * 0.05f });
	myAIScoreText->SetPosition({ res.x * 0.54f, res.y * 0.05f });
	myGameOverText->SetPosition({ res.x * 0.25f, res.y * 0.40f });

	UpdateScoreText();
	myGameOverText->SetText(""); 
}

void GameWorld::HandlePaddleBallCollision()
{
	const Vector2f ballPos = myBall->GetPosition();
	const float ballRadius = myBall->GetRadius();

	auto testPaddle = [&](Paddle* paddle)
	{
		const Vector2f pPos = paddle->GetPosition();
		const Vector2f pSize = paddle->GetSize();
		const float halfW = pSize.x * 0.5f;
		const float halfH = pSize.y * 0.5f;
		if (ballPos.x + ballRadius > pPos.x - halfW && ballPos.x - ballRadius < pPos.x + halfW &&
			ballPos.y + ballRadius > pPos.y - halfH && ballPos.y - ballRadius < pPos.y + halfH)
		{
			myBall->ReflectX();
			float diff = (ballPos.y - pPos.y) / halfH;
			myBall->AddVerticalInfluence(diff);
		}
	};

	testPaddle(myPlayerPaddle);
	testPaddle(myAIPaddle);
}

void GameWorld::Update(float aTimeDelta)
{
	if (myInput)
	{
		// Restart if game over and Enter pressed
		if (myIsGameOver && myInput->IsKeyPressed(VK_RETURN))
		{
			ResetGame();
			return;
		}
		myPlayerPaddle->SetInput(myInput);
	}

	if (myIsGameOver)
	{
		// Freeze everything except restart input
		return;
	}

	myAIPaddle->SetAiFollowY(myBall->GetPosition().y);

	myPlayerPaddle->Update(aTimeDelta);
	myAIPaddle->Update(aTimeDelta);
	myBall->Update(aTimeDelta);

	HandlePaddleBallCollision();

	const float x = myBall->GetPosition().x;
	const float radius = myBall->GetRadius();
	const float screenW = myBall->GetScreenResolution().x;
	if (x < -radius)
	{
		myAIScore++;
		UpdateScoreText();
		CheckGameOver();
		// Reset paddles to middle after score
		myPlayerPaddle->ResetPosition();
		myAIPaddle->ResetPosition();
		if (!myIsGameOver) myBall->Reset();
	}
	else if (x > screenW + radius)
	{
		myPlayerScore++;
		UpdateScoreText();
		CheckGameOver();
		// Reset paddles to middle after score
		myPlayerPaddle->ResetPosition();
		myAIPaddle->ResetPosition();
		if (!myIsGameOver) myBall->Reset();
	}
}

void GameWorld::Render()
{
	auto& engine = *Engine::GetInstance();
	SpriteDrawer& spriteDrawer(engine.GetGraphicsEngine().GetSpriteDrawer());

	myBall->Render(spriteDrawer);
	myPlayerPaddle->Render(spriteDrawer);
	myAIPaddle->Render(spriteDrawer);

	if (myPlayerScoreText) myPlayerScoreText->Render();
	if (myAIScoreText) myAIScoreText->Render();
	if (myIsGameOver && myGameOverText) myGameOverText->Render();
}

void GameWorld::SetInputHandler(InputHandler* anInput)
{
	myInput = anInput;
}

void GameWorld::UpdateScoreText()
{
	if (myPlayerScoreText) myPlayerScoreText->SetText(std::to_string(myPlayerScore));
	if (myAIScoreText) myAIScoreText->SetText(std::to_string(myAIScore));
}

void GameWorld::CheckGameOver()
{
	if (myPlayerScore >= myMaxScore || myAIScore >= myMaxScore)
	{
		myIsGameOver = true;
		if (myPlayerScore > myAIScore)
			myGameOverText->SetText("Player Wins! Press Enter to restart");
		else
			myGameOverText->SetText("AI Wins! Press Enter to restart");
	}
}

void GameWorld::ResetGame()
{
	myPlayerScore = 0;
	myAIScore = 0;
	myIsGameOver = false;
	UpdateScoreText();
	if (myGameOverText) myGameOverText->SetText("");

	// Reset positions
	if (myPlayerPaddle) myPlayerPaddle->ResetPosition();
	if (myAIPaddle) myAIPaddle->ResetPosition();
	if (myBall) myBall->Reset();
}