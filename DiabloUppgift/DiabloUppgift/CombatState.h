#pragma once

#include <memory>

#include "IGameState.h"

class CombatComponent;

class CombatState : public IGameState
{
public:
    explicit CombatState(GameContext& aContext);

    void OnEnter() override;
    void Update(StateStack& aStateStack) override;

private:
    bool AreAllRoomsCleared() const;
    void HandleCompletedCombat(StateStack& aStateStack);
    int ReadInt(const std::string& aPrompt) const;

    std::unique_ptr<CombatComponent> myCombat;
};
