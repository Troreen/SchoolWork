#include "GameManager.h"
#include "Enemy.h"
#include "Room.h"
#include "Door.h"
#include "Direction.h"
#include "CombatComponent.h"
#include <iostream>

namespace
{
    std::string DirectionToString(Direction aDirection)
    {
        switch (aDirection)
        {
        case Direction::DirectionNorth:
            return "North";
        case Direction::DirectionEast:
            return "East";
        case Direction::DirectionSouth:
            return "South";
        case Direction::DirectionWest:
            return "West";
        case Direction::DirectionCount:
            return "Unknown";
        }

        return "Unknown";
    }
}

GameManager::GameManager()
    : myPlayer("Hero", 5, 5, 5),
      myRooms(),
      myCurrentRoom(nullptr),
      myDoors()
{
    myRooms.emplace_back("First Room");
    myRooms.emplace_back("Second Room");
    myRooms.emplace_back("Third Room");
    myRooms.emplace_back("Fourth Room");

    const bool isLocked = true;
    const bool isBreakable = true;

    Door* doorToNorth = new Door(&myRooms[0],
                                 &myRooms[1],
                                 Direction::DirectionNorth,
                                 Direction::DirectionSouth,
                                 !isLocked,
                                 !isBreakable);
    Door* doorToEast = new Door(&myRooms[1],
                                &myRooms[2],
                                Direction::DirectionEast,
                                Direction::DirectionWest,
                                isLocked,
                                !isBreakable);
    Door* doorToSouth = new Door(&myRooms[2],
                                 &myRooms[3],
                                 Direction::DirectionSouth,
                                 Direction::DirectionNorth,
                                 !isLocked,
                                 isBreakable);

    myDoors.push_back(doorToNorth);
    myDoors.push_back(doorToEast);
    myDoors.push_back(doorToSouth);

    myRooms[0].AddDoor(doorToNorth);
    myRooms[1].AddDoor(doorToNorth);
    myRooms[1].AddDoor(doorToEast);
    myRooms[2].AddDoor(doorToEast);
    myRooms[2].AddDoor(doorToSouth);
    myRooms[3].AddDoor(doorToSouth);

    myRooms[1].AddEnemy(Enemy("Goblin", 15, 3, 2));
    myRooms[1].AddEnemy(Enemy("Goblin Archer", 6, 4, 1));
    myRooms[2].AddEnemy(Enemy("Orc", 12, 2, 10));
    myRooms[3].AddEnemy(Enemy("Troll", 20, 1, 15));

    myCurrentRoom = &myRooms[0];
    myCurrentRoom->EnterRoom();
}

GameManager::~GameManager()
{
    for (auto* door : myDoors)
    {
        delete door;
    }
}

void GameManager::Run()
{
    bool isRunning = true;

    while (isRunning)
    {
        DescribeCurrentRoom();
        isRunning = HandleInput();
    }

    std::cout << "\nThanks for playing!\n";
}

bool GameManager::HandleInput()
{
    const PlayerCommand command = myCurrentRoom->HasEnemies()
                                      ? GetCommandWhenEnemies()
                                      : GetCommandInSafeRoom();

    std::system("cls"); 
    return ExecuteCommand(command);
}

GameManager::PlayerCommand GameManager::GetCommandWhenEnemies() const
{
    while (true)
    {
        std::cout << "\nOptions:\n1. Fight enemies\n0. Exit game\n";
        const int choice = ReadInt("Choice: ");

        switch (choice)
        {
        case 1:
            return PlayerCommand::PlayerCommandFight;
        case 0:
            return PlayerCommand::PlayerCommandQuit;
        default:
            std::cout << "Invalid choice, try again.\n";
            break;
        }
    }
}

GameManager::PlayerCommand GameManager::GetCommandInSafeRoom() const
{
    while (true)
    {
        std::cout << "\nOptions:\n1. Move to another room\n2. Inspect room\n0. Exit game\n";
        const int choice = ReadInt("Choice: ");

        switch (choice)
        {
        case 1:
            return PlayerCommand::PlayerCommandMove;
        case 2:
            return PlayerCommand::PlayerCommandInspect;
        case 0:
            return PlayerCommand::PlayerCommandQuit;
        default:
            std::cout << "Invalid choice, try again.\n";
            break;
        }
    }
}

bool GameManager::ExecuteCommand(PlayerCommand aCommand)
{
    switch (aCommand)
    {
    case PlayerCommand::PlayerCommandQuit:
        return false;
    case PlayerCommand::PlayerCommandFight:
        return StartCombat();
    case PlayerCommand::PlayerCommandMove:
        return HandleMove();
    case PlayerCommand::PlayerCommandInspect:
        InspectRoom();
        return true;
    case PlayerCommand::PlayerCommandInvalid:
    case PlayerCommand::PlayerCommandCount:
        std::cout << "Invalid command.\n";
        return true;
    }

    std::cout << "Invalid command.\n";
    return true;
}

bool GameManager::HandleMove()
{
    const std::vector<Door*>& doorsInRoom = myCurrentRoom->GetDoors();

    if (doorsInRoom.empty())
    {
        std::cout << "There are no exits from this room.\n";
        return true;
    }

    std::cout << "\nAvailable exits:\n";
    for (size_t index = 0; index < doorsInRoom.size(); ++index)
    {
        const Door* door = doorsInRoom[index];
        std::cout << index + 1 << ". " << DirectionToString(door->GetDirectionFromRoom(myCurrentRoom));
        if (door->IsLocked())
        {
            std::cout << " (locked";
            if (door->IsBreakableDoor())
            {
                std::cout << ", breakable";
            }
            std::cout << ")";
        }
        std::cout << "\n";
    }

    const int choice = ReadInt("Choose a door: ");
    if (choice <= 0 || choice > static_cast<int>(doorsInRoom.size()))
    {
        std::cout << "Invalid choice.\n";
        return true;
    }

    Door* chosenDoor = doorsInRoom[static_cast<size_t>(choice - 1)];
    if (!chosenDoor->CanPass())
    {
        std::cout << "The door is locked.\n";
        return true;
    }

    myCurrentRoom = chosenDoor->GetOtherRoom(myCurrentRoom);
    myCurrentRoom->EnterRoom();
    return true;
}

