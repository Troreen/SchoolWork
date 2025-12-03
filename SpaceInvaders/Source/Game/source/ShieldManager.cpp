#include "ShieldManager.h"

#include <Vector2.hpp>
#include "GameTypes.h"
#include "ShieldBlock.h"

void ShieldManager::Init(const ShieldType& type, int numShields)
{
    myType = &type;
    myNumShields = numShields;
    Reset();
}

void ShieldManager::Reset()
{
    myBlocks.clear();
    if (!myType || myNumShields <= 0)
    {
        return;
    }

    const float screenWidth = 1600.0f;
    const float spacing = screenWidth / static_cast<float>(myNumShields + 1);
    const float baseY = 250.0f;

    for (int i = 0; i < myNumShields; ++i)
    {
        CommonUtilities::Vector2<float> pos;
        pos.x = spacing * static_cast<float>(i + 1);
        pos.y = baseY;

        myBlocks.push_back(std::make_unique<ShieldBlock>(*myType, pos));
    }
}

void ShieldManager::Update(float dt)
{
    for (auto& block : myBlocks)
    {
        if (block->IsAlive())
        {
            block->Update(dt);
        }
    }
}

const std::vector<std::unique_ptr<ShieldBlock>>& ShieldManager::GetBlocks() const
{
    return myBlocks;
}
