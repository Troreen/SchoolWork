#include "GameplayState.h"

#include <iomanip>
#include <iostream>

#include "ConsoleView.h"
#include "InventoryTypes.h"
#include "StateId.h"
#include "StateStack.h"

GameplayState::GameplayState(GameContext& aContext)
    : IGameState(aContext)
{
}

void GameplayState::Update(StateStack& aStateStack)
{
    if (myContext.currentRoom->HasEnemies())
    {
        aStateStack.PushState(StateId::Combat);
        return;
    }

    ConsoleView::ClearScreen();
    const Command command = GetCommandInSafeRoom();
    ConsoleView::ClearScreen();
    ExecuteCommand(command, aStateStack);
}

GameplayState::Command GameplayState::GetCommandInSafeRoom() const
{
    while (true)
    {
        const bool hasFloorItems = myContext.currentRoom->HasFloorItems();
        const bool hasChests = myContext.currentRoom->HasUnopenedChest();
        int optionIndex = 1;
        std::vector<std::pair<int, Command>> options;
        std::vector<std::pair<int, std::string>> displayOptions;

        options.emplace_back(optionIndex, Command::Move);
        displayOptions.emplace_back(optionIndex++, "Move to another room");

        options.emplace_back(optionIndex, Command::Inspect);
        displayOptions.emplace_back(optionIndex++, "Inspect room");

        options.emplace_back(optionIndex, Command::Inventory);
        displayOptions.emplace_back(optionIndex++, "View inventory");

        const InventoryState& inventoryState = myContext.player.Inventory();
        const EquipmentState& equipment = myContext.player.Equipment();
        const bool hasDroppableItems = !inventoryState.items.empty()
            || equipment.hasMainHand
            || equipment.hasChest;
        if (hasDroppableItems)
        {
            options.emplace_back(optionIndex, Command::Drop);
            displayOptions.emplace_back(optionIndex++, "Drop items");
        }

        options.emplace_back(optionIndex, Command::Stats);
        displayOptions.emplace_back(optionIndex++, "View stats");

        if (hasFloorItems)
        {
            options.emplace_back(optionIndex, Command::Pickup);
            displayOptions.emplace_back(optionIndex++, "Pick up items");
        }

        if (hasChests)
        {
            options.emplace_back(optionIndex, Command::OpenChest);
            displayOptions.emplace_back(optionIndex++, "Open chest");
        }

        displayOptions.emplace_back(0, "Exit game");

        std::cout << "Exploration timer: " << std::fixed << std::setprecision(1) << GetElapsedSeconds() << " s\n";
        ConsoleView::ShowSafeRoomMenu(*myContext.currentRoom, myContext.player, displayOptions);

        const int choice = ReadInt("Choice: ");

        if (choice == 0)
        {
            return Command::Quit;
        }

        for (const auto& option : options)
        {
            if (option.first == choice)
            {
                return option.second;
            }
        }

        ConsoleView::ShowInvalidChoice();
    }
}

bool GameplayState::ExecuteCommand(Command aCommand, StateStack& aStateStack)
{
    switch (aCommand)
    {
    case Command::Quit:
        myContext.endMessage = "Gameplay ended after " + std::to_string(static_cast<int>(GetElapsedSeconds())) + " second(s).";
        aStateStack.PushState(StateId::End);
        return false;
    case Command::Move:
        return HandleMove();
    case Command::Inspect:
        InspectRoom();
        return true;
    case Command::Inventory:
        aStateStack.PushState(StateId::InventoryMenu);
        return true;
    case Command::Drop:
        DropItems();
        return true;
    case Command::Stats:
        ConsoleView::ShowPlayerStats(myContext.player);
        ConsoleView::Pause();
        return true;
    case Command::Pickup:
        PickUpItems();
        return true;
    case Command::OpenChest:
        OpenChest();
        return true;
    case Command::Invalid:
    default:
        ConsoleView::ShowInvalidChoice();
        return true;
    }
}

