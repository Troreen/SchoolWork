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
    Player myPlayer;
    std::vector<Room> myRooms;
    Room* myCurrentRoom;
    std::vector<Door*> myDoors;
};
