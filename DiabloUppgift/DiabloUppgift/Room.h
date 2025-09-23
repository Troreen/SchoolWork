#pragma once
#include "Enemy.h"
#include <vector>
#include "Door.h"
#include "Direction.h"
#include <string>

class Room
{

public:
	Room(const std::string& name);


	std::vector<Enemy>& Enemies();
	const std::vector<Enemy>& Enemies() const;
	bool IsPlayerInRoom() const;
	bool IsRoomCleared() const;

	void EnterRoom();
	void AddEnemy(const Enemy& enemy);
	void AddItem(); // Placeholder

	Door* GetDoor(Direction dir);
	const Door* GetDoor(Direction dir) const;
	const std::vector<Door*>& GetDoors() const; 
	void AddDoor(Door* door);

	const std::string& GetName() const;
	void SetDescription(const std::string& desc);
	const std::string& GetDescription() const;

	void SetPlayerInRoom(bool inRoom);
	void SetRoomCleared(bool cleared);

	bool HasEnemies() const;



private:
	std::vector<Enemy> enemies;
	bool isPlayerInRoom = false;
	bool isRoomCleared = false;

	std::string name;
	std::string description;
	std::vector<Door*> doors;
};};