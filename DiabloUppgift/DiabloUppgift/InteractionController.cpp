#include "InteractionController.h"

#include <iostream>

#include "ConsoleView.h"
#include "CombatComponent.h"
#include "InventoryTypes.h"

InteractionController::InteractionController(Player& aPlayer,
                                             std::vector<Room>& someRooms,
                                             Room*& aCurrentRoom,
                                             std::vector<Door*>& someDoors)
    : myPlayer(aPlayer)
    , myRooms(someRooms)
    , myCurrentRoom(aCurrentRoom)
    , myDoors(someDoors)
{
}

bool InteractionController::HandleTurn()
{
    ConsoleView::ClearScreen();
    const Command command = myCurrentRoom->HasEnemies() ? GetCommandWhenEnemies() : GetCommandInSafeRoom();
    ConsoleView::ClearScreen();
    return ExecuteCommand(command);
}

InteractionController::Command InteractionController::GetCommandWhenEnemies() const
{
    while (true)
    {
        std::vector<std::pair<int, Command>> options;
        std::vector<std::pair<int, std::string>> displayOptions;
        options.emplace_back(1, Command::Fight);
        displayOptions.emplace_back(1, "Fight enemies");
        displayOptions.emplace_back(0, "Exit game");

        ConsoleView::ShowCombatLayout(myPlayer, myCurrentRoom->Enemies(), displayOptions);

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

InteractionController::Command InteractionController::GetCommandInSafeRoom() const
{
    while (true)
    {
        const bool hasFloorItems = myCurrentRoom->HasFloorItems();
        const bool hasChests = myCurrentRoom->HasUnopenedChest();
        int optionIndex = 1;
        std::vector<std::pair<int, Command>> options;
        std::vector<std::pair<int, std::string>> displayOptions;

        options.emplace_back(optionIndex, Command::Move);
        displayOptions.emplace_back(optionIndex++, "Move to another room");

        options.emplace_back(optionIndex, Command::Inspect);
        displayOptions.emplace_back(optionIndex++, "Inspect room");

        options.emplace_back(optionIndex, Command::Inventory);
        displayOptions.emplace_back(optionIndex++, "View inventory");

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

        ConsoleView::ShowSafeRoomMenu(*myCurrentRoom, myPlayer, displayOptions);

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

bool InteractionController::ExecuteCommand(Command aCommand)
{
    switch (aCommand)
    {
    case Command::Quit:
        return false;
    case Command::Fight:
        return StartCombat();
    case Command::Move:
        return HandleMove();
    case Command::Inspect:
        InspectRoom();
        return true;
    case Command::Inventory:
        ShowInventory();
        return true;
    case Command::Stats:
        ConsoleView::ShowPlayerStats(myPlayer);
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

bool InteractionController::HandleMove()
{
    const std::vector<Door*>& doorsInRoom = myCurrentRoom->GetDoors();

    if (doorsInRoom.empty())
    {
        ConsoleView::ShowNoExits();
        return true;
    }

    ConsoleView::ShowDoorList(doorsInRoom, myCurrentRoom);
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
            const bool success = chosenDoor->TryUnlock(myPlayer);
            ConsoleView::ShowDoorUnlockResult(success);
            if (success)
            {
                chosenDoor->SetLocked(false);
                myCurrentRoom = chosenDoor->GetOtherRoom(myCurrentRoom);
                myCurrentRoom->EnterRoom();
            }
            return true;
        }

        if (selection == 'b' || selection == 'B')
        {
            const bool success = chosenDoor->TryBreak(myPlayer);
            ConsoleView::ShowDoorBreakResult(success);
            if (success)
            {
                chosenDoor->SetLocked(false);
                myCurrentRoom = chosenDoor->GetOtherRoom(myCurrentRoom);
                myCurrentRoom->EnterRoom();
            }
            return true;
        }

        ConsoleView::ShowDoorSkipMessage();
        return true;
    }

    myCurrentRoom = chosenDoor->GetOtherRoom(myCurrentRoom);
    myCurrentRoom->EnterRoom();
    return true;
}

void InteractionController::InspectRoom() const
{
    ConsoleView::ShowRoomInspection(*myCurrentRoom);
}

void InteractionController::ShowInventory() const
{
    ConsoleView::ShowInventory(myPlayer);
    ConsoleView::Pause();
}

void InteractionController::PickUpItems()
{
    while (true)
    {
        std::vector<ItemInstance>& floorItems = myCurrentRoom->FloorItems();
        if (floorItems.empty())
        {
            ConsoleView::ShowNoItemsMessage();
            return;
        }

        ConsoleView::ShowPickupList(floorItems);
        const int choice = ReadInt("Pick up which item?: ");

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
            myPlayer.AddActiveEnchantment({ item.id, spec.actionsDuration });
            ConsoleView::ShowActivationMessage(spec);
            pickedUp = true;
        }
        else if (spec.hasSlot)
        {
            auto equipItem = [&](EquipmentSlot slot, const ItemSpec& equipSpec, ItemInstance equipInstance) -> bool
            {
                EquipmentState& equipment = myPlayer.Equipment();
                ItemInstance* slotItem = nullptr;
                bool* hasFlag = nullptr;
                const char* slotLabel = "";
                const float newItemWeight = myPlayer.ComputeItemWeight(equipInstance);

                switch (slot)
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
                    if (!myPlayer.CanCarryAdditionalWeight(newItemWeight))
                    {
                        ConsoleView::ShowCapacityExceededMessage(equipSpec, myPlayer.GetRemainingCarryWeight());
                        return false;
                    }
                    equipInstance.equipped = true;
                    *slotItem = equipInstance;
                    *hasFlag = true;
                    ConsoleView::ShowEquipmentResult(equipSpec, slotLabel, true);
                    myPlayer.RecalculateDerivedStats();
                    return true;
                }

                const ItemSpec& currentSpec = GetItemSpec(slotItem->id);
                const float currentItemWeight = myPlayer.ComputeItemWeight(*slotItem);
                if (!myPlayer.CanCarryAdditionalWeight(newItemWeight - currentItemWeight))
                {
                    ConsoleView::ShowCapacityExceededMessage(equipSpec, myPlayer.GetRemainingCarryWeight());
                    return false;
                }
                ConsoleView::ShowEquipmentPrompt(currentSpec, equipSpec, slotLabel);
                char answer = 'n';
                std::cin >> answer;
                std::cin.ignore(1000000, '\n');

                if (answer == 'y' || answer == 'Y')
                {
                    ItemInstance dropped = *slotItem;
                    dropped.equipped = false;
                    myCurrentRoom->AddFloorItem(dropped);

                    equipInstance.equipped = true;
                    *slotItem = equipInstance;
                    *hasFlag = true;
                    ConsoleView::ShowEquipmentResult(equipSpec, slotLabel, true);
                    myPlayer.RecalculateDerivedStats();
                    return true;
                }

                ConsoleView::ShowEquipmentResult(equipSpec, slotLabel, false);
                return false;
            };

            pickedUp = equipItem(spec.slot, spec, item);
        }
        else
        {
            if (!myPlayer.CanCarryItem(item))
            {
                ConsoleView::ShowCapacityExceededMessage(spec, myPlayer.GetRemainingCarryWeight());
                continue;
            }

            if (myPlayer.AddItem(item))
            {
                ConsoleView::ShowPickupResult(spec.name, item.count, true);
                pickedUp = true;
            }
            else
            {
                ConsoleView::ShowCapacityExceededMessage(spec, myPlayer.GetRemainingCarryWeight());
            }
        }

        if (pickedUp)
        {
            myCurrentRoom->RemoveFloorItem(itemIndex);
            myPlayer.RecalculateDerivedStats();
        }
    }
}

void InteractionController::OpenChest()
{
    std::vector<Chest>& roomChests = myCurrentRoom->Chests();
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
        const int choice = ReadInt("Open which chest?: ");

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
            myCurrentRoom->AddFloorItem(item);
        }

        return;
    }
}

