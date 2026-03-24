#include "CombatState.h"

#include <iomanip>
#include <iostream>

#include "CombatComponent.h"
#include "ConsoleView.h"
#include "Enemy.h"
#include "Room.h"
#include "StateId.h"
#include "StateStack.h"

CombatState::CombatState(GameContext& aContext)
    : IGameState(aContext)
    , myCombat()
{
}

void CombatState::OnEnter()
{
    IGameState::OnEnter();
    myCombat = std::make_unique<CombatComponent>(myContext.player, myContext.currentRoom->Enemies(), *myContext.currentRoom);
}

void CombatState::Update(StateStack& aStateStack)
{
    if (myCombat == nullptr)
    {
        myCombat = std::make_unique<CombatComponent>(myContext.player, myContext.currentRoom->Enemies(), *myContext.currentRoom);
    }

    if (myCombat->GetResult() != CombatComponent::Result::ResultOngoing)
    {
        HandleCompletedCombat(aStateStack);
        return;
    }

    const std::vector<Enemy>& enemies = myCombat->GetEnemies();
    std::vector<std::pair<int, std::string>> combatOptions = {
        {1, "Attack"},
        {2, "Defend"},
        {3, "Use item"},
        {4, "Open inventory"},
        {0, "Exit game"}
    };

    ConsoleView::ClearScreen();
    std::cout << "Combat timer: " << std::fixed << std::setprecision(1) << GetElapsedSeconds() << " s\n";
    ConsoleView::ShowCombatLayout(*myContext.currentRoom, myContext.player, enemies, combatOptions);

    const int actionInput = ReadInt("Choice: ");
    if (actionInput == 0)
    {
        myContext.endMessage = "Combat exited after " + std::to_string(static_cast<int>(GetElapsedSeconds())) + " second(s).";
        aStateStack.PushState(StateId::End);
        return;
    }

    if (actionInput == 4)
    {
        aStateStack.PushState(StateId::InventoryMenu);
        return;
    }

    CombatComponent::Action action = CombatComponent::Action::ActionAttack;
    switch (actionInput)
    {
    case 1:
        action = CombatComponent::Action::ActionAttack;
        break;
    case 2:
        action = CombatComponent::Action::ActionDefend;
        break;
    case 3:
        action = CombatComponent::Action::ActionUseItem;
        break;
    default:
        ConsoleView::ShowInvalidChoice();
        return;
    }

    if (action == CombatComponent::Action::ActionAttack)
    {
        if (enemies.empty())
        {
            ConsoleView::ShowNoEnemiesToAttack();
            return;
        }

        const int enemyChoice = ReadInt("Choose enemy to attack: ");
        if (enemyChoice <= 0 || enemyChoice > static_cast<int>(enemies.size()))
        {
            ConsoleView::ShowInvalidChoice();
            return;
        }

        const size_t enemyIndex = static_cast<size_t>(enemyChoice - 1);
        if (!myCombat->SelectEnemy(enemyIndex))
        {
            ConsoleView::ShowInvalidChoice();
            return;
        }
    }

    const bool actionPerformed = myCombat->PerformPlayerAction(action);
    if (!actionPerformed)
    {
        return;
    }

    myContext.player.AdvanceActiveEnchantments();

    if (myCombat->GetResult() == CombatComponent::Result::ResultOngoing)
    {
        myCombat->PerformEnemyTurn();

        if (myCombat->GetResult() == CombatComponent::Result::ResultOngoing)
        {
            std::cout << "Press Enter to continue...\n";
            ConsoleView::Pause();
        }
    }

    if (myCombat->GetResult() != CombatComponent::Result::ResultOngoing)
    {
        HandleCompletedCombat(aStateStack);
    }
}

bool CombatState::AreAllRoomsCleared() const
{
    for (const Room& room : myContext.rooms)
    {
        if (room.HasEnemies())
        {
            return false;
        }
    }

    return true;
}

void CombatState::HandleCompletedCombat(StateStack& aStateStack)
{
    switch (myCombat->GetResult())
    {
    case CombatComponent::Result::ResultPlayerWon:
        ConsoleView::ShowCombatVictory();
        myContext.currentRoom->SetRoomCleared(true);
        if (AreAllRoomsCleared())
        {
            ConsoleView::ShowAllEnemiesCleared();
            ConsoleView::Pause();
            myContext.endMessage = "All enemies defeated in " + std::to_string(static_cast<int>(GetElapsedSeconds())) + " combat second(s).";
            aStateStack.PushState(StateId::End);
            return;
        }

        ConsoleView::Pause();
        aStateStack.PopState();
        return;

    case CombatComponent::Result::ResultPlayerLost:
        ConsoleView::ShowCombatDefeat();
        ConsoleView::Pause();
        myContext.endMessage = "You were defeated after " + std::to_string(static_cast<int>(GetElapsedSeconds())) + " combat second(s).";
        aStateStack.PushState(StateId::End);
        return;

    case CombatComponent::Result::ResultOngoing:
    case CombatComponent::Result::ResultCount:
        return;
    }
}

int CombatState::ReadInt(const std::string& aPrompt) const
{
    while (true)
    {
        std::cout << aPrompt;
        int value = 0;
        if (std::cin >> value)
        {
            std::cin.ignore(1000000, '\n');
            return value;
        }

        std::cin.clear();
        std::cin.ignore(1000000, '\n');
        std::cout << "Please enter a number.\n";
    }
}
