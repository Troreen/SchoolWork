#include "GameManager.h"

#include "Door.h"
#include "Direction.h"
#include "Enemy.h"
#include "EnemyTypes.h"
#include "InteractionController.h"
#include "InventoryTypes.h"
#include "Chest.h"
#include "ChestTypes.h"
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

    const EnemyFactory& enemyFactory = GetEnemyFactory();
    myRooms[1].AddEnemy(enemyFactory.Make(EnemyId::Goblin));
    myRooms[2].AddEnemy(enemyFactory.Make(EnemyId::Goblin));
    myRooms[2].AddEnemy(enemyFactory.Make(EnemyId::Orc));
    myRooms[3].AddEnemy(enemyFactory.Make(EnemyId::Troll));

    myCurrentRoom = &myRooms[0];
    myCurrentRoom->EnterRoom();

    myPlayer.Inventory().maxCarryWeight = 50.0f;
    myPlayer.RecalculateDerivedStats();

    const ItemFactory& itemFactory = GetItemFactory();
    
    myRooms[0].AddFloorItem(itemFactory.Make(ItemId::LongBow));
    myRooms[0].AddFloorItem(itemFactory.Make(ItemId::ChainmailArmor));
    myRooms[0].AddFloorItem(itemFactory.Make(ItemId::HealthPotion, 2));
    
    myRooms[1].AddFloorItem(itemFactory.Make(ItemId::BattleAxe));
    myRooms[1].AddFloorItem(itemFactory.Make(ItemId::HealthPotion));
    myRooms[2].AddFloorItem(itemFactory.Make(ItemId::FuryEnchant));
    
    const ChestFactory& chestFactory = GetChestFactory();
    
    Chest entranceChest = chestFactory.Make(ChestId::WoodenChest);
    myRooms[0].AddChest(entranceChest);

    Chest treasureChest = chestFactory.Make(ChestId::IronChest);
    myRooms[2].AddChest(treasureChest);


    

    if (!myRooms[1].Enemies().empty())
    {
        myRooms[1].Enemies()[0].AddLoot(itemFactory.Make(ItemId::HealthPotion), 0.6f);
        if (myRooms[1].Enemies().size() > 1)
        {
            myRooms[1].Enemies()[1].AddLoot(itemFactory.Make(ItemId::LongBow), 0.5f);
        }
    }
    if (!myRooms[2].Enemies().empty())
    {
        myRooms[2].Enemies()[0].AddLoot(itemFactory.Make(ItemId::ChainmailArmor), 0.4f);
    }
    if (!myRooms[3].Enemies().empty())
    {
        myRooms[3].Enemies()[0].AddLoot(itemFactory.Make(ItemId::FuryEnchant), 0.3f);
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
