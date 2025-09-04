#include <iostream>
#include <random>
#include <array>

#include <Windows.h>
#include <assert.h>
#include <string>

int globalPlayerMoney = 1000;
int globalPlayerBet = 0;
signed int globalWinningsGuessTheNumber = 0;
signed int globalWinningsOddOrEven = 0;
signed int globalWinningsBlackJack = 0;

std::array<signed int, 5> globalStatHistory = { 0, 0, 0, 0, 0 };

enum GameState
{
    GameState_Menu,
    GameState_GuessTheNumber,
    GameState_OddOrEven,
    GameState_BlackJack,
    GameState_Exit
};

void ShowMenu();
GameState MenuState();
void ShowInstructions(GameState aState);
const char* GetLossTaunt(int aStreak);
const char* GetWinTaunt(int aWinCounter);
int GetInput(int aMinNum, int aMaxNum, const char* aPrompt, const char* aFailPrompt);
int RollDie(std::mt19937& aGenerator);
GameState PlayGuessTheNumber(std::mt19937& aGenerator);
GameState PlayOddOrEven(std::mt19937& aGenerator);
void DrawHUD();
void HandlePlayerMoney(int aMoney);
void Bet();
GameState HandleBankruptcy();
bool RecognizePlayer(GameState aState);
void UpdatePlayerStatHistory(int anAmount);
std::array<int, 52> CreateDeck();
void ShuffleDeck(std::array<int, 52>& aDeck, const std::mt19937& aGenerator);
int GetCardValue(int aCardIndex);
int GetHandValue(std::array<int, 12>& aHand, int aCardCount);
int DealOneCard(const std::array<int, 52>& aDeck, int& deckTop);
GameState PlayBlackJack(std::mt19937& aGenerator);
void ShowHands(const std::array<int, 12>& aPlayer, int aPlayerCount, const std::array<int, 12>& aDealer, int aDealerCount, bool aRevealDealer);



int main()
{
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());

    DrawHUD();

    system("cls");
    GameState currentState = GameState_Menu;

    while (true)
    {
        switch (currentState)
        {
            case GameState_Menu:
            {
                currentState = MenuState();
                break;
            }
            case GameState_GuessTheNumber:
            {
                currentState = PlayGuessTheNumber(generator);
                std::cout << "\nThat's it, huh? Walking away while you still can.\n";
                std::cout << "Fair enough, kid. The alley's always open if you change your mind...\n";
                break;
            }
            case GameState_OddOrEven:
            {
                currentState = PlayOddOrEven(generator);
                std::cout << "\nStreet's that way, hotshot.\n";
                break;
            }
            case GameState_BlackJack:
            {
                currentState = PlayBlackJack(generator);
                std::cout << "\nStreet's that way, hotshot.\n";
                break;
            }
            case GameState_Exit:
            {
                system("cls");
                std::cout << "\n\nHere is how much money you're leaving with: " << globalPlayerMoney << "\n\n";
                system("pause");
                return 0;
            }
            default:
            {
                return 0;
            }
        }
    }
    return 0;
}


std::array<int, 52> CreateDeck()
{
    std::array<int, 52> deck { };
    for (int i = 0; i < 52; ++i)
    {
        deck[i] = i;
    }
    return deck;
}

void ShuffleDeck(std::array<int, 52>& aDeck, std::mt19937& aGenerator)
{
    for (int i = static_cast<int>(aDeck.size()) - 1; i > 0; i--)
    {
        std::uniform_int_distribution<int> distribution(0, i);
        int j = distribution(aGenerator);
        int tempVar = aDeck[i];
        aDeck[i] = aDeck[j];
        aDeck[j] = tempVar;
    }
}

int GetCardValue(int aCardIndex)
{
    int rank = (aCardIndex % 13) + 1;
    if (rank == 13)
    {
        rank = 11;
    }
    else if (rank > 9)
    {
        rank = 10;
    }
    else
    {
        rank++;
    }

    return rank;
}