bool InteractionController::StartCombat()
{
    CombatComponent combat(myPlayer, myCurrentRoom->Enemies(), *myCurrentRoom);

    while (combat.GetResult() == CombatComponent::Result::ResultOngoing)
    {
        const std::vector<Enemy>& enemies = combat.GetEnemies();

        std::vector<std::pair<int, std::string>> combatOptions = {
            {1, "Attack"},
            {2, "Defend"},
            {3, "Use item"},
            {0, "Attempt escape"}
        };
        ConsoleView::ShowCombatLayout(myPlayer, enemies, combatOptions);

        const int actionInput = ReadInt("Choice: ");

        CombatComponent::Action action = CombatComponent::Action::ActionAttack;

        switch (actionInput)
        {
        case 1:
            action = CombatComponent::Action::ActionAttack;
            break;
        case 2:
            action = CombatComponent::Action::ActionDefend;
            break;
        case 3:
            action = CombatComponent::Action::ActionUseItem;
            break;
        default:
            ConsoleView::ShowInvalidChoice();
            continue;
        }

        if (action == CombatComponent::Action::ActionAttack)
        {
            if (enemies.empty())
            {
                ConsoleView::ShowNoEnemiesToAttack();
                continue;
            }

            const int enemyChoice = ReadInt("Choose enemy to attack: ");
            if (enemyChoice <= 0 || enemyChoice > static_cast<int>(enemies.size()))
            {
                ConsoleView::ShowInvalidChoice();
                continue;
            }

            const size_t enemyIndex = static_cast<size_t>(enemyChoice - 1);
            if (!combat.SelectEnemy(enemyIndex))
            {
                ConsoleView::ShowInvalidChoice();
                continue;
            }
        }

        const bool actionPerformed = combat.PerformPlayerAction(action);
        if (!actionPerformed)
        {
            continue;
        }

        if (combat.GetResult() != CombatComponent::Result::ResultOngoing)
        {
            break;
        }

        combat.PerformEnemyTurn();
    }

    switch (combat.GetResult())
    {
    case CombatComponent::Result::ResultPlayerWon:
        ConsoleView::ShowCombatVictory();
        myCurrentRoom->SetRoomCleared(true);
        return true;
    case CombatComponent::Result::ResultPlayerLost:
        ConsoleView::ShowCombatDefeat();
        return false;
    case CombatComponent::Result::ResultOngoing:
    case CombatComponent::Result::ResultCount:
        return true;
    }

    return true;
}

int InteractionController::ReadInt(const std::string& aPrompt) const
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
