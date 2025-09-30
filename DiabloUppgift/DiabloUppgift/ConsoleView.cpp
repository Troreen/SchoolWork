#include "ConsoleView.h"

#include <iostream>
#include <string>

#include "InventoryTypes.h"

namespace
{
    std::string DirectionToString(Direction aDirection)
    {
        switch (aDirection)
        {
        case Direction::DirectionNorth:
            return "North";
        case Direction::DirectionEast:
            return "East";
        case Direction::DirectionSouth:
            return "South";
        case Direction::DirectionWest:
            return "West";
        case Direction::DirectionCount:
            return "Unknown";
        }

        return "Unknown";
    }

    std::string FormatItemStack(const ItemInstance& anItem)
    {
        const ItemSpec& spec = GetItemSpec(anItem.id);
        std::string line = spec.name;
        if (spec.maxStack > 1)
        {
            line += " x" + std::to_string(anItem.count);
        }
        return line;
    }

    std::string BuildChestSummary(const std::vector<Chest>& someChests)
    {
        if (someChests.empty())
        {
            return "Chests: none";
        }

        int unopened = 0;
        for (const Chest& chest : someChests)
        {
            if (!chest.IsOpened())
            {
                ++unopened;
            }
        }

        const int total = static_cast<int>(someChests.size());
        if (unopened == 0)
        {
            return "Chests: all opened";
        }

        if (total == 1)
        {
            return unopened == 1 ? "Chests: 1 unopened" : "Chests: 1";
        }

        return "Chests: " + std::to_string(unopened) + " unopened (" + std::to_string(total) + " total)";
    }

    std::string BuildFloorLootSummary(const std::vector<ItemInstance>& someItems)
    {
        if (someItems.empty())
        {
            return "none";
        }

        std::string summary;
        for (size_t index = 0; index < someItems.size(); ++index)
        {
            if (index > 0)
            {
                summary += ", ";
            }
            summary += FormatItemStack(someItems[index]);
        }
        return summary;
    }

    std::string BuildEnchantmentSummary(const Player& aPlayer)
    {
        if (!aPlayer.HasActiveEnchantments())
        {
            return "none";
        }
        return aPlayer.GetActiveEnchantmentsSummary();
    }

    void PrintEquipmentSlot(const char* aLabel, bool aHasItem, const ItemInstance& anItem)
    {
        std::cout << aLabel << ": ";
        if (aHasItem && anItem.equipped)
        {
            const ItemSpec& spec = GetItemSpec(anItem.id);
            std::cout << spec.name;
            if (spec.attackBonus != 0 || spec.defenseBonus != 0)
            {
                std::cout << " (ATK +" << spec.attackBonus << ", DEF +" << spec.defenseBonus << ")";
            }
            std::cout << "\n";
        }
        else
        {
            std::cout << "Empty\n";
        }
    }
}

void ConsoleView::ClearScreen()
{
    std::cout << "\x1B[2J\x1B[H";
}

void ConsoleView::ShowRoomOverview(const Room& aRoom, const Player& aPlayer)
{
    std::cout << "\nYou are in: " << aRoom.GetName() << "\n";
    std::cout << aRoom.GetDescription() << "\n";

    if (aRoom.HasEnemies())
    {
        std::cout << "Enemies present:\n";
        for (const auto& enemy : aRoom.Enemies())
        {
            std::cout << "- " << enemy.GetName() << "\n";
        }
    }
    else
    {
        const auto& doorsInRoom = aRoom.GetDoors();
        if (doorsInRoom.empty())
        {
            std::cout << "There are no exits from this room.\n";
        }
        else
        {
            std::cout << "Available exits:\n";
            for (const auto* door : doorsInRoom)
            {
                std::cout << "- " << DirectionToString(door->GetDirectionFromRoom(&aRoom));
                if (door->IsLocked())
                {
                    std::cout << " (locked) [DEX to unlock: " << door->GetDexterityToUnlock() << ", STR to break: " << door->GetStrengthToBreak() << "]";
                }
                std::cout << "\n";
            }
        }

        const auto& floorItems = aRoom.FloorItems();
        if (!floorItems.empty())
        {
            std::cout << "Items on the floor:\n";
            for (const ItemInstance& instance : floorItems)
            {
                std::cout << "- " << FormatItemStack(instance) << "\n";
            }
        }

        ShowChestOverview(aRoom.Chests());

        if (aRoom.IsRoomCleared())
        {
            std::cout << "The room is cleared.\n";
        }
    }

    if (aPlayer.HasActiveEnchantments())
    {
        std::cout << "Active enchantments: " << aPlayer.GetActiveEnchantmentsSummary() << "\n";
    }
}

