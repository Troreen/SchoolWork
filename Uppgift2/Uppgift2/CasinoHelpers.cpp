#include "CasinoHelpers.h"
#include <array>
#include <Windows.h>
#include <iostream>
#include <string>

namespace CasinoHelpers
{
    void PrintIntegerBySign(int aValue, HANDLE aConsoleHandle, WORD someDefaultTextAttributes)
    {
        WORD chosenAttributes;
        if (aValue > 0) {
            chosenAttributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        }
        else if (aValue < 0) {
            chosenAttributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
        }
        else {
            chosenAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        }
        SetConsoleTextAttribute(aConsoleHandle, chosenAttributes);
        std::cout << aValue;
        SetConsoleTextAttribute(aConsoleHandle, someDefaultTextAttributes);
    }

    void DrawHUD(int playerMoney, const std::array<signed int, 5>& globalStatHistory)
    {
        system("cls");
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO screenInfo{};
        WORD defaultTextAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        if (GetConsoleScreenBufferInfo(consoleHandle, &screenInfo)) {
            defaultTextAttributes = screenInfo.wAttributes;
        }
        std::cout << "  ";
        for (size_t index = 0; index < globalStatHistory.size(); ++index) {
            int currentValue = globalStatHistory[index];
            PrintIntegerBySign(currentValue, consoleHandle, defaultTextAttributes);
            std::cout << '\t';
        }
        std::cout << "\t\t\t\t\t\tYour current money: " << playerMoney << '\n';
    }

    void UpdatePlayerStatHistory(std::array<signed int, 5>& globalStatHistory, int anAmount)
    {
        for (size_t i = globalStatHistory.size(); i > 1; i--)
        {
            globalStatHistory[i - 1] = globalStatHistory[i - 2];
        }
        globalStatHistory[0] = anAmount;
    }

    void HandlePlayerMoney(int& playerMoney, int& playerBet, int aMoney)
    {
        playerMoney += aMoney;
        playerBet = 0;
    }

    GameState HandleBankruptcy(int playerMoney, const std::array<signed int, 5>& globalStatHistory)
    {
        DrawHUD(playerMoney, globalStatHistory);
        std::cout << "\nYour pockets echo like an empty alley, kid. Not a chip left to your name.";
        std::cout << "\nThe house grins, the bones go quiet. Come back when your luck grows legs... and brings cash.\n\n";
        system("pause");
        return GameState::Exit;
    }

    void Bet(int& playerMoney, int& playerBet)
    {
        DrawHUD(playerMoney, std::array<signed int, 5>{}); // stat history not needed for bet HUD
        std::cout << "\nSlide your stake across the felt, slick. The house is listening...";
        playerBet = GetInput(
            1, playerMoney,
            "How much do you wanna bet?",
            "You don't have the scratch for that. Pick a number you can actually cover."
        );
        int playerOldMoney = playerMoney;
        playerMoney -= playerBet;
        DrawHUD(playerMoney, std::array<signed int, 5>{});
        if (playerBet == playerOldMoney)
        {
            std::cout << "Woah, feeling lucky are you?.. We'll see about that hotshot..";
        }
        else
        {
            std::cout << "Chips down. The room leans in. Let's see if the night likes you.";
        }
    }

    bool RecognizePlayer(GameState aState, int winningsGuessTheNumber, int winningsOddOrEven, int winningsBlackJack)
    {
        system("cls");
        // stat history not needed for HUD here
        bool shouldBan = false;
        switch (aState)
        {
        case GameState::GuessTheNumber:
        {
            if (winningsGuessTheNumber > 500)
            {
                std::cout << "You have earned too much at this table, you are banned\n";
                shouldBan = true;
                system("pause");
            }
            else if (winningsGuessTheNumber < -150)
            {
                std::cout << "Come right on.\n";
                system("pause");
            }
            else
            {
                std::cout << "Welcome, the night is young..\n";
                system("pause");
            }
            break;
        }
        case GameState::OddOrEven:
        {
            if (winningsOddOrEven > 500)
            {
                std::cout << "You have earned too much at this table, you are banned\n";
                shouldBan = true;
                system("pause");
            }
            else if (winningsOddOrEven < -150)
            {
                std::cout << "Come right on.\n";
                system("pause");
            }
            else
            {
                std::cout << "Welcome, the night is young..\n";
                system("pause");
            }
            break;
        }
        case GameState::BlackJack:
        {
            if (winningsBlackJack > 1000)
            {
                std::cout << "You have earned too much at this table, you are banned\n";
                shouldBan = true;
                system("pause");
            }
            else if (winningsBlackJack < -150)
            {
                std::cout << "Come right on.\n";
                system("pause");
            }
            else
            {
                std::cout << "Welcome, the night is young..\n";
                system("pause");
            }
            break;
        }
        default:
        {
            break;
        }
        }
        return shouldBan;
    }

