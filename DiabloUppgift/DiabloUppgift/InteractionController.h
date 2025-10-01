#pragma once

#include <vector>
#include <cstddef>
#include <string>

#include "Player.h"
#include "Room.h"
#include "Door.h"

class InteractionController
{
public:
    InteractionController() = delete;
    InteractionController(Player& aPlayer,
                          std::vector<Room>& someRooms,
                          Room*& aCurrentRoom,
                          std::vector<Door*>& someDoors);
    ~InteractionController();

    bool HandleTurn();

private:
    enum class Command
    {
        Quit,
        Fight,
        Move,
        Inspect,
        Inventory,
        Drop,
        Stats,
        Pickup,
        OpenChest,
        Invalid
    };

    Player& myPlayer;
    std::vector<Room>& myRooms;
    Room*& myCurrentRoom;
    std::vector<Door*>& myDoors;

    Command GetCommandWhenEnemies() const;
    Command GetCommandInSafeRoom() const;
    bool ExecuteCommand(Command aCommand);

    bool HandleMove();
    void InspectRoom() const;
    void ShowInventory();
    void DropItems();
    void PickUpItems();
    void OpenChest();
    bool StartCombat();

    bool EquipFromInventory(size_t anIndex);
    bool AreAllRoomsCleared() const;
    int ReadInt(const std::string& aPrompt) const;
};
