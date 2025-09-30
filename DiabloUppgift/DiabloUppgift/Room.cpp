#include "Room.h"
#include "Door.h"

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
