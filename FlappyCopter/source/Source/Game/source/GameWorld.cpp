#include "GameWorld.h"

#include "tge/graphics/GraphicsEngine.h"
#include "tge/drawers/SpriteDrawer.h"
#include "tge/texture/TextureManager.h"
#include "tge/drawers/DebugDrawer.h"
#include "tge/engine.h"
#include "tge/text/Text.h"

#include <Windows.h>
#include <cmath>
#include <algorithm>
#include <cstdlib>

#include <InputHandler.h>
#include "Obstacle.h"
#include "ConfigLoader.h"

using namespace Tga;

namespace
{
    constexpr float kMetersPerPixel = 1.0f / 10.0f;

    constexpr float kBaseScrollSpeed = 300.0f;
    constexpr float kSpeedIncreaseFactor = 0.05f;
    constexpr float kMaxScrollSpeed = 2000.0f;

    static float gScrollSpeedScale = 1.0f;

    constexpr float kObstacleGapPixels = 32.0f;

    constexpr float kObstacleWidth = 64.0f;
    constexpr float kMinObstacleHeight = 32.0f;
    constexpr float kMaxObstacleHeight = 120.0f;

    constexpr float kWaveAmplitudeFactor = 0.25f;
    constexpr float kNoiseAmplitude = 20.0f;
    constexpr float kWaveSpeedBase = 0.22f;
    constexpr float kWaveSpeedMin = 0.18f;
    constexpr float kWaveSpeedMax = 0.26f;

    constexpr float kPi = 3.141f;
    constexpr float kTwoPi = 6.282f;

    constexpr int   kMiddleObstacleChance = 10;
    constexpr float kMinMiddleGapMultiplier = 2.0f;
    constexpr float kMinMiddleHeight = 100.0f;
    constexpr float kMiddleHeightRandomExtra = 150.0f;

    constexpr float kScorePosXFactor = 0.46f;
    constexpr float kScorePosYFactor = 0.05f;
    constexpr float kGameOverPosXFactor = 0.25f;
    constexpr float kGameOverPosYFactor = 0.40f;

    static std::string gGameOverMessage;
    static std::string gScoreLabel;
    static std::string gScoreSuffix;

