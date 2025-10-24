#pragma once

#include <cstddef>

#include "ChestTypes.h"

class Chest;

struct ChestFactory
{
    const ChestSpec* specs = nullptr;
    size_t specCount = 0;

    Chest Make(ChestId id) const;
};

const ChestFactory& GetChestFactory();
