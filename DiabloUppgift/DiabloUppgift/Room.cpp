#include "Room.h"
#include "Door.h"
#include <iostream>

Room::Room(const std::string& aName)
    : myEnemies(),
      myIsPlayerInRoom(false),
      myIsRoomCleared(false),
      myName(aName),
      myDescription(),
      myDoors()
{
}

std::vector<Enemy>& Room::Enemies()
{
    return myEnemies;
}

const std::vector<Enemy>& Room::Enemies() const
{
    return myEnemies;
}

bool Room::IsPlayerInRoom() const
{
    return myIsPlayerInRoom;
}

bool Room::IsRoomCleared() const
{
    return myIsRoomCleared;
}

void Room::EnterRoom()
{
    myIsPlayerInRoom = true;
}

void Room::AddEnemy(const Enemy& anEnemy)
{
    myEnemies.push_back(anEnemy);
}

void Room::AddItem()
{
    // Placeholder for adding items to the room.
}

Door* Room::GetDoor(Direction aDirection)
{
    for (auto* door : myDoors)
    {
        if (door->GetDirectionFromRoom(this) == aDirection)
        {
            return door;
        }
    }

    return nullptr;
}

const Door* Room::GetDoor(Direction aDirection) const
{
    for (const auto* door : myDoors)
    {
        if (door->GetDirectionFromRoom(this) == aDirection)
        {
            return door;
        }
    }

    return nullptr;
}

const std::vector<Door*>& Room::GetDoors() const
{
    return myDoors;
}

void Room::AddDoor(Door* aDoor)
{
    myDoors.push_back(aDoor);
}

const std::string& Room::GetName() const
{
    return myName;
}

void Room::SetDescription(const std::string& aDescription)
{
    myDescription = aDescription;
}

const std::string& Room::GetDescription() const
{
    return myDescription;
}

void Room::SetPlayerInRoom(bool aIsPlayerInRoom)
{
    myIsPlayerInRoom = aIsPlayerInRoom;
}

void Room::SetRoomCleared(bool aIsCleared)
{
    myIsRoomCleared = aIsCleared;
}

bool Room::HasEnemies() const
{
    return !myEnemies.empty();
}

void Room::DisplayOptions()
{
    std::cout << "Room: " << myName << std::endl;
    std::cout << "Description: " << myDescription << std::endl;

    if (!myEnemies.empty())
    {
        std::cout << "There are enemies in this room! You must fight before doing anything else." << std::endl;

        for (const auto& enemy : myEnemies)
        {
            std::cout << "- " << enemy.GetName() << std::endl;
        }

        std::cout << "Options:\n1. Fight enemies\n0. Exit game\n";

        return;
    }

    if (!myDoors.empty())
    {
        std::cout << "There are no enemies in this room." << std::endl;
        std::cout << "Doors available:" << std::endl;

        for (const auto* door : myDoors)
        {
            switch (door->GetDirectionFromRoom(this))
            {
            case Direction::DirectionNorth:
                std::cout << "- Door at the North wall" << std::endl;
                break;
            case Direction::DirectionEast:
                std::cout << "- Door at the East wall" << std::endl;
                break;
            case Direction::DirectionSouth:
                std::cout << "- Door at the South wall" << std::endl;
                break;
            case Direction::DirectionWest:
                std::cout << "- Door at the West wall" << std::endl;
                break;
            case Direction::DirectionCount:
                std::cout << "- Door at an unknown wall" << std::endl;
                break;
            }
        }
    }

    if (myIsRoomCleared)
    {
        std::cout << "This room is cleared.\n";
    }

    std::cout << "1. Move.\n";
}