void ConsoleView::ShowRoomInspection(const Room& aRoom)
{
    std::cout << "\nYou inspect the room carefully.\n";
    std::cout << aRoom.GetDescription() << "\n";

    if (aRoom.HasEnemies())
    {
        std::cout << "Enemies present:\n";
        for (const auto& enemy : aRoom.Enemies())
        {
            std::cout << "- " << enemy.GetName() << "\n";
        }
    }
    else
    {
        const auto& doorsInRoom = aRoom.GetDoors();
        if (doorsInRoom.empty())
        {
            std::cout << "There are no exits from this room.\n";
        }
        else
        {
            std::cout << "Available exits:\n";
            for (const auto* door : doorsInRoom)
            {
                std::cout << "- " << DirectionToString(door->GetDirectionFromRoom(&aRoom));
                if (door->IsLocked())
                {
                    std::cout << " (locked) [DEX to unlock: " << door->GetDexterityToUnlock() << ", STR to break: " << door->GetStrengthToBreak() << "]";
                }
                std::cout << "\n";
            }
        }

        ShowChestOverview(aRoom.Chests());

        if (aRoom.IsRoomCleared())
        {
            std::cout << "The room is cleared.\n";
        }
    }
}

void ConsoleView::ShowSafeRoomMenu(const Room& aRoom, const Player& aPlayer, const std::vector<std::pair<int, std::string>>& someOptions)
{
    std::cout << "\n== Room Summary ==\n";
    std::cout << aRoom.GetName();
    if (aRoom.IsRoomCleared())
    {
        std::cout << " (cleared)\n";
    }
    else
    {
        std::cout << "\n";
    }

    std::cout << BuildChestSummary(aRoom.Chests())
              << " | Floor Loot: " << BuildFloorLootSummary(aRoom.FloorItems()) << "\n";

    const float carryWeight = aPlayer.GetCarryWeight();
    std::cout << "HP " << aPlayer.GetHealth() << "/" << aPlayer.GetMaxHealth()
              << "   Carry " << carryWeight << "/" << aPlayer.GetMaxCarryWeight()
              << "   Enchants: " << BuildEnchantmentSummary(aPlayer) << "\n";

    std::cout << "\n== Description ==\n";
    const std::string& description = aRoom.GetDescription();
    if (description.empty())
    {
        std::cout << "(No description)\n";
    }
    else
    {
        std::cout << description << "\n";
    }

    std::cout << "\n== Actions ==\n";
    for (const auto& option : someOptions)
    {
        std::cout << option.first << ") " << option.second << "\n";
    }
}

void ConsoleView::ShowCombatLayout(const Player& aPlayer, const std::vector<Enemy>& someEnemies, const std::vector<std::pair<int, std::string>>& someOptions)
{
    std::cout << "\n== Combat ==\n";
    if (someEnemies.empty())
    {
        std::cout << "(no enemies)\n";
    }
    else
    {
        for (const Enemy& enemy : someEnemies)
        {
            std::cout << "- " << enemy.GetName() << " (HP " << enemy.GetHealth() << "/" << enemy.GetMaxHealth() << ")\n";
        }
    }

    std::cout << "\n== You ==\n";
    std::cout << "HP " << aPlayer.GetHealth() << "/" << aPlayer.GetMaxHealth()
              << "   Enchants: " << BuildEnchantmentSummary(aPlayer) << "\n";

    std::cout << "\n== Actions ==\n";
    for (const auto& option : someOptions)
    {
        std::cout << option.first << ") " << option.second << "\n";
    }
}

