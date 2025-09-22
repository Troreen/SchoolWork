#pragma once
#include "Enemy.h"
#include <vector>
class Room
{
public:
	std::vector<Enemy>& GetEnemies();
	bool GetIsPlayerInRoom();
	bool GetIsRoomCleared();
	bool GetIsLocked();

	
	void EnterRoom();
	void AddEnemy(const Enemy& enemy);
	void AddItem(); // Placeholder for future item addition functionality
	


private:
	std::vector<Enemy> enemies;
	bool isPlayerInRoom = false;
	bool isRoomCleared = false;
	bool isLocked = false;

};