    float Clamp(float value, float min, float max)
    {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    float ComputeScrollSpeed(float distanceTravelled)
    {
        float speed = kBaseScrollSpeed + distanceTravelled * kSpeedIncreaseFactor;
        return Clamp(speed, kBaseScrollSpeed, kMaxScrollSpeed);
    }

    float RandomRange(float min, float max)
    {
        const float t = static_cast<float>(rand()) / RAND_MAX;
        return min + t * (max - min);
    }
}

GameWorld::GameWorld()
    : myHelicopter(nullptr)
    , myInput(nullptr)
    , myPlayerScore(0)
    , myState(GameState::Title)
    , myDistanceTravelled(0.0f)
    , myDistanceSinceLastObstacle(0.0f)
{
}

GameWorld::~GameWorld() = default;

void GameWorld::Init()
{
    Engine& engine = *Engine::GetInstance();

    const GameConfig cfg = ConfigLoader::Load("config.json");

    myHelicopter = std::make_unique<Helicopter>();
    myHelicopter->ApplyConfig(cfg);
    myHelicopter->Init(engine);

    gScrollSpeedScale = (cfg.scrollSpeedScale > 0.1f) ? cfg.scrollSpeedScale : 1.0f;

    myPlayerScoreText = std::make_unique<Text>("Text/arial.ttf", FontSize_36);
    myGameOverText = std::make_unique<Text>("Text/arial.ttf", FontSize_48);

    myPlayerScoreText->SetColor({ 1, 1, 1, 1 });
    myGameOverText->SetColor({ 1, 0.9f, 0.2f, 1 });

    const Vector2ui rsz = engine.GetRenderSize();
    const Vector2f  res{ static_cast<float>(rsz.x), static_cast<float>(rsz.y) };

    myPlayerScoreText->SetPosition({ res.x * kScorePosXFactor,    res.y * kScorePosYFactor });
    myGameOverText->SetPosition({ res.x * kGameOverPosXFactor, res.y * kGameOverPosYFactor });

    myTitleStateTexts.clear();
    myPlayingStateTexts.clear();
    myGameOverStateTexts.clear();

    {
        auto addTextTo = [&](const TextElement& e, std::vector<std::unique_ptr<Text>>& dst, FontSize size)
            {
                auto t = std::make_unique<Text>("Text/arial.ttf", size);
                t->SetColor({ 1,1,1,1 });
                t->SetText(e.text);
                t->SetPosition({ e.x, e.y });
                dst.emplace_back(std::move(t));
            };

        auto isequal = [](const std::string& s, const char* a, const char* b = nullptr, const char* c = nullptr)
            {
                if (_stricmp(s.c_str(), a) == 0) return true;
                if (b && _stricmp(s.c_str(), b) == 0) return true;
                if (c && _stricmp(s.c_str(), c) == 0) return true;
                return false;
            };

        auto findByState = [&](const std::string& state) -> const TextElement*
            {
                for (const auto& e : cfg.textElements)
                {
                    if (e.state == state)
                    {
                        return &e;
                    }
                }
                return nullptr;
            };

        for (const auto& e : cfg.textElements)
        {
            if (isequal(e.state, "title", "start_menu"))
            {
                addTextTo(e, myTitleStateTexts, FontSize_48);
            }
            else if (isequal(e.state, "playing", "in_game", "hud"))
            {
                addTextTo(e, myPlayingStateTexts, FontSize_36);
            }
            else if (isequal(e.state, "game_over"))
            {
                addTextTo(e, myGameOverStateTexts, FontSize_48);
            }
        }

        if (const TextElement* scoreElem = findByState("score"))
        {
            if (myPlayerScoreText)
            {
                myPlayerScoreText->SetPosition({ scoreElem->x, scoreElem->y });
            }
            if (!scoreElem->text.empty())
            {
                gScoreLabel = scoreElem->text;
            }
        }

        if (const TextElement* scoreSuf = findByState("score_suffix"))
        {
            if (!scoreSuf->text.empty())
            {
                gScoreSuffix = scoreSuf->text;
            }
        }

        if (const TextElement* gameOverElem = findByState("game_over_message"))
        {
            if (!gameOverElem->text.empty())
            {
                gGameOverMessage = gameOverElem->text;
            }
            myGameOverText->SetPosition({ gameOverElem->x, gameOverElem->y });
        }
    }

    myTitleTexts.clear();
    for (const auto& tt : cfg.titleTexts)
    {
        auto t = std::make_unique<Text>("Text/arial.ttf", FontSize_48);
        t->SetColor({ 1, 1, 1, 1 });
        t->SetText(tt.text);
        t->SetPosition({ tt.x, tt.y });
        myTitleTexts.emplace_back(std::move(t));
    }

    myTitleImages.clear();
    for (const auto& ti : cfg.titleImages)
    {
        TitleSprite ts{};
        ts.shared.myTexture = engine.GetTextureManager().GetTexture(ti.path.c_str());
        if (ts.shared.myTexture)
        {
            ts.inst.mySize = ts.shared.myTexture->CalculateTextureSize();
        }
        else
        {
            ts.inst.mySize = { 128.0f, 128.0f };
        }
        ts.inst.myPivot = { 0.5f, 0.5f };
        ts.inst.myPosition = { ti.x, ti.y };
        ts.inst.myColor = { 1,1,1,1 };
        myTitleImages.emplace_back(std::move(ts));
    }

    myGameOverText->SetText("");

    myObstacles.clear();
    myDistanceTravelled = 0.0f;
    myDistanceSinceLastObstacle = 0.0f;
    myPlayerScore = 0;
    myState = GameState::Title;

    UpdateScoreText();
}

void GameWorld::StartGame()
{
    myObstacles.clear();
    myDistanceTravelled = 0.0f;
    myDistanceSinceLastObstacle = 0.0f;
    myPlayerScore = 0;
    if (myGameOverText) myGameOverText->SetText("");
    if (myHelicopter) myHelicopter->ResetPosition();
    myState = GameState::Playing;
    UpdateScoreText();
}

void GameWorld::Update(float aTimeDelta)
{
    if (!myInput)
    {
        return;
    }

    if (myState == GameState::Title)
    {
        if (myInput->IsKeyPressed(VK_RETURN))
        {
            StartGame();
        }
        return;
    }

    if (myState == GameState::GameOver)
    {
        if (myInput->IsKeyPressed(VK_RETURN))
        {
            StartGame();
        }
        return;
    }

    if (myInput->IsKeyPressed(VK_ESCAPE))
    {
        TriggerGameOver();
        return;
    }

    myHelicopter->SetInput(myInput);
    myHelicopter->Update(aTimeDelta);

    const float halfH = 0.5f * myHelicopter->GetSize().y;

    if (myHelicopter->GetPosition().y - halfH <= 0.0f)
    {
        TriggerGameOver();
        return;
    }

    const float currentSpeed = gScrollSpeedScale * ComputeScrollSpeed(myDistanceTravelled);

    for (auto& obs : myObstacles)
    {
        obs.SetSpeed(currentSpeed);
        obs.Update(aTimeDelta);
    }

    myObstacles.erase(
        std::remove_if(
            myObstacles.begin(), myObstacles.end(),
            [](const Obstacle& obs) { return obs.IsOffScreen(); }),
        myObstacles.end()
    );

    const float distanceThisFrame = currentSpeed * aTimeDelta;
    myDistanceTravelled += distanceThisFrame;
    myDistanceSinceLastObstacle += distanceThisFrame;

    while (myDistanceSinceLastObstacle >= kObstacleGapPixels)
    {
        SpawnObstacle(currentSpeed);
        myDistanceSinceLastObstacle -= kObstacleGapPixels;
    }

    UpdateScoreText();

    for (const auto& obs : myObstacles)
    {
        if (obs.CheckCollision(*myHelicopter))
        {
            TriggerGameOver();
            return;
        }
    }
}

void GameWorld::Render()
{
    Engine& engine = *Engine::GetInstance();
    SpriteDrawer& spriteDrawer = engine.GetGraphicsEngine().GetSpriteDrawer();

    if (myState == GameState::Title)
    {
        for (const auto& ts : myTitleImages)
        {
            spriteDrawer.Draw(ts.shared, ts.inst);
        }
        for (const auto& t : myTitleTexts)
        {
            if (t) t->Render();
        }
        for (const auto& t : myTitleStateTexts)
        {
            if (t) t->Render();
        }
        return;
    }

    if (myHelicopter)
    {
        myHelicopter->Render(spriteDrawer);
    }

    for (auto& obs : myObstacles)
    {
        obs.Render(spriteDrawer);
    }

    for (const auto& t : myPlayingStateTexts)
    {
        if (t) t->Render();
    }

    if (myPlayerScoreText)
    {
        myPlayerScoreText->Render();
    }

    if (myState == GameState::GameOver)
    {
        for (const auto& t : myGameOverStateTexts)
        {
            if (t) t->Render();
        }

        if (myGameOverText)
        {
            myGameOverText->Render();
        }
    }
}

void GameWorld::SetInputHandler(InputHandler* anInput)
{
    myInput = anInput;
}

void GameWorld::UpdateScoreText()
{
    if (myPlayerScoreText)
    {
        const int meters = static_cast<int>(myDistanceTravelled * kMetersPerPixel);
        myPlayerScoreText->SetText(gScoreLabel + std::to_string(meters) + gScoreSuffix);
    }
}

void GameWorld::SpawnObstacle(float aSpeed)
{
    Engine& engine = *Engine::GetInstance();
    const Vector2ui rsz = engine.GetRenderSize();
    const float screenW = static_cast<float>(rsz.x);
    const float screenH = static_cast<float>(rsz.y);

    const float xPos = screenW + kObstacleWidth * 0.5f;

    static float phase = 0.0f;
    static float waveSpeed = kWaveSpeedBase;

    const float amplitude = (kMaxObstacleHeight - kMinObstacleHeight) * kWaveAmplitudeFactor;
    const float baseHeight = (kMinObstacleHeight + kMaxObstacleHeight) * 0.5f;
    const float noise = RandomRange(-0.5f * kNoiseAmplitude, 0.5f * kNoiseAmplitude);

    float topHeight = baseHeight + amplitude * std::sin(phase) + noise;
    float bottomHeight = baseHeight + amplitude * std::sin(phase + kPi) + noise;

    topHeight = Clamp(topHeight, kMinObstacleHeight, kMaxObstacleHeight);
    bottomHeight = Clamp(bottomHeight, kMinObstacleHeight, kMaxObstacleHeight);

    phase += waveSpeed;
    if (phase > kTwoPi)
    {
        phase -= kTwoPi;
    }

    if (rand() % 50 == 0)
    {
        waveSpeed = RandomRange(kWaveSpeedMin, kWaveSpeedMax);
    }

    Obstacle topObs;
    topObs.Init(engine, { xPos, topHeight * 0.5f });
    topObs.SetSize({ kObstacleWidth, topHeight });
    topObs.SetSpeed(aSpeed);
    myObstacles.push_back(topObs);

    Obstacle botObs;
    botObs.Init(engine, { xPos, screenH - bottomHeight * 0.5f });
    botObs.SetSize({ kObstacleWidth, bottomHeight });
    botObs.SetSpeed(aSpeed);
    myObstacles.push_back(botObs);

    if (rand() % kMiddleObstacleChance == 0 && myHelicopter)
    {
        const float playerHeight = myHelicopter->GetSize().y;
        const float minMiddleGap = playerHeight * kMinMiddleGapMultiplier;
        const float maxMiddleHeight = screenH - minMiddleGap;

        float middleHeight = RandomRange(
            kMinMiddleHeight,
            kMinMiddleHeight + kMiddleHeightRandomExtra
        );
        middleHeight = Clamp(middleHeight, kMinMiddleHeight, maxMiddleHeight);

        const float middleY = screenH * 0.5f;

        Obstacle midObs;
        midObs.Init(engine, { xPos, middleY });
        midObs.SetSize({ kObstacleWidth, middleHeight });
        midObs.SetSpeed(aSpeed);
        myObstacles.push_back(midObs);
    }
}

void GameWorld::ResetGame()
{
    myPlayerScore = 0;
    myDistanceTravelled = 0.0f;
    myDistanceSinceLastObstacle = 0.0f;

    if (myGameOverText)
    {
        myGameOverText->SetText("");
    }

    if (myHelicopter)
    {
        myHelicopter->ResetPosition();
    }

    myObstacles.clear();
    UpdateScoreText();
    myState = GameState::Title;
}

void GameWorld::TriggerGameOver()
{
    myState = GameState::GameOver;
    if (myGameOverText)
    {
        myGameOverText->SetText(gGameOverMessage.c_str());
    }
}