void GameManager::InspectRoom() const
{
    std::cout << "\nYou inspect the room carefully.\n";
    std::cout << myCurrentRoom->GetDescription() << "\n";

    if (myCurrentRoom->HasEnemies())
    {
        std::cout << "Enemies present:\n";
        for (const auto& enemy : myCurrentRoom->Enemies())
        {
            std::cout << "- " << enemy.GetName() << "\n";
        }
    }
    else
    {
        const auto& doorsInRoom = myCurrentRoom->GetDoors();
        if (doorsInRoom.empty())
        {
            std::cout << "There are no exits from this room.\n";
        }
        else
        {
            std::cout << "Available exits:\n";
            for (const auto* door : doorsInRoom)
            {
                std::cout << "- " << DirectionToString(door->GetDirectionFromRoom(myCurrentRoom));
                if (door->IsLocked())
                {
                    std::cout << " (locked";
                    if (door->IsBreakableDoor())
                    {
                        std::cout << ", breakable";
                    }
                    std::cout << ")";
                }
                std::cout << "\n";
            }
        }

        if (myCurrentRoom->IsRoomCleared())
        {
            std::cout << "The room is cleared.\n";
        }
    }
}

void GameManager::DescribeCurrentRoom() const
{
    std::cout << "\nYou are in: " << myCurrentRoom->GetName() << "\n";
    std::cout << myCurrentRoom->GetDescription() << "\n";

    if (myCurrentRoom->HasEnemies())
    {
        std::cout << "Enemies present:\n";
        for (const auto& enemy : myCurrentRoom->Enemies())
        {
            std::cout << "- " << enemy.GetName() << "\n";
        }
    }
    else
    {
        const auto& doorsInRoom = myCurrentRoom->GetDoors();
        if (doorsInRoom.empty())
        {
            std::cout << "There are no exits from this room.\n";
        }
        else
        {
            std::cout << "Available exits:\n";
            for (const auto* door : doorsInRoom)
            {
                std::cout << "- " << DirectionToString(door->GetDirectionFromRoom(myCurrentRoom));
                if (door->IsLocked())
                {
                    std::cout << " (locked";
                    if (door->IsBreakableDoor())
                    {
                        std::cout << ", breakable";
                    }
                    std::cout << ")";
                }
                std::cout << "\n";
            }
        }

        if (myCurrentRoom->IsRoomCleared())
        {
            std::cout << "The room is cleared.\n";
        }
    }
}

int GameManager::ReadInt(const std::string& aPrompt) const
{
    while (true)
    {
        std::cout << aPrompt;
        int value = 0;
        if (std::cin >> value)
        {
            std::cin.ignore(100000, '\n');
            return value;
        }

        std::cin.clear();
        std::cin.ignore(100000, '\n');
        std::cout << "Please enter a number.\n";
    }
}

bool GameManager::StartCombat()
{
    CombatComponent combat(myPlayer, myCurrentRoom->Enemies());

    while (combat.GetResult() == CombatComponent::Result::ResultOngoing)
    {
        const std::vector<Enemy>& enemies = combat.GetEnemies();

        if (!enemies.empty())
        {
            std::cout << "\nEnemies in the room:\n";
            for (size_t index = 0; index < enemies.size(); ++index)
            {
                const Enemy& enemy = enemies[index];
                std::cout << index + 1 << ". " << enemy.PrintStats();
                std::cout << " | HP: " << enemy.GetDamagable().GetHealth() << "/" << enemy.GetDamagable().GetMaxHealth() << "\n";
            }
        }

        std::cout << "\nYour stats:\n" << myPlayer.PrintStats();
        std::cout << "\nChoose your action:\n";
        std::cout << "1. Attack\n";
        std::cout << "2. Defend\n";
        std::cout << "3. Use item\n";

        const int actionInput = ReadInt("Choice: ");

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
            std::cout << "Invalid action. Try again.\n";
            continue;
        }

        if (action == CombatComponent::Action::ActionAttack)
        {
            if (enemies.empty())
            {
                std::cout << "There are no enemies to attack.\n";
                continue;
            }

            const int enemyChoice = ReadInt("Choose enemy to attack: ");
            if (enemyChoice <= 0 || enemyChoice > static_cast<int>(enemies.size()))
            {
                std::cout << "Invalid enemy choice.\n";
                continue;
            }

            const size_t enemyIndex = static_cast<size_t>(enemyChoice - 1);
            if (!combat.SelectEnemy(enemyIndex))
            {
                std::cout << "Invalid enemy choice.\n";
                continue;
            }
        }

        combat.PerformPlayerAction(action);

        if (combat.GetResult() != CombatComponent::Result::ResultOngoing)
        {
            break;
        }

        combat.PerformEnemyTurn();
    }

    switch (combat.GetResult())
    {
    case CombatComponent::Result::ResultPlayerWon:
        std::cout << "All enemies defeated!\n";
        myCurrentRoom->SetRoomCleared(true);
        return true;
    case CombatComponent::Result::ResultPlayerLost:
        std::cout << "You have been slain!\n";
        return false;
    case CombatComponent::Result::ResultOngoing:
    case CombatComponent::Result::ResultCount:
        return true;
    }

    return true;
}



