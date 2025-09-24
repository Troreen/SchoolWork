#include "Room.h"
#include "Door.h"
#include <iostream>

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

void Room::DisplayOptions()
{
	std::cout << "Room: " << name << std::endl;
	std::cout << "Description: " << description << std::endl;

	if (!enemies.empty())
	{
		std::cout << "There are enemies in this room! You must fight before doing anything else." << std::endl;

		for(const auto& enemy : enemies)
		{
			std::cout << "- " << enemy.GetName() << std::endl;
		}
		std::cout << "Options:\n1. Fight enemies\n0. Exit game\n";

		return;
	}

	if (!doors.empty())
	{
		std::cout << "There are no enemies in this room." << std::endl;
		std::cout << "Doors available:" << std::endl;
		
		for (const auto& door : doors)
		{
			switch (door->GetDirectionFromRoom(this))
			{
				case Direction::NORTH:
					std::cout << "- Door at the North wall" << std::endl;
					break;
				case Direction::EAST:
					std::cout << "- Door at the East wall" << std::endl;
					break;
				case Direction::SOUTH:
					std::cout << "- Door at the South wall" << std::endl;
					break;
				case Direction::WEST:
					std::cout << "- Door at the West wall" << std::endl;
					break;
				default:
					std::cout << "- Door at an unknown wall" << std::endl;
					break;
			}
		}
	}

	// chest item display print here
	
	if (isRoomCleared)
	{
		std::cout << "This room is cleared.\n";
	}

	std::cout << "1. Move.\n";
}





