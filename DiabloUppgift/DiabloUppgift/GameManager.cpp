#include "GameManager.h"

#include "Door.h"
#include "Direction.h"
#include "Enemy.h"
#include "EnemyFactory.h"
#include "EnemyTypes.h"
#include "InteractionController.h"
#include "InventoryTypes.h"
#include "ItemFactory.h"
#include "Chest.h"
#include "ChestFactory.h"
#include "ChestTypes.h"

#include <memory>
#include <iostream>

GameManager::GameManager()
    : myPlayer("Hero", 10, 7, 8)
    , myRooms()
    , myCurrentRoom(nullptr)
    , myDoors()
{
    GetEnemyFactory().Initialize();

    myRooms.emplace_back("First Room");
    myRooms.emplace_back("Second Room");
    myRooms.emplace_back("Third Room");
    myRooms.emplace_back("Fourth Room");

    const bool isLocked = true;

    auto& door1 = myDoors.emplace_back(std::make_unique<Door>(
        &myRooms[0],
        &myRooms[1],
        Direction::DirectionNorth,
        Direction::DirectionSouth,
        !isLocked,
        0,
        0));
    Door* door1Ptr = door1.get();

    myRooms[0].AddDoor(door1Ptr);
    myRooms[1].AddDoor(door1Ptr);

    std::unique_ptr<Door>& door2 = myDoors.emplace_back(std::make_unique<Door>(
        &myRooms[1],
        &myRooms[2],
        Direction::DirectionEast,
        Direction::DirectionWest,
        isLocked,
        3,
        10));
    Door* door2Ptr = door2.get();

    myRooms[1].AddDoor(door2Ptr);
    myRooms[2].AddDoor(door2Ptr);

    auto& door3 = myDoors.emplace_back(std::make_unique<Door>(
        &myRooms[2],
        &myRooms[3],
        Direction::DirectionSouth,
        Direction::DirectionNorth,
        isLocked,
        7,
        2));
    Door* door3Ptr = door3.get();

    myRooms[2].AddDoor(door3Ptr);
    myRooms[3].AddDoor(door3Ptr);

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

GameManager::~GameManager() = default;

void GameManager::Run()
{
    std::vector<Door*> doorViews;
    doorViews.reserve(myDoors.size());
    for (const auto& door : myDoors)
    {
        doorViews.push_back(door.get());
    }

    InteractionController interaction(myPlayer, myRooms, myCurrentRoom, doorViews);
    bool isRunning = true;

    while (isRunning)
    {
        isRunning = interaction.HandleTurn();
    }

    std::cout << "\nThanks for playing!\n";
}




