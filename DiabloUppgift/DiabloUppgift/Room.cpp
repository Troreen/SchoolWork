#include "Room.h"

Room::Room(const std::string& name) : name(name) 
{
}

std::vector<Enemy>& Room::Enemies() 
{ 
	return enemies; 
}

const std::vector<Enemy>& Room::Enemies() const 
{ 
	return enemies; 
}

bool Room::IsPlayerInRoom() const 
{ 
	return isPlayerInRoom; 
}

bool Room::IsRoomCleared() const 
{ 
	return isRoomCleared; 
}

void Room::EnterRoom() 
{ 
	isPlayerInRoom = true; 
}

void Room::AddEnemy(const Enemy& enemy) 
{ 
	enemies.push_back(enemy); 
}

void Room::AddItem() 
{ 
	// Placeholder for adding items to the room
}

Door* Room::GetDoor(Direction dir)
{
	for (auto door : doors) 
	{
		if (door->GetDirectionFromRoom(this) == dir) 
		{
			return door;
		}
	}
	return nullptr; 
}

const Door* Room::GetDoor(Direction dir) const 
{
	for (auto door : doors) 
	{
		if (door->GetDirectionFromRoom(this) == dir) 
		{
			return door;
		}
	}
	return nullptr; 
}

const std::vector<Door*>& Room::GetDoors() const
{
	return doors;
}

void Room::AddDoor(Door* door) 
{ 
	doors.push_back(door); 
}

const std::string& Room::GetName() const 
{ 
	return name; 
}

void Room::SetDescription(const std::string& desc) 
{ 
	description = desc; 
}

const std::string& Room::GetDescription() const 
{ 
	return description; 
}

void Room::SetPlayerInRoom(bool inRoom) 
{ 
	isPlayerInRoom = inRoom; 
}

void Room::SetRoomCleared(bool cleared) 
{ 
	isRoomCleared = cleared; 
}

bool Room::HasEnemies() const 
{ 
	return !enemies.empty(); 
}





