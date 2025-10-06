#pragma once

#include <cstddef>

#include "EnemyTypes.h"

class Enemy;

struct EnemyFactory
{
    const EnemySpec* specs = nullptr;
    size_t specCount = 0;

    Enemy Make(EnemyId id) const;
};

const EnemyFactory& GetEnemyFactory();
