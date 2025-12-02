#include "GameTypes.h"
#include "GameConfig.h"
#include <tge/engine.h>
#include <tge/texture/TextureManager.h>

void GameTypesInitializer::Init(GameTypes& types, Tga::Engine& engine, const GameConfig& cfg)
{
    auto& texMgr = engine.GetTextureManager();

    types.player.moveSpeed = cfg.playerMoveSpeed;
    types.player.startLives = cfg.playerLives;
    types.player.spriteData.myTexture = texMgr.GetTexture(("Sprites/Player.dds"));

    types.playerShot.speed = cfg.playerProjectileSpeed;
    types.playerShot.goesDown = false;
    types.playerShot.spriteData = texMgr.GetTexture("Sprites/PlayerShot.dds");

    // Fill enemyTypes[0..2], enemyShot, shield sprites, etc…
}
