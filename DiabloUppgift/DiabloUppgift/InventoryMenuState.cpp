#include "InventoryMenuState.h"

#include <iomanip>
#include <iostream>

#include "ConsoleView.h"
#include "InventoryTypes.h"
#include "StateStack.h"

InventoryMenuState::InventoryMenuState(GameContext& aContext)
    : IGameState(aContext)
{
}

void InventoryMenuState::Update(StateStack& aStateStack)
{
    while (true)
    {
        ConsoleView::ClearScreen();
        std::cout << "Inventory timer: " << std::fixed << std::setprecision(1) << GetElapsedSeconds() << " s\n";
        ConsoleView::ShowInventory(myContext.player);

        InventoryState& inventory = myContext.player.Inventory();
        const int choice = ReadInt("Choice: ");

        if (choice == 0)
        {
            aStateStack.PopState();
            return;
        }

        if (choice < 0 || choice > static_cast<int>(inventory.items.size()))
        {
            ConsoleView::ShowInvalidChoice();
            continue;
        }

        const size_t index = static_cast<size_t>(choice - 1);
        EquipFromInventory(index);
    }
}

bool InventoryMenuState::EquipFromInventory(size_t anIndex)
{
    InventoryState& inventory = myContext.player.Inventory();
    if (anIndex >= inventory.items.size())
    {
        ConsoleView::ShowInvalidChoice();
        return false;
    }

    ItemInstance item = inventory.items[anIndex];
    const ItemSpec& spec = GetItemSpec(item.id);

    if (!spec.hasSlot)
    {
        ConsoleView::ShowCannotEquipMessage();
        return false;
    }

    EquipmentState& equipment = myContext.player.Equipment();
    ItemInstance* slotItem = nullptr;
    bool* hasFlag = nullptr;
    const char* slotLabel = "";

    switch (spec.slot)
    {
    case EquipmentSlot::MainHand:
        slotItem = &equipment.mainHand;
        hasFlag = &equipment.hasMainHand;
        slotLabel = "main hand";
        break;
    case EquipmentSlot::Chest:
        slotItem = &equipment.chest;
        hasFlag = &equipment.hasChest;
        slotLabel = "chest";
        break;
    default:
        ConsoleView::ShowCannotEquipMessage();
        return false;
    }

    if (!(*hasFlag))
    {
        item.equipped = true;
        *slotItem = item;
        *hasFlag = true;
        inventory.items.erase(inventory.items.begin() + static_cast<long long>(anIndex));
        ConsoleView::ShowEquipmentResult(spec, slotLabel, true);
        myContext.player.RecalculateDerivedStats();
        return true;
    }

    const ItemSpec& currentSpec = GetItemSpec(slotItem->id);
    ConsoleView::ShowEquipmentPrompt(currentSpec, spec, slotLabel);
    char answer = 'n';
    std::cin >> answer;
    std::cin.ignore(1000000, '\n');

    if (answer != 'y' && answer != 'Y')
    {
        ConsoleView::ShowEquipmentResult(spec, slotLabel, false);
        return true;
    }

    ItemInstance previous = *slotItem;
    previous.equipped = false;

    inventory.items.erase(inventory.items.begin() + static_cast<long long>(anIndex));
    inventory.items.insert(inventory.items.begin() + static_cast<long long>(anIndex), previous);

    item.equipped = true;
    *slotItem = item;
    *hasFlag = true;

    ConsoleView::ShowEquipmentResult(spec, slotLabel, true);
    myContext.player.RecalculateDerivedStats();
    return true;
}

int InventoryMenuState::ReadInt(const std::string& aPrompt) const
{
    while (true)
    {
        std::cout << aPrompt;
        int value = 0;
        if (std::cin >> value)
        {
            std::cin.ignore(1000000, '\n');
            return value;
        }

        std::cin.clear();
        std::cin.ignore(1000000, '\n');
        std::cout << "Please enter a number.\n";
    }
}
