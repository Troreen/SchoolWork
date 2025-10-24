#pragma once

#include <cstddef>
#include <array>

#include "EnemyTypes.h"

class Enemy;

class EnemyFactory
{
public:
    EnemyFactory();
    ~EnemyFactory();

    void Initialize();
    Enemy Make(EnemyId anId) const;

    const EnemyType& GetType(EnemyId anId) const;

private:
    static constexpr size_t kEnemyTypeCount = 3; // Goblin, Orc, Troll
    std::array<EnemyType, kEnemyTypeCount> myTypes;
    bool myInitialized = false;
};

const EnemyFactory& GetEnemyFactory();