    int GetInput(int aMinNum, int aMaxNum, const char* aPrompt, const char* aFailPrompt)
    {
        int value;
        while (true)
        {
            std::cout << '\n' << aPrompt << " (" << aMinNum << "-" << aMaxNum << "): ";
            std::cin >> value;
            if (value >= aMinNum && value <= aMaxNum)
            {
                break;
            }
            else
            {
                std::cout << '\n' << aFailPrompt;
                std::cin.clear();
                std::cin.ignore(10000, '\n');
            }
        }
        return value;
    }

    int RollDie(std::mt19937& aGenerator)
    {
        std::uniform_int_distribution<> distribution(1, 6);
        return distribution(aGenerator);
    }

void ShowMenu()
{
    std::cout << "\n--- The Casino ---";
    std::cout << "\n1. Slide over to the Guessing Game table";
    std::cout << "\n2. Take a shot at Odd or Even";
    std::cout << "\n3. Try your luck in Blackjack";
    std::cout << "\n4. Spin the Slot Machine";
    std::cout << "\n0. Leave while your shoes still match";
    std::cout << "\n---------------------\n";
}

GameState MenuState(int& playerMoney, int& playerBet, std::array<signed int, 5>& globalStatHistory)
{
    if (playerMoney <= 0)
    {
        return HandleBankruptcy(playerMoney, globalStatHistory);
    }
    DrawHUD(playerMoney, globalStatHistory);
    ShowMenu();
    int choiceInt = GetInput(0, 4, "Pick your poison", "Keep it tidy, pal - choose a valid option.");
    MenuOption choice = static_cast<MenuOption>(choiceInt);
    switch (choice)
    {
    case MenuOption::GuessTheNumber:
        DrawHUD(playerMoney, globalStatHistory);
        std::cout << "\nGuessing Game table it is. Bones are hungry tonight.";
        return GameState::GuessTheNumber;
    case MenuOption::OddOrEven:
        DrawHUD(playerMoney, globalStatHistory);
        std::cout << "Odd/Even corner, huh? Grease twirls his toothpick: pick a side.\n";
        return GameState::OddOrEven;
    case MenuOption::BlackJack:
        DrawHUD(playerMoney, globalStatHistory);
        std::cout << "Entering blackjack table.\n";
        return GameState::BlackJack;
    case MenuOption::SlotMachine:
        DrawHUD(playerMoney, globalStatHistory);
        std::cout << "Entering the slot machines.\n";
        return GameState::SlotMachine;
    case MenuOption::Exit:
        std::cout << "Smart legs take smart exits. Door's that way.";
        return GameState::Exit;
    default:
        std::cout << "That choice ain't on the menu, friend.";
        return GameState::Menu;
    }
}

    void ShowInstructions(GameState aState)
    {
        switch (aState)
        {
        case GameState::GuessTheNumber:
            system("cls");
            std::cout << "Two dice, cold and cruel. You guess the sum (2-12).\n";
            std::cout << "Hit it right, and maybe you walk away with your pride intact.\n";
            std::cout << "Miss it, and the house laughs last...\n\n";
            break;
        case GameState::OddOrEven:
            system("cls");
            std::cout << "\n\nOdd or Even, simple as sin: roll the bones, call the parity.\n";
            std::cout << "Get it right, you grin; get it wrong, you learn.\n\n";
            break;
        case GameState::BlackJack:
            system("cls");
            std::cout << "Cards on the felt, nerves on edge. Your job: get as close to 21 as you dare.\n";
            std::cout << "Draw more cards if you're feeling lucky, but bust over 21 and the house grins.\n";
            std::cout << "Stay put if you think you've got the edge. Dealer plays to 17-higher hand wins, but don't get greedy...\n\n";
            break;
        default:
            break;
        }
    }

    const char* GetLossTaunt(int aStreak)
    {
        switch (aStreak)
        {
        case 1:
            return "\nHa! First swing and a miss. Beginner's luck didn't show up tonight.\n";
        case 2:
            return "\nOuch, two in a row. Starting to look like you're outta your league, pal...\n";
        case 3:
            return "\nThree down! You're bleedin' chips if this were the real table.\n";
        case 4:
            return "\nFour misses straight? Kid, the dice don't even respect you anymore.\n";
        case 5:
            return "\nThis is gettin' painful to watch. Ever thought about quittin' while you're still breathin'?\n";
        default:
            return "\nSeven losses deep and you're still here? Either you're fearless... or just plain foolish.\n";
        }
    }

    const char* GetWinTaunt(int aWinCounter)
    {
        if (aWinCounter % 3 == 1)
        {
            return "\nWell, look at that - you actually nailed it. Don't let it swell your head.\n";
        }
        else if (aWinCounter % 3 == 2)
        {
            return "\nHeh, even a busted watch is right twice a day. Nice shot, slick.\n";
        }
        else
        {
            return "\nLucky break... savor it. Luck's got short legs, doesn't run far.\n";
        }
    }
}
