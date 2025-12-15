#include "GameTypes.h"
#include "ConfigLoader.h"
#include <tge/engine.h>
#include <tge/texture/TextureManager.h>
#include <tge/sprite/sprite.h>

void GameTypesInitializer::Init(GameTypes& types, Tga::Engine& engine, const GameConfig& cfg)
{
    auto& texMgr = engine.GetTextureManager();

    static Tga::SpriteSharedData playerShared{};
    playerShared.myTexture = texMgr.GetTexture(("Sprites/player.dds"));

    static Tga::SpriteSharedData playerShotShared{};
    playerShotShared.myTexture = texMgr.GetTexture(("Sprites/shot2.dds"));

    static Tga::SpriteSharedData enemyShotShared{};
    enemyShotShared.myTexture = texMgr.GetTexture(("Sprites/shot1.dds"));

    static Tga::SpriteSharedData enemyShared[3]{};
    enemyShared[0].myTexture = texMgr.GetTexture(("Sprites/enemy1.dds"));
    enemyShared[1].myTexture = texMgr.GetTexture(("Sprites/enemy2.dds"));
    enemyShared[2].myTexture = texMgr.GetTexture(("Sprites/enemy3.dds"));

    static Tga::SpriteSharedData mysteryShared{};
    mysteryShared.myTexture = texMgr.GetTexture(("Sprites/mysteryship.dds"));

    static Tga::SpriteSharedData shieldShared[4]{};
    shieldShared[0].myTexture = texMgr.GetTexture(("Sprites/shelter1.dds"));
    shieldShared[1].myTexture = texMgr.GetTexture(("Sprites/shelter2.dds"));
    shieldShared[2].myTexture = texMgr.GetTexture(("Sprites/shelter3.dds"));
    shieldShared[3].myTexture = texMgr.GetTexture(("Sprites/shelter4.dds"));

    static Tga::SpriteSharedData explosionFrame1{};
    explosionFrame1.myTexture = texMgr.GetTexture(("Sprites/explosion1.dds"));
    static Tga::SpriteSharedData explosionFrame2{};
    explosionFrame2.myTexture = texMgr.GetTexture(("Sprites/explosion2.dds"));
    static Tga::SpriteSharedData explosionFrame3{};
    explosionFrame3.myTexture = texMgr.GetTexture(("Sprites/explosion3.dds"));

    types.player.moveSpeed = cfg.playerMoveSpeed;
    types.player.startLives = cfg.playerLives;
    types.player.spriteData = &playerShared;

    types.playerShot.speed = cfg.playerProjectileSpeed;
    types.playerShot.goesDown = false;
    types.playerShot.spriteData = &playerShotShared;

    types.enemyShot.speed = cfg.enemyProjectileSpeed;
    types.enemyShot.goesDown = true;
    types.enemyShot.spriteData = &enemyShotShared;

    types.enemyTypes[0].id = "grunt";
    types.enemyTypes[0].health = 1;
    types.enemyTypes[0].score = 100;
    types.enemyTypes[0].baseSpeed = cfg.enemyBaseSpeed;
    types.enemyTypes[0].baseFireRate = 1.5f;
    types.enemyTypes[0].spriteData = &enemyShared[0];

    types.enemyTypes[1].id = "soldier";
    types.enemyTypes[1].health = 1;
    types.enemyTypes[1].score = 150;
    types.enemyTypes[1].baseSpeed = cfg.enemyBaseSpeed * 1.05f;
    types.enemyTypes[1].baseFireRate = 1.7f;
    types.enemyTypes[1].spriteData = &enemyShared[1];

    types.enemyTypes[2].id = "tank";
    types.enemyTypes[2].health = 2;
    types.enemyTypes[2].score = 300;
    types.enemyTypes[2].baseSpeed = cfg.enemyBaseSpeed * 0.9f;
    types.enemyTypes[2].baseFireRate = 1.9f;
    types.enemyTypes[2].spriteData = &enemyShared[2];

    types.mystery.speed = cfg.enemyBaseSpeed * 0.8f;
    types.mystery.health = 2;
    types.mystery.score = 2000;
    types.mystery.spriteData = &mysteryShared;

    types.shield.maxHits = 4;
    types.shield.damageSprites[0] = &shieldShared[0];
    types.shield.damageSprites[1] = &shieldShared[1];
    types.shield.damageSprites[2] = &shieldShared[2];
    types.shield.damageSprites[3] = &shieldShared[3];

    types.explosion.duration = 0.4f;
    types.explosion.frames[0] = &explosionFrame1;
    types.explosion.frames[1] = &explosionFrame2;
    types.explosion.frames[2] = &explosionFrame3;
}
