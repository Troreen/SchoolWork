#pragma once
#include "Enemy.h"
#include "InventoryTypes.h"
#include "Chest.h"
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

    std::vector<ItemInstance>& FloorItems();
    const std::vector<ItemInstance>& FloorItems() const;
    void AddFloorItem(const ItemInstance& anItem);
    void RemoveFloorItem(size_t anIndex);
    bool HasFloorItems() const;

    std::vector<Chest>& Chests();
    const std::vector<Chest>& Chests() const;
    void AddChest(const Chest& aChest);
    bool HasUnopenedChest() const;

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
    bool myIsPlayerInRoom;
    bool myIsRoomCleared;

    std::string myName;
    std::string myDescription;
    std::vector<Door*> myDoors;
    std::vector<ItemInstance> myFloorItems;
    std::vector<Chest> myChests;
};