int GetHandValue(std::array<int, 12>& aHand, int aCardCount)
{
    assert(aCardCount <= aHand.size());

    int total = 0;
    int aceCount = 0;
    for (int i = 0; i < aCardCount; i++)
    {
        int cardValue = GetCardValue(aHand[i]);
        total += cardValue;
        if (cardValue == 11)
        {
            aceCount++;
        }
    }
    while (total > 21 && aceCount > 0)
    {
        total -= 10;
        aceCount--;
    }

    return total;
}

int DealOneCard(const std::array<int, 52>& aDeck, int& aDeckTop)
{
    assert(aDeckTop < static_cast<int>(aDeck.size()));
    int card = aDeck[aDeckTop];
    ++aDeckTop;
    return card;

}

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

void DrawHUD()
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

    std::cout << "\t\t\t\t\t\tYour current money: " << globalPlayerMoney << '\n';
}

void UpdatePlayerStatHistory(int anAmount)
{
    for (size_t i = globalStatHistory.size(); i > 1; i--)
    {
        globalStatHistory[i - 1] = globalStatHistory[i - 2];
    }
    globalStatHistory[0] = anAmount;
}

void HandlePlayerMoney(int aMoney)
{
    globalPlayerMoney += aMoney;
    globalPlayerBet = 0;
}

GameState HandleBankruptcy()
{
    DrawHUD();
    std::cout << "\nYour pockets echo like an empty alley, kid. Not a chip left to your name.";
    std::cout << "\nThe house grins, the bones go quiet. Come back when your luck grows legs... and brings cash.\n\n";
    system("pause");
    return GameState_Exit;
}

void Bet()
{
    DrawHUD();

    std::cout << "\nSlide your stake across the felt, slick. The house is listening...";
    globalPlayerBet = GetInput(
        1, globalPlayerMoney,
        "How much do you wanna bet?",
        "You don't have the scratch for that. Pick a number you can actually cover."
    );

    int playerOldMoney = globalPlayerMoney;
    globalPlayerMoney -= globalPlayerBet;

    DrawHUD();

    if (globalPlayerBet == playerOldMoney)
    {
        std::cout << "Woah, feeling lucky are you?.. We'll see about that hotshot..";
    }
    else
    {
        std::cout << "Chips down. The room leans in. Let's see if the night likes you.";
    }
}

void ShowMenu()
{
    std::cout << "\n--- The Casino ---";
    std::cout << "\n1. Slide over to the Guessing Game table";
    std::cout << "\n2. Take a shot at Odd or Even";
    std::cout << "\n3. Try your luck in Blackjack";
    std::cout << "\n4. Leave while your shoes still match";
    std::cout << "\n---------------------\n";
}

GameState MenuState()
{
    if (globalPlayerMoney <= 0)
    {
        return HandleBankruptcy();
    }
    DrawHUD();
    ShowMenu();

    int choice = GetInput(1, 4, "Pick your poison", "Keep it tidy, pal - choose a valid option.");
    switch (choice)
    {
    case 1:
        DrawHUD();
        std::cout << "\nGuessing Game table it is. Bones are hungry tonight.";
        return GameState_GuessTheNumber;
    case 2:
        DrawHUD();
        std::cout << "Odd/Even corner, huh? Grease twirls his toothpick: pick a side.\n";
        return GameState_OddOrEven;
    case 3:
        DrawHUD();
        std::cout << "Entering blackjack table.\n";
        return GameState_BlackJack;
    case 4:
        std::cout << "Smart legs take smart exits. Door's that way.";
        return GameState_Exit;
    default:
        std::cout << "That choice ain't on the menu, friend.";
        return GameState_Menu;
    }
}

