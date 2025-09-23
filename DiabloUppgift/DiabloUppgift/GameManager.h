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
	std::vector<Room> rooms;
	Room* currentRoom;

	void PrintCurrentRoomAndPrompt() const;
	int GetDirectionInput() const;
	void PrintRoomContents() const;
	void PrintEnemiesWithStats(const std::vector<Enemy>& enemies) const;
};
