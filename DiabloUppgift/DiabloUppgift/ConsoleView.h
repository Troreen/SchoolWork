#pragma once

#include <vector>
#include <utility>
#include <string>

#include "Player.h"
#include "Room.h"
#include "Door.h"
#include "Enemy.h"
#include "Chest.h"

namespace ConsoleView
{
    void ClearScreen();
    void ShowRoomOverview(const Room& aRoom, const Player& aPlayer);
    void ShowRoomInspection(const Room& aRoom);
    void ShowSafeRoomMenu(const Room& aRoom, const Player& aPlayer, const std::vector<std::pair<int, std::string>>& someOptions);
    void ShowCombatLayout(const Room& aRoom, const Player& aPlayer, const std::vector<Enemy>& someEnemies, const std::vector<std::pair<int, std::string>>& someOptions);
    void ShowInventory(const Player& aPlayer);
    void ShowDropList(const std::vector<std::pair<std::string, ItemInstance>>& someItems);
    void ShowPickupList(const std::vector<ItemInstance>& someItems);
    void ShowPickupResult(const std::string& anItemName, int aCount, bool aSuccess);
    void ShowDropResult(const ItemSpec& aSpec, int aCount);
    void ShowActivationMessage(const ItemSpec& aSpec);
    void ShowEquipmentPrompt(const ItemSpec& currentSpec, const ItemSpec& newSpec, const char* aSlotLabel);
    void ShowEquipmentResult(const ItemSpec& aSpec, const char* aSlotLabel, bool aEquipped);
    void ShowInvalidChoice();
    void ShowNoItemsMessage();
    void ShowNoItemsToDrop();
    void ShowCannotEquipMessage();
    void ShowDoorList(const std::vector<Door*>& someDoors, const Room* aCurrentRoom);
    void ShowNoExits();
    void ShowLockedDoorPrompt();
    void ShowDoorUnlockResult(bool aSuccess);
    void ShowDoorBreakResult(bool aSuccess);
    void ShowDoorSkipMessage();
    void ShowCapacityExceededMessage(const ItemSpec& aSpec, float remainingWeight);
    void ShowChestList(const std::vector<const Chest*>& someChests);
    void ShowChestOverview(const std::vector<Chest>& someChests);
    void ShowChestOpenResult(const Chest& aChest, const std::vector<ItemInstance>& someItems);
    void ShowNoClosedChests();
    void ShowEnemies(const std::vector<Enemy>& someEnemies);
    void ShowPlayerStats(const Player& aPlayer);
    void ShowNoEnemiesToAttack();
    void ShowAllEnemiesCleared();
    void ShowCombatVictory();
    void ShowCombatDefeat();
    void Pause();
}
