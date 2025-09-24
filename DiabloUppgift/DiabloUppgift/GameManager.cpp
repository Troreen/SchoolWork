#include "GameManager.h"
#include "Enemy.h"
#include "Room.h"
#include "Door.h"
#include "Direction.h"
#include "CombatComponent.h"

GameManager::GameManager()
	: player("Hero", 5, 5, 5)
{
	// Initialize rooms
	rooms.emplace_back("First Room");
	rooms.emplace_back("Second Room");
	rooms.emplace_back("Third Room");
	rooms.emplace_back("Fourth Room");

	// Create doors between rooms
	bool locked = true;
	bool breakable = true;
	Door* door1 = new Door(&rooms[0], &rooms[1], Direction::NORTH, Direction::SOUTH, !locked, !breakable);
	Door* door2 = new Door(&rooms[1], &rooms[2], Direction::EAST, Direction::WEST, locked, !breakable); // Locked door
	Door* door3 = new Door(&rooms[2], &rooms[3], Direction::SOUTH, Direction::NORTH, !locked, breakable); // Breakable door
	doors.push_back(door1);
	doors.push_back(door2);
	doors.push_back(door3);

	// Add doors to rooms
	rooms[0].AddDoor(door1);
	rooms[1].AddDoor(door1);
	rooms[1].AddDoor(door2);
	rooms[2].AddDoor(door2);
	rooms[2].AddDoor(door3);
	rooms[3].AddDoor(door3);
	
	// Add enemies to rooms
	rooms[0].AddEnemy(Enemy("Goblin", 2, 3, 2));
	rooms[1].AddEnemy(Enemy("Orc", 4, 2, 4));
	rooms[2].AddEnemy(Enemy("Troll", 5, 1, 6));

	// Set the starting room
	currentRoom = &rooms[0];
	currentRoom->EnterRoom();
}

void GameManager::Run()
{
	int input = 1;
	while (input)
	{
		currentRoom->PrintRoomInfo();
	}
	input = HandleInput();
}

int GameManager::HandleInput()
{
	currentRoom->DisplayOptions();
	return 0;
}

bool GameManager::StartCombat()
{
	CombatComponent combat(player, currentRoom->Enemies());

	while (combat.GetResult() == CombatComponent::Result::ONGOING)
	{
		const Enemy* enemy = combat.GetCurrentEnemy();
		if (enemy)
		{
			std::cout << "\nYou are fighting: " << enemy->PrintStats();
			std::cout << "\nYour stats:\n" << player.PrintStats();
		}

		std::cout << "\nChoose your action:\n";
		std::cout << "1. Attack\n";
		std::cout << "2. Defend\n";
		std::cout << "Choice: ";

		int actionInput;
		std::cin >> actionInput;

		CombatComponent::Action action;
		switch (actionInput)
		{
		case 1:
			action = CombatComponent::Action::ATTACK; 
			break;
		case 2:
			action = CombatComponent::Action::DEFEND; 
			break;
		case 3:
			action = CombatComponent::Action::USE_ITEM; 
			break;
		default:
			std::cout << "Invalid action. Try again.\n";
			continue;
		}

		combat.PerformPlayerAction(action);

		if (combat.GetResult() != CombatComponent::Result::ONGOING)
		{
			break;
		}

		combat.PerformEnemyTurn();
	}

	switch (combat.GetResult())
	{
	case CombatComponent::Result::PLAYER_WON:
		std::cout << "All enemies defeated!\n";
		currentRoom->SetRoomCleared(true);
		return true;
	case CombatComponent::Result::PLAYER_LOST:
		std::cout << "You have been slain!\n";
		return false;	
	default:
		return true;
	}
}

GameManager::~GameManager()
{
	for (auto door : doors)
		delete door;
}