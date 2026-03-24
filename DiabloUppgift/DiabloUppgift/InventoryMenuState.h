#pragma once

#include "IGameState.h"

class InventoryMenuState : public IGameState
{
public:
    explicit InventoryMenuState(GameContext& aContext);

    void Update(StateStack& aStateStack) override;

private:
    bool EquipFromInventory(size_t anIndex);
    int ReadInt(const std::string& aPrompt) const;
};
