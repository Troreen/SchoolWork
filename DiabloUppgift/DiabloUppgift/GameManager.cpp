#include "GameManager.h"

#include "Door.h"
#include "Direction.h"
#include "Enemy.h"
#include "InteractionController.h"
#include "InventoryTypes.h"
#include "Chest.h"
#include <iostream>

GameManager::GameManager()
    : myPlayer("Hero", 5, 5, 5)
    , myRooms()
    , myCurrentRoom(nullptr)
    , myDoors()
{
    myRooms.emplace_back("First Room");
    myRooms.emplace_back("Second Room");
    myRooms.emplace_back("Third Room");
    myRooms.emplace_back("Fourth Room");

    const bool isLocked = true;

    Door* doorFromRoom1ToRoom2 = new Door(&myRooms[0],
                                          &myRooms[1],
                                          Direction::DirectionNorth,
                                          Direction::DirectionSouth,
                                          !isLocked,
                                          0,
                                          0);
    Door* doorFromRoom2ToRoom3 = new Door(&myRooms[1],
                                          &myRooms[2],
                                          Direction::DirectionEast,
                                          Direction::DirectionWest,
                                          isLocked,
                                          3,
                                          10);
    Door* doorFromRoom3ToRoom4 = new Door(&myRooms[2],
                                          &myRooms[3],
                                          Direction::DirectionSouth,
                                          Direction::DirectionNorth,
                                          isLocked,
                                          7,
                                          2);

    myDoors.push_back(doorFromRoom1ToRoom2);
    myDoors.push_back(doorFromRoom2ToRoom3);
    myDoors.push_back(doorFromRoom3ToRoom4);

    myRooms[0].AddDoor(doorFromRoom1ToRoom2);
    myRooms[1].AddDoor(doorFromRoom1ToRoom2);
    myRooms[1].AddDoor(doorFromRoom2ToRoom3);
    myRooms[2].AddDoor(doorFromRoom2ToRoom3);
    myRooms[2].AddDoor(doorFromRoom3ToRoom4);
    myRooms[3].AddDoor(doorFromRoom3ToRoom4);

    myRooms[1].AddEnemy(Enemy("Goblin", 15, 3, 2));
    myRooms[1].AddEnemy(Enemy("Goblin Archer", 6, 4, 1));
    myRooms[2].AddEnemy(Enemy("Orc", 12, 2, 10));
    myRooms[3].AddEnemy(Enemy("Troll", 20, 1, 15));

    myCurrentRoom = &myRooms[0];
    myCurrentRoom->EnterRoom();

    myPlayer.Inventory().maxCarryWeight = 50.0f;
    myPlayer.RecalculateDerivedStats();

    myRooms[0].AddFloorItem({ ItemId::LongBow, 1, 0, 0, false });
    myRooms[0].AddFloorItem({ ItemId::ChainmailArmor, 1, 0, 0, false });
    myRooms[0].AddFloorItem({ ItemId::HealthPotion, 2, 0, 0, false });

    myRooms[1].AddFloorItem({ ItemId::BattleAxe, 1, 0, 0, false });
    myRooms[1].AddFloorItem({ ItemId::HealthPotion, 1, 0, 0, false });
    myRooms[2].AddFloorItem({ ItemId::FuryEnchant, 1, 0, 5, false });

    Chest entranceChest("Wooden Chest", "A small wooden chest sits in the corner.");
    entranceChest.AddItem({ ItemId::HealthPotion, 1, 0, 0, false });
    entranceChest.AddItem({ ItemId::ShortSword, 1, 0, 0, false });
    myRooms[0].AddChest(entranceChest);

    Chest treasureChest("Ironbound Chest", "A sturdy chest bound in iron.");
    treasureChest.AddItem({ ItemId::ChainmailArmor, 1, 0, 0, false });
    treasureChest.AddItem({ ItemId::HealthPotion, 1, 0, 0, false });
    myRooms[2].AddChest(treasureChest);

    if (!myRooms[1].Enemies().empty())
    {
        myRooms[1].Enemies()[0].AddLoot({ ItemId::HealthPotion, 1, 0, 0, false }, 60);
        if (myRooms[1].Enemies().size() > 1)
        {
            myRooms[1].Enemies()[1].AddLoot({ ItemId::LongBow, 1, 0, 0, false }, 50);
        }
    }
    if (!myRooms[2].Enemies().empty())
    {
        myRooms[2].Enemies()[0].AddLoot({ ItemId::ChainmailArmor, 1, 0, 0, false }, 40);
    }
    if (!myRooms[3].Enemies().empty())
    {
        myRooms[3].Enemies()[0].AddLoot({ ItemId::FuryEnchant, 1, 0, 5, false }, 30);
    }
}

GameManager::~GameManager()
{
    for (Door* door : myDoors)
    {
        delete door;
    }
}

void GameManager::Run()
{
    InteractionController interaction(myPlayer, myRooms, myCurrentRoom, myDoors);
    bool isRunning = true;

    while (isRunning)
    {
        isRunning = interaction.HandleTurn();
    }

    std::cout << "\nThanks for playing!\n";
}