bool GameplayState::HandleMove()
{
    const std::vector<Door*>& doorsInRoom = myContext.currentRoom->GetDoors();

    if (doorsInRoom.empty())
    {
        ConsoleView::ShowNoExits();
        return true;
    }

    ConsoleView::ShowDoorList(doorsInRoom, myContext.currentRoom);
    const int choice = ReadInt("Choose exit: ");

    if (choice <= 0 || choice > static_cast<int>(doorsInRoom.size()))
    {
        ConsoleView::ShowInvalidChoice();
        return true;
    }

    Door* chosenDoor = doorsInRoom[static_cast<size_t>(choice - 1)];

    if (chosenDoor->IsLocked())
    {
        ConsoleView::ShowLockedDoorPrompt();
        char selection = 'n';
        std::cin >> selection;
        std::cin.ignore(1000000, '\n');

        if (selection == 'l' || selection == 'L')
        {
            const bool success = chosenDoor->TryUnlock(myContext.player);
            ConsoleView::ShowDoorUnlockResult(success);
            if (success)
            {
                chosenDoor->SetLocked(false);
                myContext.currentRoom = chosenDoor->GetOtherRoom(myContext.currentRoom);
                myContext.currentRoom->EnterRoom();
            }
            return true;
        }

        if (selection == 'b' || selection == 'B')
        {
            const bool success = chosenDoor->TryBreak(myContext.player);
            ConsoleView::ShowDoorBreakResult(success);
            if (success)
            {
                chosenDoor->SetLocked(false);
                myContext.currentRoom = chosenDoor->GetOtherRoom(myContext.currentRoom);
                myContext.currentRoom->EnterRoom();
            }
            return true;
        }

        ConsoleView::ShowDoorSkipMessage();
        return true;
    }

    myContext.currentRoom = chosenDoor->GetOtherRoom(myContext.currentRoom);
    myContext.currentRoom->EnterRoom();
    return true;
}

void GameplayState::InspectRoom() const
{
    ConsoleView::ShowRoomInspection(*myContext.currentRoom);
}

void GameplayState::DropItems()
{
    while (true)
    {
        InventoryState& inventory = myContext.player.Inventory();
        EquipmentState& equipment = myContext.player.Equipment();

        struct DropCandidate
        {
            enum class Type
            {
                Inventory,
                Equipment
            };

            Type type;
            size_t index;
            EquipmentSlot slot;
        };

        std::vector<DropCandidate> candidates;
        std::vector<std::pair<std::string, ItemInstance>> displayItems;
        candidates.reserve(inventory.items.size() + 2);
        displayItems.reserve(inventory.items.size() + 2);

        for (size_t i = 0; i < inventory.items.size(); ++i)
        {
            candidates.push_back({ DropCandidate::Type::Inventory, i, EquipmentSlot::MainHand });
            displayItems.emplace_back("Backpack", inventory.items[i]);
        }

        if (equipment.hasMainHand)
        {
            candidates.push_back({ DropCandidate::Type::Equipment, 0, EquipmentSlot::MainHand });
            const char* label = equipment.mainHand.equipped ? "Main Hand (equipped)" : "Main Hand";
            displayItems.emplace_back(label, equipment.mainHand);
        }

        if (equipment.hasChest)
        {
            candidates.push_back({ DropCandidate::Type::Equipment, 0, EquipmentSlot::Chest });
            const char* label = equipment.chest.equipped ? "Chest (equipped)" : "Chest";
            displayItems.emplace_back(label, equipment.chest);
        }

        if (candidates.empty())
        {
            ConsoleView::ShowNoItemsToDrop();
            return;
        }

        ConsoleView::ShowDropList(displayItems);
        const int choice = ReadInt("Drop which item?: ");

        if (choice == 0)
        {
            return;
        }

        if (choice < 0 || choice > static_cast<int>(candidates.size()))
        {
            ConsoleView::ShowInvalidChoice();
            continue;
        }

        const DropCandidate& selected = candidates[static_cast<size_t>(choice - 1)];
        bool dropped = false;

        switch (selected.type)
        {
        case DropCandidate::Type::Inventory:
        {
            const size_t itemIndex = selected.index;
            if (itemIndex >= inventory.items.size())
            {
                ConsoleView::ShowInvalidChoice();
                break;
            }

            ItemInstance droppedItem = inventory.items[itemIndex];
            droppedItem.equipped = false;
            myContext.currentRoom->AddFloorItem(droppedItem);
            const ItemSpec& spec = GetItemSpec(droppedItem.id);
            ConsoleView::ShowDropResult(spec, droppedItem.count);
            inventory.items.erase(inventory.items.begin() + static_cast<long long>(itemIndex));
            dropped = true;
            break;
        }
        case DropCandidate::Type::Equipment:
        {
            ItemInstance* slotItem = nullptr;
            bool* hasFlag = nullptr;

            if (selected.slot == EquipmentSlot::MainHand && equipment.hasMainHand)
            {
                slotItem = &equipment.mainHand;
                hasFlag = &equipment.hasMainHand;
            }
            else if (selected.slot == EquipmentSlot::Chest && equipment.hasChest)
            {
                slotItem = &equipment.chest;
                hasFlag = &equipment.hasChest;
            }

            if (slotItem == nullptr || hasFlag == nullptr)
            {
                ConsoleView::ShowInvalidChoice();
                break;
            }

            ItemInstance droppedItem = *slotItem;
            droppedItem.equipped = false;
            myContext.currentRoom->AddFloorItem(droppedItem);
            const ItemSpec& spec = GetItemSpec(droppedItem.id);
            ConsoleView::ShowDropResult(spec, droppedItem.count);
            *slotItem = ItemInstance{};
            *hasFlag = false;
            dropped = true;
            break;
        }
        }

        if (dropped)
        {
            myContext.player.RecalculateDerivedStats();
        }
    }
}