void ShowInstructions(GameState aState)
{
    switch (aState)
    {
    case GameState_GuessTheNumber:
        system("cls");
        std::cout << "Two dice, cold and cruel. You guess the sum (2-12).\n";
        std::cout << "Hit it right, and maybe you walk away with your pride intact.\n";
        std::cout << "Miss it, and the house laughs last...\n\n";
        break;
    case GameState_OddOrEven:
        system("cls");
        std::cout << "\n\nOdd or Even, simple as sin: roll the bones, call the parity.\n";
        std::cout << "Get it right, you grin; get it wrong, you learn.\n\n";
        break;

    case GameState_BlackJack:
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

static const char* RANKS[14] = { "", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
static const char* SUITS[4] = { "S", "H", "D", "C" };

void PrintCardColored(int aCardIndex, HANDLE consoleHandle, WORD defaultTextAttributes)
{
    int suit = aCardIndex / 13;
    int rank = (aCardIndex % 13) + 1;

    if (suit == 1 || suit == 2) {
        SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
    }
    else {
        SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    }

    std::cout << RANKS[rank] << SUITS[suit];

    SetConsoleTextAttribute(consoleHandle, defaultTextAttributes);
}


std::string CardToText(int aCardIndex)
{
    int suit = aCardIndex / 13;
    int rank = (aCardIndex % 13) + 1;

    return std::string(RANKS[rank]) + SUITS[suit];
}


void ShowHands(const std::array<int, 12>& aPlayerHand, int aPlayerCardCount, const std::array<int, 12>& aDealerHand, int aDealerCardCount, bool aRevealDealer)
{
    DrawHUD();
    std::cout << "\nYour hand: ";
    for (int i = 0; i < aPlayerCardCount; ++i) 
    {
        std::cout << CardToText(aPlayerHand[i]) << " ";
    }
    std::cout << "(Value: " << GetHandValue(const_cast<std::array<int, 12>&>(aPlayerHand), aPlayerCardCount) << ")\n";

    std::cout << "Dealer's hand: ";
    if (aRevealDealer) 
    {
        for (int i = 0; i < aDealerCardCount; ++i) 
        {
            std::cout << CardToText(aDealerHand[i]) << " ";
        }
        std::cout << "(Value: " << GetHandValue(const_cast<std::array<int, 12>&>(aDealerHand), aDealerCardCount) << ")";
    } 
    else 
    {
        if (aDealerCardCount > 0) 
        {
            std::cout << CardToText(aDealerHand[0]) << " ";
            for (int i = 1; i < aDealerCardCount; ++i) 
            {
                std::cout << "[Hidden] ";
            }
        }
        std::cout << "(Value: " << GetCardValue(aDealerHand[0]) << " + ?)";
    }
    std::cout << "\n";
}


bool RecognizePlayer(GameState aState)
{
    system("cls");
    DrawHUD();
    bool shouldBan = false;

    switch (aState) 
    {
        case GameState_GuessTheNumber:
        {
            if (globalWinningsGuessTheNumber > 500)
            {
                std::cout << "You have earned too much at this table, you are banned\n";
                shouldBan = true;
                system("pause");
            }
            else if (globalWinningsGuessTheNumber < -150)
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
        case GameState_OddOrEven:
        {
            if (globalWinningsOddOrEven > 500)
            {
                std::cout << "You have earned too much at this table, you are banned\n";
                shouldBan = true;
                system("pause");
            }
            else if (globalWinningsOddOrEven< -150)
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
        case GameState_BlackJack:
        {
            if (globalWinningsBlackJack > 1000)
            {
                std::cout << "You have earned too much at this table, you are banned\n";
                shouldBan = true;
                system("pause");
            }
            else if (globalWinningsBlackJack< -150)
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

GameState PlayGuessTheNumber(std::mt19937& aGenerator)
{
    int aSeeInstructions = GetInput(
        0, 1,
        "Do you want instructions? (0: No, 1: Yes)",
        "Don't test me, choose 0 for No or 1 for Yes"
    );
    if (aSeeInstructions)
    {
        ShowInstructions(GameState_GuessTheNumber);
    }
    int aAcceptRules = GetInput(
        0, 1,
        "Do you still want to play knowing the stakes? (0: No, 1: Yes)",
        "Give me a real answer. 0 for No, 1 for Yes."
    );
    if (!aAcceptRules)
    {
        return GameState_Menu;
    }

    int playAgain = 1;
    int lossStreak = 0;
    int winCounter = 0;
    while (playAgain)
    {
        if (globalPlayerMoney <= 0)
        {
            return HandleBankruptcy();
        }

        if (RecognizePlayer(GameState_GuessTheNumber))
        {
            return GameState_Menu;
        }

        Bet();

        int guess = GetInput(
            2, 12,
            "Lay your wager... what's the sum gonna be?",
            "Heh, don't play me for a fool... pick a number between 2 and 12."
        );

        DrawHUD();

        std::cout << "Hands off the table. Dice are talking now...";
        int die1 = RollDie(aGenerator);
        int die2 = RollDie(aGenerator);
        int sum = die1 + die2;
        std::cout << "\nThe dice hit the table...\n";
        std::cout << "Die 1: " << die1 << "\n";
        std::cout << "Die 2: " << die2 << "\n";
        std::cout << "Total showing: " << sum << "\n";

        if (guess == sum)
        {
            lossStreak = 0;
            ++winCounter;
            int payout = (globalPlayerBet * 3) / 2;
            HandlePlayerMoney(payout);
            globalWinningsGuessTheNumber += payout;
            UpdatePlayerStatHistory(payout);
            std::cout << GetWinTaunt(winCounter);
            std::cout << "House peels off a stack and slides it back: +" << (payout - globalPlayerBet)
                << ". Don't get cocky.\n";
        }
        else
        {
            winCounter = 0;
            ++lossStreak;
            std::cout << GetLossTaunt(lossStreak);
            std::cout << "House keeps your stake: -" << globalPlayerBet << ". Maybe the next alley's kinder.\n";
            globalWinningsGuessTheNumber -= globalPlayerBet;
            UpdatePlayerStatHistory(-globalPlayerBet);
            globalPlayerBet = 0;
            system("pause");
            if (globalPlayerMoney <= 0)
            {
                return HandleBankruptcy();
            }
        }
        playAgain = GetInput(
            0, 1,
            "\nYou feelin' bold enough for another round? (0: No, 1: Yes): ",
            "Eh-eh, only '0' for No or '1' for Yes. Try again..."
        );
        if (playAgain)
        {
            DrawHUD();
            std::cout << "Back for more punishment? I like your style...\n\n";
        }
        else
        {
            std::cout << ("\nSmart. Live to lose another night.");
        }
    }
    return GameState_Menu;
}

GameState PlayOddOrEven(std::mt19937& aGenerator)
{
    int seeInstructions = GetInput(
        0, 1,
        "Do you want instructions? (0: No, 1: Yes)",
        "Don't test me, choose 0 for No or 1 for Yes"
    );
    if (seeInstructions)
    {
        ShowInstructions(GameState_OddOrEven);
    }
    int acceptRules = GetInput(
        0, 1,
        "Do you still want to play knowing the stakes? (0: No, 1: Yes)",
        "Give me a real answer. 0 for No, 1 for Yes."
    );
    if (!acceptRules)
    {
        return GameState_Menu;
    }
    int playAgain = 1;
    int lossStreak = 0;
    int winCounter = 0;
    while (playAgain)
    {
        if (globalPlayerMoney <= 0)
        {
            return HandleBankruptcy();
        }

        if (RecognizePlayer(GameState_OddOrEven))
        {
            return GameState_Menu;
        }

        Bet();
        int guess = GetInput(
            1, 2,
            "Whisper it to me, pal: odd (1) or even (2)  -  both bones gotta match",
            "Use your fingers if you gotta - 1 for odd, 2 for even."
        );
        DrawHUD();
        std::cout << "Hands off the felt. Let the bones breathe...\n";
        int die1 = RollDie(aGenerator);
        int die2 = RollDie(aGenerator);
        std::cout << "\nThe dice kiss concrete...\n";
        std::cout << "Die 1: " << die1 << "\n";
        std::cout << "Die 2: " << die2 << "\n";
        bool bothOdd = (((die1 % 2) != 0) && ((die2 % 2) != 0));
        bool bothEven = (((die1 % 2) == 0) && ((die2 % 2) == 0));
        bool split1 = (((die1 % 2) != 0) && ((die2 % 2) == 0));
        bool split2 = (((die1 % 2) == 0) && ((die2 % 2) != 0));
        bool win = (((guess == 1) && bothOdd) || ((guess == 2) && bothEven));
        if (win)
        {
            lossStreak = 0;
            ++winCounter;
            int payout = globalPlayerBet * 3;
            HandlePlayerMoney(payout);
            UpdatePlayerStatHistory(payout);
            globalWinningsOddOrEven += payout;
            std::cout << "Grease whistles low: \"" << (bothEven ? "Both even" : "Both odd") << ". You threaded it.\"\n";
            std::cout << GetWinTaunt(winCounter);
            std::cout << "He shoves your stack over: +" << (payout - globalPlayerBet) << ".\n";
        }
        else if (split1 || split2)
        {
            winCounter = 0;
            ++lossStreak;
            UpdatePlayerStatHistory(-globalPlayerBet);
            globalWinningsOddOrEven -= globalPlayerBet;
            std::cout << "Grease shrugs: \"Split shoes - one odd, one even. House eats.\"\n";
            std::cout << "He pockets your bet like it owed him money: -" << globalPlayerBet << ".\n";
            globalPlayerBet = 0;
            system("pause");
            if (globalPlayerMoney <= 0)
            {
                return HandleBankruptcy();
            }
        }
        else
        {
            winCounter = 0;
            ++lossStreak;
            UpdatePlayerStatHistory(-globalPlayerBet);
            globalWinningsOddOrEven -= globalPlayerBet;
            std::cout << "Grease shrugs: \"Hah! Not your day is it? House eats.\"\n";
            std::cout << "He pockets your bet like it owed him money: -" << globalPlayerBet << ".\n";
            globalPlayerBet = 0;
            system("pause");
            if (globalPlayerMoney <= 0)
            {
                return HandleBankruptcy();
            }
        }
        playAgain = GetInput(
            0, 1,
            "\nYou sticking around this alley? (0: No, 1: Yes): ",
            "Eh-eh, it's binary, hotshot - '0' for No or '1' for Yes."
        );
        if (playAgain)
        {
            DrawHUD();
            std::cout << "Grease taps the rail. \"Again.\"\n\n";
        }
        else
        {
            std::cout << "\nGrease waves you off. \"Keep your shoes clean.\"\n";
        }
    }
    return GameState_Menu;
}

GameState PlayBlackJack(std::mt19937& aGenerator)
{

    int seeInstructions = GetInput(
        0, 1,
        "Do you want instructions? (0: No, 1: Yes)",
        "Don't test me, choose 0 for No or 1 for Yes"
    );
    if (seeInstructions)
    {
        ShowInstructions(GameState_BlackJack);
    }
    int acceptRules = GetInput(
        0, 1,
        "Do you still want to play knowing the stakes? (0: No, 1: Yes)",
        "Give me a real answer. 0 for No, 1 for Yes."
    );
    if (!acceptRules)
    {
        return GameState_Menu;
    }
    int playAgain = 1;
    int lossStreak = 0;
    int winCounter = 0;
    while (playAgain)
    {
        std::array<int, 52> deck = CreateDeck();
        ShuffleDeck(deck, aGenerator);

        std::array<int, 12> player; int playerCount = 0;
        std::array<int, 12> dealer; int dealerCount = 0;

        int topCard = 0;

        player[playerCount++] = DealOneCard(deck, topCard);
        dealer[dealerCount++] = DealOneCard(deck, topCard);
        player[playerCount++] = DealOneCard(deck, topCard);
        dealer[dealerCount++] = DealOneCard(deck, topCard);

        if (globalPlayerMoney <= 0)
        {
            return HandleBankruptcy();
        }

        if (RecognizePlayer(GameState_BlackJack))
        {
            return GameState_Menu;
        }

        Bet();
        int ongoingRound = 1;
        bool revealDealerCard = false;
        while (ongoingRound && GetHandValue(player, playerCount) < 21)
        {
            ShowHands(player, playerCount, dealer, dealerCount, revealDealerCard);
            int guess = GetInput(
                0, 1,
                "Whisper it to me, pal: stay (0) or hit (1)",
                "It ain't too hard, only two choices - 0 to stay, 1 to hit."
            );
            if (guess)
            {
                player[playerCount++] = DealOneCard(deck, topCard);
                if (GetHandValue(player, playerCount) > 21)
                {
                    revealDealerCard = true;
                    ShowHands(player, playerCount, dealer, dealerCount, revealDealerCard);
                    ongoingRound = 0;
                }
            }
            else
            {
                revealDealerCard = true;
                break;
            }
        }
        if (ongoingRound)
        {
            revealDealerCard = true;
            ShowHands(player, playerCount, dealer, dealerCount, revealDealerCard);
            while (GetHandValue(dealer, dealerCount) < 17)
            {
                dealer[dealerCount++] = DealOneCard(deck, topCard);
                ShowHands(player, playerCount, dealer, dealerCount, revealDealerCard);
            }
            ongoingRound = 0;
        }


        if (GetHandValue(player, playerCount) > 21)
        {
            ShowHands(player, playerCount, dealer, dealerCount, revealDealerCard);
            winCounter = 0;
            ++lossStreak;
            UpdatePlayerStatHistory(-globalPlayerBet);
            globalWinningsBlackJack -= globalPlayerBet;
            std::cout << "You busted" << "\n";
            std::cout << "You have lost: -" << globalPlayerBet << ".\n";
            globalPlayerBet = 0;
            system("pause");
            if (globalPlayerMoney <= 0)
            {
                return HandleBankruptcy();
            }
        }
        else if (GetHandValue(dealer, dealerCount) > 21)
        {
            ShowHands(player, playerCount, dealer, dealerCount, revealDealerCard);
            lossStreak = 0;
            ++winCounter;
            int payout = globalPlayerBet * 2;
            globalWinningsBlackJack += payout;
            HandlePlayerMoney(payout);
            UpdatePlayerStatHistory(payout);
            std::cout << "The Dealer bust you win.""\n";
            std::cout << GetWinTaunt(winCounter);
            std::cout << "You win: +" << (payout - globalPlayerBet) << ".\n";
        }
        else
        {
            ShowHands(player, playerCount, dealer, dealerCount, revealDealerCard);
            if (GetHandValue(player, playerCount) <= GetHandValue(dealer, dealerCount))
            {
                winCounter = 0;
                ++lossStreak;
                globalWinningsBlackJack -= globalPlayerBet;
                UpdatePlayerStatHistory(-globalPlayerBet);
                std::cout << "The dealer has a higher hand, you've lost" << "\n";
                std::cout << "You have lost: -" << globalPlayerBet << ".\n";
                globalPlayerBet = 0;
                system("pause");
                if (globalPlayerMoney <= 0)
                {
                    return HandleBankruptcy();
                }
            }
            else
            {
                lossStreak = 0;
                ++winCounter;
                int payout = globalPlayerBet * 2;
                globalWinningsBlackJack += payout;
                HandlePlayerMoney(payout);
                UpdatePlayerStatHistory(payout);
                std::cout << "You have the higher hand, you win." << "\n";
                std::cout << GetWinTaunt(winCounter);
                std::cout << "You win: +" << (payout - globalPlayerBet) << ".\n";
            }
        }
       
        playAgain = GetInput(
            0, 1,
            "\nYou sticking around this alley? (0: No, 1: Yes): ",
            "Eh-eh, it's binary, hotshot - '0' for No or '1' for Yes."
        );
        if (playAgain)
        {
            DrawHUD();
            std::cout << "Grease taps the rail. \"Again.\"\n\n";
        }
        else
        {
            std::cout << "\nGrease waves you off. \"Keep your shoes clean.\"\n";
        }
    }
    return GameState_Menu;
}
