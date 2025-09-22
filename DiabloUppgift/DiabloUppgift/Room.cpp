#include "Room.h"



std::vector<Enemy>& Room::GetEnemies()
{
    return enemies;
}

bool Room::GetIsPlayerInRoom()
{
	return isPlayerInRoom;
}

bool Room::GetIsRoomCleared()
{
	return isRoomCleared;
}

bool Room::GetIsLocked()
{
	return isLocked;
}

void Room::EnterRoom()
{
	// player enters the room
	isPlayerInRoom = true;
}

void Room::AddEnemy(const Enemy& enemy)
{
	enemies.push_back(enemy);
}

void Room::AddItem()
{
}
