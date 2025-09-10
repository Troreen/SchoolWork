#pragma once
#include <random>
#include <array>
#include <Windows.h>
#include <string>

namespace CasinoHelpers
{
    const int CHOICE_NO = 0;
    const int CHOICE_YES = 1;
    const int PLAY_AGAIN_NO = 0;
    const int PLAY_AGAIN_YES = 1;
    const int PLAYER_INITIAL_MONEY = 1000;
    const int STAT_HISTORY_SIZE = 5;

    enum class GameState {
        Menu,
        GuessTheNumber,
        OddOrEven,
        BlackJack,
        SlotMachine,
        Exit
    };

    enum class MenuOption {
        GuessTheNumber = 1,
        OddOrEven = 2,
        BlackJack = 3,
        SlotMachine = 4,
        Exit = 0
    };

    void DrawHUD(int somePlayerMoney, const std::array<signed int, 5>& aStatHistory);
    void HandlePlayerMoney(int& somePlayerMoney, int& aPlayerBet, int aMoney);
    void Bet(int& somePlayerMoney, int& aPlayerBet);
    GameState HandleBankruptcy(int somePlayerMoney, const std::array<signed int, 5>& aStatHistory);
    bool RecognizePlayer(GameState aState, int someWinningsGuessTheNumber, int someWinningsOddOrEven, int someWinningsBlackJack, int someWinningsSlot);
    void UpdatePlayerStatHistory(std::array<signed int, 5>& aStatHistory, int anAmount);
    int GetInput(int aMinNum, int aMaxNum, const char* aPrompt, const char* aFailPrompt);
    int RollDie(std::mt19937& aGenerator);
    void ShowMenu();
    GameState MenuState(int& somePlayerMoney, int& aPlayerBet, std::array<signed int, 5>& aStatHistory);
    void ShowInstructions(GameState aState);
    const char* GetLossTaunt(int aStreak);
    const char* GetWinTaunt(int aWinCounter);
}