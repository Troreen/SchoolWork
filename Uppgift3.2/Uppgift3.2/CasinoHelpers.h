#pragma once

#include <array>
#include <random>
#include <string>

namespace CasinoHelpers
{
    extern const int globalChoiceNo;
    extern const int globalChoiceYes;
    extern const int globalPlayAgainNo;
    extern const int globalPlayAgainYes;
    extern const int globalPlayerInitialMoney;

    using StatHistory = std::array<signed int, 5>;

    enum class GameState
    {
        Menu,
        GuessTheNumber,
        OddOrEven,
        BlackJack,
        SlotMachine,
        Roulette,
        Exit,
        Count
    };

    enum class MenuOption
    {
        GuessTheNumber = 1,
        OddOrEven = 2,
        BlackJack = 3,
        SlotMachine = 4,
        Roulette = 5,
        Exit = 0,
        Count
    };

    void DrawHud(int somePlayerMoney, const StatHistory& aStatHistory, const std::string& aPlayerName);
    void HandlePlayerMoney(int& somePlayerMoney, int& aPlayerBet, int aMoney);
    void Bet(int& somePlayerMoney, int& aPlayerBet, const std::string& aPlayerName);
    void Bet(int& somePlayerMoney, int& aPlayerBet, const std::string& aPlayerName, int aMinBet);
    void Bet(int& somePlayerMoney, int& aPlayerBet, const std::string& aPlayerName, int aMinBet, int aMaxBet);
    GameState HandleBankruptcy(int somePlayerMoney, const StatHistory& aStatHistory, const std::string& aPlayerName);
    bool RecognizePlayer(GameState aState, int someWinningsGuessTheNumber, int someWinningsOddOrEven, int someWinningsBlackJack, int someWinningsSlot, int someWinningsRoulette, const std::string& aPlayerName);
    void UpdatePlayerStatHistory(StatHistory& aStatHistory, int anAmount);
    int GetInput(int aMinNum, int aMaxNum, const char* aPrompt, const char* aFailPrompt);
    int RollDie(std::mt19937& aGenerator);
    void ShowMenu(const std::string& aPlayerName);
    GameState MenuState(int& somePlayerMoney, int& aPlayerBet, StatHistory& aStatHistory, const std::string& aPlayerName);
    void ShowInstructions(GameState aState, const std::string& aPlayerName);
    void ShowInstructions(GameState aState);
    const char* GetLossTaunt(int aStreak);
    const char* GetWinTaunt(int aWinCounter);
}
