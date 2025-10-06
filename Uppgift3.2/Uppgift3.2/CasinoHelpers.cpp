#include "CasinoHelpers.h"

#include <Windows.h>
#include <array>
#include <cstdlib>
#include <iostream>
#include <string>

namespace CasinoHelpers
{
    namespace
    {
        void PrintIntegerBySign(int aValue, HANDLE aConsoleHandle, WORD someDefaultTextAttributes)
        {
            WORD chosenAttributes{};
            if (aValue > 0)
            {
                chosenAttributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            }
            else if (aValue < 0)
            {
                chosenAttributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
            }
            else
            {
                chosenAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            }

            SetConsoleTextAttribute(aConsoleHandle, chosenAttributes);
            std::cout << aValue;
            SetConsoleTextAttribute(aConsoleHandle, someDefaultTextAttributes);
        }
    }

    void DrawHud(int somePlayerMoney, const std::array<signed int, globalStatHistorySize>& aStatHistory, const std::string& aPlayerName)
    {
        system("cls");
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO screenInfo{};
        WORD defaultTextAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        if (GetConsoleScreenBufferInfo(consoleHandle, &screenInfo))
        {
            defaultTextAttributes = screenInfo.wAttributes;
        }

        std::cout << "  ";
        for (signed int value : aStatHistory)
        {
            PrintIntegerBySign(value, consoleHandle, defaultTextAttributes);
            std::cout << '\t';
        }

        std::cout << "\t\t\t\t\t\t" << aPlayerName << "'s current money: " << somePlayerMoney << '\n';
    }

    void UpdatePlayerStatHistory(std::array<signed int, globalStatHistorySize>& aStatHistory, int anAmount)
    {
        for (size_t index = aStatHistory.size(); index > 1; --index)
        {
            aStatHistory[index - 1] = aStatHistory[index - 2];
        }

        aStatHistory[0] = anAmount;
    }

    void HandlePlayerMoney(int& somePlayerMoney, int& aPlayerBet, int aMoney)
    {
        somePlayerMoney += aMoney;
        aPlayerBet = 0;
    }

    GameState HandleBankruptcy(int somePlayerMoney, const std::array<signed int, globalStatHistorySize>& aStatHistory, const std::string& aPlayerName)
    {
        DrawHud(somePlayerMoney, aStatHistory, aPlayerName);
        std::cout << '\n' << aPlayerName << ", your pockets echo like an empty alley. Not a chip left to your name.";
        std::cout << "\nThe house grins. Come back when your luck grows legs... and brings cash.\n\n";
        system("pause");
        return GameState::Exit;
    }

    void Bet(int& somePlayerMoney, int& aPlayerBet, const std::string& aPlayerName, int aMinBet, int aMaxBet)
    {
        int effectiveMax = somePlayerMoney;
        if (aMaxBet > 0 && aMaxBet < effectiveMax)
        {
            effectiveMax = aMaxBet;
        }

        if (aMinBet > effectiveMax)
        {
            aPlayerBet = 0;
            std::cout << '\n' << aPlayerName << ", you cannot place a bet with the current limits.\n";
            system("pause");
            return;
        }

        std::cout << '\n' << aPlayerName << ", enter the amount you want to bet (minimum "
                  << aMinBet << ", maximum " << effectiveMax << "): ";
        aPlayerBet = GetInput(
            aMinBet,
            effectiveMax,
            "How much do you want to bet?",
            "You do not have enough money for that bet. Please enter a valid amount.");

        const int playerOldMoney = somePlayerMoney;
        somePlayerMoney -= aPlayerBet;
        if (aPlayerBet == playerOldMoney)
        {
            std::cout << aPlayerName << ", you have bet all your remaining money.\n";
        }
        else
        {
            std::cout << "Bet placed: " << aPlayerBet << ".\n";
        }
    }

