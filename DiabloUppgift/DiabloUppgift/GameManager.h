#pragma once
#include "Player.h"
#include "Room.h"
#include <vector>
#include <string>
#include <iostream>
#include <random>

class GameManager
{
public:
	GameManager();
	~GameManager();
	void Run();

private:
	Player player;
	std::vector<Room> rooms;
	Room* currentRoom;
	std::vector<Door*> doors;

	
	int HandleInput();
	bool StartCombat();
};