void ConsoleView::ShowInventory(const Player& aPlayer)
{
    std::cout << "\n=== Equipment ===\n";
    const EquipmentState& equipment = aPlayer.Equipment();

    PrintEquipmentSlot("Main Hand", equipment.hasMainHand, equipment.mainHand);
    PrintEquipmentSlot("Chest", equipment.hasChest, equipment.chest);

    const InventoryState& inventory = aPlayer.Inventory();
    std::cout << "\n=== Backpack ===\n";

    if (inventory.items.empty())
    {
        std::cout << "Empty\n";
    }
    else
    {
        for (size_t index = 0; index < inventory.items.size(); ++index)
        {
            std::cout << index + 1 << ". " << FormatItemStack(inventory.items[index]) << "\n";
        }
    }

    const float carryWeight = aPlayer.GetCarryWeight();
    std::cout << "Carry weight: " << carryWeight << "/" << aPlayer.GetMaxCarryWeight() << "\n";

    if (aPlayer.HasActiveEnchantments())
    {
        std::cout << "\nActive enchantments: " << aPlayer.GetActiveEnchantmentsSummary() << "\n";
    }

    std::cout << "\nPress enter to continue...";
}

void ConsoleView::ShowPickupList(const std::vector<ItemInstance>& someItems)
{
    std::cout << "\nItems on the floor:\n";
    for (size_t index = 0; index < someItems.size(); ++index)
    {
        std::cout << index + 1 << ". " << FormatItemStack(someItems[index]) << "\n";
    }
    std::cout << "0. Cancel\n";
}

void ConsoleView::ShowPickupResult(const std::string& anItemName, int aCount, bool aSuccess)
{
    if (aSuccess)
    {
        std::cout << "You pick up " << anItemName;
        if (aCount > 1)
        {
            std::cout << " x" << aCount;
        }
        std::cout << ".\n";
    }
    else
    {
        std::cout << "You leave the " << anItemName << ".\n";
    }
}

void ConsoleView::ShowActivationMessage(const ItemSpec& aSpec)
{
    std::cout << "You feel the " << aSpec.name << " surge through you.";
    if (aSpec.actionsDuration > 0)
    {
        std::cout << " It will persist for " << aSpec.actionsDuration << " actions.";
    }
    std::cout << "\n";
}

void ConsoleView::ShowEquipmentPrompt(const ItemSpec& currentSpec, const ItemSpec& newSpec, const char* aSlotLabel)
{
    std::cout << "You are already using " << currentSpec.name << " in your " << aSlotLabel
              << ". Drop it and equip " << newSpec.name << "? (y/n): ";
}

void ConsoleView::ShowEquipmentResult(const ItemSpec& aSpec, const char* aSlotLabel, bool aEquipped)
{
    if (aEquipped)
    {
        std::cout << "You equip the " << aSpec.name << " in your " << aSlotLabel << ".\n";
    }
    else
    {
        std::cout << "You decide to keep your current equipment.\n";
    }
}

void ConsoleView::ShowInvalidChoice()
{
    std::cout << "Invalid choice.\n";
}

void ConsoleView::ShowNoItemsMessage()
{
    std::cout << "\nThere are no items to pick up.\n";
}

void ConsoleView::ShowCannotEquipMessage()
{
    std::cout << "You cannot equip that item right now.\n";
}

void ConsoleView::ShowDoorList(const std::vector<Door*>& someDoors, const Room* aCurrentRoom)
{
    std::cout << "\nAvailable exits:\n";
    for (size_t index = 0; index < someDoors.size(); ++index)
    {
        const Door* door = someDoors[index];
        std::cout << index + 1 << ". " << DirectionToString(door->GetDirectionFromRoom(aCurrentRoom));
        if (door->IsLocked())
        {
            std::cout << " (locked) [DEX to unlock: " << door->GetDexterityToUnlock() << ", STR to break: " << door->GetStrengthToBreak() << "]";
        }
        std::cout << "\n";
    }
}

void ConsoleView::ShowNoExits()
{
    std::cout << "There are no exits from this room.\n";
}

