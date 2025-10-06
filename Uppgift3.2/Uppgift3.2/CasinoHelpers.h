#pragma once

#include <array>
#include <random>
#include <string>

namespace CasinoHelpers
{
    inline constexpr int globalChoiceNo = 0;
    inline constexpr int globalChoiceYes = 1;
    inline constexpr int globalPlayAgainNo = 0;
    inline constexpr int globalPlayAgainYes = 1;
    inline constexpr int globalPlayerInitialMoney = 1000;
    inline constexpr int globalStatHistorySize = 5;

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

    void DrawHud(int somePlayerMoney, const std::array<signed int, globalStatHistorySize>& aStatHistory, const std::string& aPlayerName);
    void HandlePlayerMoney(int& somePlayerMoney, int& aPlayerBet, int aMoney);
    void Bet(int& somePlayerMoney, int& aPlayerBet, const std::string& aPlayerName, int aMinBet = 1, int aMaxBet = -1);
    GameState HandleBankruptcy(int somePlayerMoney, const std::array<signed int, globalStatHistorySize>& aStatHistory, const std::string& aPlayerName);
    bool RecognizePlayer(GameState aState, int someWinningsGuessTheNumber, int someWinningsOddOrEven, int someWinningsBlackJack, int someWinningsSlot, int someWinningsRoulette, const std::string& aPlayerName);
    void UpdatePlayerStatHistory(std::array<signed int, globalStatHistorySize>& aStatHistory, int anAmount);
    int GetInput(int aMinNum, int aMaxNum, const char* aPrompt, const char* aFailPrompt);
    int RollDie(std::mt19937& aGenerator);
    void ShowMenu(const std::string& aPlayerName);
    GameState MenuState(int& somePlayerMoney, int& aPlayerBet, std::array<signed int, globalStatHistorySize>& aStatHistory, const std::string& aPlayerName);
    void ShowInstructions(GameState aState, const std::string& aPlayerName);
    void ShowInstructions(GameState aState);
    const char* GetLossTaunt(int aStreak);
    const char* GetWinTaunt(int aWinCounter);
}
