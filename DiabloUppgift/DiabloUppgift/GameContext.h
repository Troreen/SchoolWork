#pragma once

#include <string>
#include <vector>

class Player;
class Room;

struct GameContext
{
    Player& player;
    std::vector<Room>& rooms;
    Room*& currentRoom;

    bool isRunning = true;
    std::string endMessage;
};
