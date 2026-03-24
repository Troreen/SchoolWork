#pragma once

#include "IGameState.h"

class GameplayState : public IGameState
{
public:
    explicit GameplayState(GameContext& aContext);

    void Update(StateStack& aStateStack) override;

private:
    enum class Command
    {
        Quit,
        Move,
        Inspect,
        Inventory,
        Drop,
        Stats,
        Pickup,
        OpenChest,
        Invalid
    };

    Command GetCommandInSafeRoom() const;
    bool ExecuteCommand(Command aCommand, StateStack& aStateStack);

    bool HandleMove();
    void InspectRoom() const;
    void DropItems();
    void PickUpItems();
    void OpenChest();

    int ReadInt(const std::string& aPrompt) const;
};
