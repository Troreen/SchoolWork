#pragma once

#include <functional>
#include <map>
#include <memory>
#include <vector>

#include "StateId.h"

class IGameState;

class StateStack
{
public:
    using StateFactory = std::function<std::unique_ptr<IGameState>()>;

    StateStack();

    void RegisterState(StateId aStateId, StateFactory aFactory);

    void PushState(StateId aStateId);
    void PopState();
    void ClearStates();

    void Update();
    bool Empty() const;

private:
    enum class PendingAction
    {
        Push,
        Pop,
        Clear
    };

    struct PendingChange
    {
        PendingAction action;
        StateId stateId;
    };

    void ApplyPendingChanges();

    std::vector<std::unique_ptr<IGameState>> myStates;
    std::map<StateId, StateFactory> myFactories;
    std::vector<PendingChange> myPendingChanges;
};
