#include <iostream>
#include <array>

namespace Poker
{
	std::array<int, 4> chips;
	std::array<int, 4> betThisRound;
	std::array<bool, 4> isFolded;
	std::array<std::array<int, 2>, 4> hands;
	std::array<int, 5> tableCards;

	int pot;
	int currentBet;
	int raisesThisRound;
	int dealerIndex;

	const int smallBlind;
	const int bigBlind;
	const int raiseCap;
	
	std::array<int, 52> deck;
	int topCard;


}
