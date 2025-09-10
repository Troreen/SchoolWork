#pragma once
#include <array>
#include <random>
#include <string>
#include <iostream>
#include <Windows.h>

namespace CasinoHelpers {
    enum class GameState { Menu, BlackJack };
    const int CHOICE_NO = 0;
    const int CHOICE_YES = 1;
    const int PLAY_AGAIN_YES = 1;
    const int PAYOUT_MULTIPLIER = 2;
    int GetInput(int min, int max, const char* prompt, const char* errorMsg);
    void ShowInstructions(GameState state);
    void DrawHUD(int playerMoney, const std::array<signed int, 5>& globalStatHistory);
    void Bet(int& playerMoney, int& playerBet);
    void UpdatePlayerStatHistory(std::array<signed int, 5>& globalStatHistory, int change);
    void HandlePlayerMoney(int& playerMoney, int& playerBet, int payout);
    GameState HandleBankruptcy(int& playerMoney, std::array<signed int, 5>& globalStatHistory);
    bool RecognizePlayer(GameState state, int a, int b, int winningsBlackJack);
    std::string GetWinTaunt(int winCounter);
}
