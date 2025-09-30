#pragma once

#include <vector>
#include <string>

#include "Player.h"
#include "Room.h"
#include "Door.h"

class InteractionController
{
public:
    InteractionController(Player& aPlayer,
                          std::vector<Room>& someRooms,
                          Room*& aCurrentRoom,
                          std::vector<Door*>& someDoors);

    bool HandleTurn();

private:
    enum class Command
    {
        Quit,
        Fight,
        Move,
        Inspect,
        Inventory,
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
    void ShowInventory() const;
    void PickUpItems();
    void OpenChest();
    bool StartCombat();

    int ReadInt(const std::string& aPrompt) const;
};
