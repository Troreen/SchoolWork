#include "GameManager.h"
#include "Enemy.h"
#include "Room.h"
#include "Door.h"
#include "Direction.h"

GameManager::GameManager()
{
	// Initialize rooms
	rooms.emplace_back("Entrance Hall");
	rooms.emplace_back("Armory");
	rooms.emplace_back("Library");
	rooms.emplace_back("Dungeon");

	// Create doors between rooms
	Door* door1 = new Door(&rooms[0], &rooms[1], Direction::NORTH, Direction::SOUTH);
	Door* door2 = new Door(&rooms[1], &rooms[2], Direction::EAST, Direction::WEST, true); // Locked door
	Door* door3 = new Door(&rooms[2], &rooms[3], Direction::SOUTH, Direction::NORTH, false, true); // Breakable door
	// Add doors to rooms
	rooms[0].AddDoor(door1);
	rooms[1].AddDoor(door1);
	rooms[1].AddDoor(door2);
	rooms[2].AddDoor(door2);
	rooms[2].AddDoor(door3);
	rooms[3].AddDoor(door3);
	// Add enemies to rooms
	// 

	// Set the starting room
	currentRoom = &rooms[0];
	currentRoom->EnterRoom();
}

// deconstructor to clean up dynamically allocated doors
GameManager::~GameManager()
{
    // To avoid double-deletion, keep track of which doors have been deleted
    std::vector<Door*> deletedDoors;
    for (auto& room : rooms)
    {
        for (auto door : room.doors) // doors is private, so add a public accessor if needed
        {
            // Only delete if not already deleted
            if (std::find(deletedDoors.begin(), deletedDoors.end(), door) == deletedDoors.end())
            {
                deletedDoors.push_back(door);
                delete door;
            }
        }
    }
}
