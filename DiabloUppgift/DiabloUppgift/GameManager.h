#pragma once
#include "Player.h"
#include "Room.h"
#include <vector>
#include <string>

class Door;

class GameManager
{
public:
    GameManager();
    ~GameManager();
    void Run();

private:
    enum class PlayerCommand
    {
        PlayerCommandQuit,
        PlayerCommandFight,
        PlayerCommandMove,
        PlayerCommandInspect,
        PlayerCommandInventory,
        PlayerCommandInvalid,
        PlayerCommandCount
    };

    Player myPlayer;
    std::vector<Room> myRooms;
    Room* myCurrentRoom;
    std::vector<Door*> myDoors;

    bool HandleInput();
    PlayerCommand GetCommandWhenEnemies() const;
    PlayerCommand GetCommandInSafeRoom() const;
    bool ExecuteCommand(PlayerCommand aCommand);
    bool HandleMove();
    void InspectRoom() const;
    void ShowInventory() const;
    void DescribeCurrentRoom() const;
    int ReadInt(const std::string& aPrompt) const;
    bool StartCombat();
};