void ConsoleView::ShowLockedDoorPrompt()
{
    std::cout << "The door is locked. Try to unlock (L), break (B), or leave it (N)? ";
}

void ConsoleView::ShowDoorUnlockResult(bool aSuccess)
{
    if (aSuccess)
    {
        std::cout << "You successfully picked the lock!\n";
    }
    else
    {
        std::cout << "You failed to pick the lock.\n";
    }
}

void ConsoleView::ShowDoorBreakResult(bool aSuccess)
{
    if (aSuccess)
    {
        std::cout << "You successfully broke down the door!\n";
    }
    else
    {
        std::cout << "You failed to break down the door.\n";
    }
}

void ConsoleView::ShowDoorSkipMessage()
{
    std::cout << "You decided not to attempt to open the door.\n";
}

void ConsoleView::ShowCapacityExceededMessage(const ItemSpec& aSpec, float remainingWeight)
{
    if (remainingWeight < 0.0f)
    {
        remainingWeight = 0.0f;
    }

    std::cout << "You cannot carry the " << aSpec.name << ". Remaining capacity: " << remainingWeight << "\n";
}

void ConsoleView::ShowChestOverview(const std::vector<Chest>& someChests)
{
    if (someChests.empty())
    {
        return;
    }

    bool printed = false;
    for (const Chest& chest : someChests)
    {
        if (!printed)
        {
            std::cout << "Chests:\n";
            printed = true;
        }

        std::cout << "- " << chest.GetName();
        if (chest.IsOpened())
        {
            std::cout << " (opened)";
        }
        std::cout << "\n";
    }
}

void ConsoleView::ShowChestList(const std::vector<const Chest*>& someChests)
{
    std::cout << "\nChests:\n";
    for (size_t index = 0; index < someChests.size(); ++index)
    {
        const Chest* chest = someChests[index];
        std::cout << index + 1 << ". " << chest->GetName() << "\n";
    }
    std::cout << "0. Cancel\n";
}

void ConsoleView::ShowChestOpenResult(const Chest& aChest, const std::vector<ItemInstance>& someItems)
{
    std::cout << "You open the " << aChest.GetName() << ".";
    if (someItems.empty())
    {
        std::cout << " It is empty.\n";
        return;
    }

    std::cout << " Inside you find:\n";
    for (const ItemInstance& item : someItems)
    {
        std::cout << "- " << FormatItemStack(item) << "\n";
    }
}

void ConsoleView::ShowNoClosedChests()
{
    std::cout << "There are no unopened chests here.\n";
}

void ConsoleView::ShowEnemies(const std::vector<Enemy>& someEnemies)
{
    if (someEnemies.empty())
    {
        return;
    }

    std::cout << "\nEnemies in the room:\n";
    for (size_t index = 0; index < someEnemies.size(); ++index)
    {
        const Enemy& enemy = someEnemies[index];
        std::cout << index + 1 << ". " << enemy.PrintStats();
        std::cout << " | HP: " << enemy.GetDamagable().GetHealth() << "/" << enemy.GetDamagable().GetMaxHealth() << "\n";
    }
}

void ConsoleView::ShowPlayerStats(const Player& aPlayer)
{
    std::cout << "\nYour stats:\n" << aPlayer.PrintStats();
    if (aPlayer.HasActiveEnchantments())
    {
        std::cout << "Active enchantments: " << aPlayer.GetActiveEnchantmentsSummary() << "\n";
    }
}

void ConsoleView::ShowCombatMenu()
{
    std::cout << "\nChoose your action:\n"
              << "1. Attack\n"
              << "2. Defend\n"
              << "3. Use item\n";
}

void ConsoleView::ShowNoEnemiesToAttack()
{
    std::cout << "There are no enemies to attack.\n";
}

void ConsoleView::ShowCombatVictory()
{
    std::cout << "All enemies defeated!\n";
}

void ConsoleView::ShowCombatDefeat()
{
    std::cout << "You have been slain!\n";
}

void ConsoleView::Pause()
{
    std::cin.ignore(1000000, '\n');
}
