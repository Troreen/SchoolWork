#pragma once
#include <random>
#include <array>
#include <Windows.h>
#include <string>

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
    Exit
};

enum class MenuOption {
    GuessTheNumber = 1,
    OddOrEven = 2,
    BlackJack = 3,
    Exit = 4
};

void DrawHUD(int playerMoney, const std::array<signed int, 5>& globalStatHistory);
void HandlePlayerMoney(int& playerMoney, int& playerBet, int aMoney);
void Bet(int& playerMoney, int& playerBet);
GameState HandleBankruptcy(int playerMoney, const std::array<signed int, 5>& globalStatHistory);
bool RecognizePlayer(GameState aState, int winningsGuessTheNumber, int winningsOddOrEven, int winningsBlackJack);
void UpdatePlayerStatHistory(std::array<signed int, 5>& globalStatHistory, int anAmount);
int GetInput(int aMinNum, int aMaxNum, const char* aPrompt, const char* aFailPrompt);
int RollDie(std::mt19937& aGenerator);
void ShowMenu();
GameState MenuState(int& playerMoney, int& playerBet, std::array<signed int, 5>& globalStatHistory);
void ShowInstructions(GameState aState);
const char* GetLossTaunt(int aStreak);
const char* GetWinTaunt(int aWinCounter);
