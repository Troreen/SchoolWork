#pragma once

#include <string>
#include <vector>

class Door;
class Player;
class Room;

struct GameContext
{
    Player& player;
    std::vector<Room>& rooms;
    Room*& currentRoom;
    std::vector<Door*>& doors;

    bool isRunning = true;
    std::string endMessage;
};