void GameplayState::PickUpItems()
{
    while (true)
    {
        std::vector<ItemInstance>& floorItems = myContext.currentRoom->FloorItems();
        if (floorItems.empty())
        {
            ConsoleView::ShowNoItemsMessage();
            return;
        }

        ConsoleView::ShowPickupList(floorItems);
        const int choice = ReadInt("Which item do you want to pick up?: ");

        if (choice == 0)
        {
            return;
        }

        if (choice < 0 || choice > static_cast<int>(floorItems.size()))
        {
            ConsoleView::ShowInvalidChoice();
            continue;
        }

        const size_t itemIndex = static_cast<size_t>(choice - 1);
        ItemInstance item = floorItems[itemIndex];
        const ItemSpec& spec = GetItemSpec(item.id);

        bool pickedUp = false;

        if (spec.type == ItemType::Enchantment)
        {
            myContext.player.AddActiveEnchantment({ item.id, spec.durationTurns });
            ConsoleView::ShowActivationMessage(spec);
            pickedUp = true;
        }
        else
        {
            if (!myContext.player.CanCarryItem(item))
            {
                ConsoleView::ShowCapacityExceededMessage(spec, myContext.player.GetRemainingCarryWeight());
                continue;
            }

            if (myContext.player.AddItem(item))
            {
                ConsoleView::ShowPickupResult(spec.name, item.count, true);
                pickedUp = true;
            }
            else
            {
                ConsoleView::ShowCapacityExceededMessage(spec, myContext.player.GetRemainingCarryWeight());
            }
        }

        if (pickedUp)
        {
            myContext.currentRoom->RemoveFloorItem(itemIndex);
            myContext.player.RecalculateDerivedStats();
        }
    }
}

void GameplayState::OpenChest()
{
    std::vector<Chest>& roomChests = myContext.currentRoom->Chests();
    std::vector<Chest*> closedChests;
    std::vector<const Chest*> displayChests;
    for (Chest& chest : roomChests)
    {
        if (!chest.IsOpened())
        {
            closedChests.push_back(&chest);
            displayChests.push_back(&chest);
        }
    }

    if (closedChests.empty())
    {
        ConsoleView::ShowNoClosedChests();
        return;
    }

    while (true)
    {
        ConsoleView::ShowChestList(displayChests);
        const int choice = ReadInt("Which chest do you want to open?: ");

        if (choice == 0)
        {
            return;
        }

        if (choice < 0 || choice > static_cast<int>(closedChests.size()))
        {
            ConsoleView::ShowInvalidChoice();
            continue;
        }

        Chest* chest = closedChests[static_cast<size_t>(choice - 1)];
        std::vector<ItemInstance> contents = chest->Open();
        ConsoleView::ShowChestOpenResult(*chest, contents);

        for (ItemInstance& item : contents)
        {
            item.equipped = false;
            myContext.currentRoom->AddFloorItem(item);
        }

        return;
    }
}

int GameplayState::ReadInt(const std::string& aPrompt) const
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
