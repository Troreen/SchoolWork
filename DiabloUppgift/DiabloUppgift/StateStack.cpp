#include "StateStack.h"

#include <stdexcept>

#include "IGameState.h"

StateStack::StateStack()
    : myStates()
    , myFactories()
    , myPendingChanges()
{
}

void StateStack::RegisterState(StateId aStateId, StateFactory aFactory)
{
    myFactories[aStateId] = std::move(aFactory);
}

void StateStack::PushState(StateId aStateId)
{
    myPendingChanges.push_back({ PendingAction::Push, aStateId });
}

void StateStack::PopState()
{
    myPendingChanges.push_back({ PendingAction::Pop, StateId::MainMenu });
}

void StateStack::ClearStates()
{
    myPendingChanges.push_back({ PendingAction::Clear, StateId::MainMenu });
}

void StateStack::Update()
{
    ApplyPendingChanges();

    if (myStates.empty())
    {
        return;
    }

    myStates.back()->Update(*this);
    ApplyPendingChanges();
}

bool StateStack::Empty() const
{
    return myStates.empty();
}

void StateStack::ApplyPendingChanges()
{
    for (const PendingChange& change : myPendingChanges)
    {
        switch (change.action)
        {
        case PendingAction::Push:
        {
            auto factoryIt = myFactories.find(change.stateId);
            if (factoryIt == myFactories.end())
            {
                throw std::runtime_error("State requested but no factory registered.");
            }

            std::unique_ptr<IGameState> state = factoryIt->second();
            state->OnEnter();
            myStates.push_back(std::move(state));
            break;
        }
        case PendingAction::Pop:
        {
            if (!myStates.empty())
            {
                myStates.back()->OnExit();
                myStates.pop_back();
            }
            break;
        }
        case PendingAction::Clear:
        {
            while (!myStates.empty())
            {
                myStates.back()->OnExit();
                myStates.pop_back();
            }
            break;
        }
        }
    }

    myPendingChanges.clear();
}
