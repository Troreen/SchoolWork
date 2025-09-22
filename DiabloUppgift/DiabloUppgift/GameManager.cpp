#include "GameManager.h"
#include "Enemy.h"

GameManager::GameManager()
    : player("Hero", 5, 5, 5), currentRoomIndex(0)
{
    // Room names
    roomNames[0] = "Central Chamber";
    roomNames[1] = "Northern Hall";
    roomNames[2] = "Eastern Hall";
    roomNames[3] = "Southern Hall";
    roomNames[4] = "Western Hall";

    // Room connections: -1 means no room in that direction
    // Index: 0=Central, 1=North, 2=East, 3=South, 4=West
    // Directions: 0=North, 1=East, 2=South, 3=West
    roomConnections[0][0] = 1; // Central north -> North
    roomConnections[0][1] = 2; // Central east -> East
    roomConnections[0][2] = 3; // Central south -> South
    roomConnections[0][3] = 4; // Central west -> West

    roomConnections[1][0] = -1; // North has no north
    roomConnections[1][1] = -1;
    roomConnections[1][2] = 0; // North south -> Central
    roomConnections[1][3] = -1;

    roomConnections[2][0] = -1;
    roomConnections[2][1] = -1;
    roomConnections[2][2] = -1;
    roomConnections[2][3] = 0; // East west -> Central

    roomConnections[3][0] = 0; // South north -> Central
    roomConnections[3][1] = -1;
    roomConnections[3][2] = -1;
    roomConnections[3][3] = -1;

    roomConnections[4][0] = -1;
    roomConnections[4][1] = 0; // West east -> Central
    roomConnections[4][2] = -1;
    roomConnections[4][3] = -1;

    // Add 0-2 enemies to each room (except central room for demo)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> enemyCountDist(0, 2);
    for (int i = 0; i < 5; ++i) {
        int enemyCount = (i == 0) ? 0 : enemyCountDist(gen); // Central room has 0 enemies
        for (int j = 0; j < enemyCount; ++j) {
            std::string enemyName = "Goblin " + std::to_string(j + 1);
            Enemy enemy(enemyName, 3 + j, 2 + j, 2 + j); // Example stats
            rooms[i].AddEnemy(enemy);
        }
    }
}

void GameManager::PrintRoomContents()
{
    const auto& enemies = rooms[currentRoomIndex].GetEnemies();
    if (enemies.empty()) {
        std::cout << "The room is quiet. No enemies here.\n";
    } else {
        std::cout << "Enemies in this room:";
        for (size_t i = 0; i < enemies.size(); ++i) {
            std::cout << "\n " << (i+1) << ". " << enemies[i].GetName();
        }
        std::cout << "\n";
    }
}

void GameManager::PrintCurrentRoomAndPrompt()
{
    std::cout << "\nYou are in: " << roomNames[currentRoomIndex] << "\n";
    PrintRoomContents();
}

int GameManager::GetDirectionInput()
{
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        if (input == "8") return 0;
        if (input == "6") return 1;
        if (input == "2") return 2;
        if (input == "4") return 3;
        std::cout << "Invalid input. Please enter 8(N), 6(E), 2(S), or 4(W): ";
    }
}

void GameManager::PrintEnemiesWithStats(const std::vector<Enemy>& enemies)
{
    for (size_t i = 0; i < enemies.size(); ++i) {
        const Enemy& e = enemies[i];
        std::cout << i+1 << ". " << e.GetName() << " (STR: " << e.GetStrength()
                  << ", DEX: " << e.GetDexterity() << ", BODY: " << e.GetBody() << ", HP: "
                  << e.GetDamagable().GetHealth() << "/" << e.GetDamagable().GetMaxHealth() << ")\n";
    }
}

void GameManager::MainLoop()
{
    while (true) {
        PrintCurrentRoomAndPrompt();
        auto& enemies = rooms[currentRoomIndex].GetEnemies();
        bool hasEnemies = !enemies.empty();
        std::cout << (hasEnemies ? "What do you want to do? (f = fight, m = move): " : "No enemies. You can only move. (m = move): ");
        std::string action;
        std::getline(std::cin, action);
        if (action == "m") {
            std::cout << "Where do you want to go? (8=N, 6=E, 2=S, 4=W): ";
            int dir = GetDirectionInput();
            int nextRoom = roomConnections[currentRoomIndex][dir];
            if (nextRoom == -1) {
                std::cout << "There's a wall there.\n";
                continue;
            }
            currentRoomIndex = nextRoom;
        } else if (action == "f" && hasEnemies) {
            std::cout << "Choose an enemy to attack:\n";
            PrintEnemiesWithStats(enemies);
            std::cout << "Enter enemy number: ";
            int choice = 0;
            while (true) {
                std::string input;
                std::getline(std::cin, input);
                try {
                    choice = std::stoi(input);
                } catch (...) {
                    choice = 0;
                }
                if (choice >= 1 && choice <= (int)enemies.size()) break;
                std::cout << "Invalid choice. Enter a valid enemy number: ";
            }
            // Player always deals 5 damage for now
            enemies[choice-1].TakeDamage(5);
            std::cout << "You attack " << enemies[choice-1].GetName() << " for 5 damage!\n";
            if (enemies[choice-1].GetDamagable().GetHealth() <= 0) {
                std::cout << enemies[choice-1].GetName() << " is defeated!\n";
                enemies.erase(enemies.begin() + (choice-1));
            }
        } else {
            std::cout << "Invalid action.\n";
        }
    }
}