    bool RecognizePlayer(GameState aState,
        int someWinningsGuessTheNumber,
        int someWinningsOddOrEven,
        int someWinningsBlackJack,
        int someWinningsSlot,
        int someWinningsRoulette,
        const std::string& aPlayerName)
    {
        system("cls");
        bool shouldBan = false;
        auto pauseWithMessage = [&](const std::string& aMessage)
        {
            std::cout << aPlayerName << ", " << aMessage << '\n';
            system("pause");
        };

        switch (aState)
        {
        case GameState::GuessTheNumber:
        {
            if (someWinningsGuessTheNumber > 500)
            {
                pauseWithMessage("you have earned too much at this table, you are banned.");
                shouldBan = true;
            }
            else if (someWinningsGuessTheNumber < -150)
            {
                pauseWithMessage("step right up.");
            }
            else
            {
                pauseWithMessage("welcome back; the night is young.");
            }

            break;
        }
        case GameState::OddOrEven:
        {
            if (someWinningsOddOrEven > 500)
            {
                pauseWithMessage("you have earned too much at this table, you are banned.");
                shouldBan = true;
            }
            else if (someWinningsOddOrEven < -150)
            {
                pauseWithMessage("step right up.");
            }
            else
            {
                pauseWithMessage("welcome back; the night is young.");
            }

            break;
        }
        case GameState::BlackJack:
        {
            if (someWinningsBlackJack > 1000)
            {
                pauseWithMessage("you have earned too much at this table, you are banned.");
                shouldBan = true;
            }
            else if (someWinningsBlackJack < -150)
            {
                pauseWithMessage("step right up.");
            }
            else
            {
                pauseWithMessage("welcome back; the night is young.");
            }

            break;
        }
        case GameState::SlotMachine:
        {
            if (someWinningsSlot > 1000)
            {
                pauseWithMessage("you have earned too much at this table, you are banned.");
                shouldBan = true;
            }
            else if (someWinningsSlot < -150)
            {
                pauseWithMessage("step right up.");
            }
            else
            {
                pauseWithMessage("welcome back; the night is young.");
            }

            break;
        }
        case GameState::Roulette:
        {
            if (someWinningsRoulette > 1000)
            {
                pauseWithMessage("you have earned too much at this table, you are banned.");
                shouldBan = true;
            }
            else if (someWinningsRoulette < -150)
            {
                pauseWithMessage("step right up.");
            }
            else
            {
                pauseWithMessage("welcome back; the night is young.");
            }

            break;
        }
        case GameState::Menu:
        case GameState::Exit:
        case GameState::Count:
        default:
            break;
        }

        return shouldBan;
    }

    int GetInput(int aMinNum, int aMaxNum, const char* aPrompt, const char* aFailPrompt)
    {
        int value = 0;
        while (true)
        {
            std::cout << '\n' << aPrompt << " (" << aMinNum << "-" << aMaxNum << "): ";
            std::cin >> value;
            if (value >= aMinNum && value <= aMaxNum)
            {
                break;
            }

            std::cout << '\n' << aFailPrompt;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        return value;
    }

    int RollDie(std::mt19937& aGenerator)
    {
        std::uniform_int_distribution<> distribution(1, 6);
        return distribution(aGenerator);
    }

    void ShowMenu(const std::string& aPlayerName)
    {
        std::cout << "\n--- " << aPlayerName << "'s Casino Menu ---\n";
        std::cout << "1. Guess The Number\n";
        std::cout << "2. Odd Or Even\n";
        std::cout << "3. BlackJack\n";
        std::cout << "4. Slot Machine\n";
        std::cout << "5. Roulette\n";
        std::cout << "0. Exit\n";
    }

    GameState MenuState(int& somePlayerMoney, int& aPlayerBet, std::array<signed int, globalStatHistorySize>& aStatHistory, const std::string& aPlayerName)
    {
        (void)somePlayerMoney;
        (void)aPlayerBet;
        (void)aStatHistory;
        const std::string prompt = aPlayerName + ", select a game by entering its number:";
        int choice = GetInput(0, 5, prompt.c_str(), "Please enter a number between 0 and 5.");
        switch (choice)
        {
        case 1:
            return GameState::GuessTheNumber;
        case 2:
            return GameState::OddOrEven;
        case 3:
            return GameState::BlackJack;
        case 4:
            return GameState::SlotMachine;
        case 5:
            return GameState::Roulette;
        case 0:
        default:
            return GameState::Exit;
        }
    }

    void ShowInstructions(GameState aState, const std::string& aPlayerName)
    {
        switch (aState)
        {
        case GameState::GuessTheNumber:
        {
            system("cls");
            std::cout << aPlayerName << ", two dice hit the table. Call the sum (2-12) before they settle.\n";
            std::cout << "Hit it right and you leave with extra chips; miss and the house takes its due.\n\n";
            break;
        }
        case GameState::OddOrEven:
        {
            system("cls");
            std::cout << aPlayerName << ", odd or even - simple call, sharp stakes.\n";
            std::cout << "Guess the parity of the roll. Right call pays, wrong one stings.\n\n";
            break;
        }
        case GameState::BlackJack:
        {
            system("cls");
            std::cout << aPlayerName << ", chase 21 without tumbling over.\n";
            std::cout << "Hit if you want another card, stand if you're content. Dealer draws to 17 - closest hand wins.\n\n";
            break;
        }
        case GameState::Roulette:
        {
            system("cls");
            std::cout << "Welcome to Roulette, " << aPlayerName << "! Here are the instructions:\n";
            std::cout << "1. Straight: Bet on a single number.\n";
            std::cout << "2. Split: Bet on two adjacent numbers.\n";
            std::cout << "3. Corner: Bet on four numbers forming a square.\n";
            std::cout << "4. Red/Black: Bet on the color of the number.\n";
            std::cout << "5. Odd/Even: Bet on whether the number is odd or even.\n";
            std::cout << "6. Column: Bet on one of the three vertical columns.\n";
            std::cout << "Good luck!\n";
            break;
        }
        case GameState::SlotMachine:
        case GameState::Menu:
        case GameState::Exit:
        case GameState::Count:
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

        return "\nLucky break... savor it. Luck's got short legs, doesn't run far.\n";
    }

    void ShowInstructions(GameState aState)
    {
        static const std::string anonymousPlayerName = "Player";
        ShowInstructions(aState, anonymousPlayerName);
    }
}
