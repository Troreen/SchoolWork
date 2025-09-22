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
	void MainLoop();

private:
	Player player;
	Room rooms[5];
	int currentRoomIndex;
	std::string roomNames[5];
	int roomConnections[5][4]; // 0: North, 1: East, 2: South, 3: West
	void PrintCurrentRoomAndPrompt();
	int GetDirectionInput();
	void PrintRoomContents();
	void PrintEnemiesWithStats(const std::vector<Enemy>& enemies);
};
