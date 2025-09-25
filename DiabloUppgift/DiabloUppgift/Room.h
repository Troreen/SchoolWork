#pragma once
#include "Enemy.h"
#include <vector>
#include "Direction.h"
#include <string>

class Door;

class Room
{
public:
    Room(const std::string& aName);

    std::vector<Enemy>& Enemies();
    const std::vector<Enemy>& Enemies() const;
    bool IsPlayerInRoom() const;
    bool IsRoomCleared() const;

    void EnterRoom();
    void AddEnemy(const Enemy& anEnemy);
    void AddItem();

    Door* GetDoor(Direction aDirection);
    const Door* GetDoor(Direction aDirection) const;
    const std::vector<Door*>& GetDoors() const;
    void AddDoor(Door* aDoor);

    const std::string& GetName() const;
    void SetDescription(const std::string& aDescription);
    const std::string& GetDescription() const;

    void SetPlayerInRoom(bool aIsPlayerInRoom);
    void SetRoomCleared(bool aIsCleared);

    bool HasEnemies() const;
private:
    std::vector<Enemy> myEnemies;
    bool myIsPlayerInRoom = false;
    bool myIsRoomCleared = false;

    std::string myName;
    std::string myDescription;
    std::vector<Door*> myDoors;
};
