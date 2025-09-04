#include <iostream>
#include <random>

int globalPlayerMoney = 100;
int globalPlayerBet = 0;

enum GameState
{
    Menu,
    GuessTheNumber,
    OddOrEven,
    Exit
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

int main()
{
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());

    system("cls");
    GameState currentState = Menu;

    while (true)
    {
        switch (currentState)
        {
            case Menu:
            {
                currentState = MenuState();
                break;
            }
            case GuessTheNumber:
            {
                currentState = PlayGuessTheNumber(generator);
                std::cout << "\nThat's it, huh? Walking away while you still can.\n";
                std::cout << "Fair enough, kid. The alley's always open if you change your mind...\n";
                break;
            }
            case OddOrEven:
            {
                currentState = PlayOddOrEven(generator);
                std::cout << "\nGrease snaps his toothpick and palms a chip. \"Street's that way, hotshot.\"\n";
                break;
            }
            case Exit:
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

void DrawHUD()
{
    system("cls");
    std::cout << "\n\t\t\t\t\t\t\t\t\t\t\t\tYour current money: " << globalPlayerMoney << '\n';
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
    return Exit;
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
    std::cout << "\n3. Leave while your shoes still match";
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

    int choice = GetInput(1, 3, "Pick your poison", "Keep it tidy, pal - choose a valid option.");
    switch (choice)
    {
    case 1:
        DrawHUD();
        std::cout << "\nGuessing Game table it is. Bones are hungry tonight.";
        return GuessTheNumber;
    case 2:
        DrawHUD();
        std::cout << "Odd/Even corner, huh? Grease twirls his toothpick: pick a side.\n";
        return OddOrEven;
    case 3:
        std::cout << "Smart legs take smart exits. Door's that way.";
        return Exit;
    default:
        std::cout << "That choice ain't on the menu, friend.";
        return Menu;
    }
}

void ShowInstructions(GameState aState)
{
    switch (aState)
    {
    case GuessTheNumber:
        system("cls");
        std::cout << "Two dice, cold and cruel. You guess the sum (2-12).\n";
        std::cout << "Hit it right, and maybe you walk away with your pride intact.\n";
        std::cout << "Miss it, and the house laughs last...\n\n";
        break;
    case OddOrEven:
        system("cls");
        std::cout << "\n\nOdd or Even, simple as sin: roll the bones, call the parity.\n";
        std::cout << "Get it right, you grin; get it wrong, you learn.\n\n";
        break;
    default:
        break;
    }
}

const char* GetLossTaunt(int aStreak)
{
    if (aStreak == 1)
    {
        return "\nHa! First swing and a miss. Beginner's luck didn't show up tonight.\n";
    }
    else if (aStreak == 2)
    {
        return "\nOuch, two in a row. Starting to look like you're outta your league, pal...\n";
    }
    else if (aStreak == 3)
    {
        return "\nThree down! You're bleedin' chips if this were the real table.\n";
    }
    else if (aStreak == 4)
    {
        return "\nFour misses straight? Kid, the dice don't even respect you anymore.\n";
    }
    else if (aStreak >= 5 && aStreak < 8)
    {
        return "\nThis is gettin' painful to watch. Ever thought about quittin' while you're still breathin'?\n";
    }
    else
    {
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
    int aValue;
    while (true)
    {
        std::cout << '\n' << aPrompt << " (" << aMinNum << "-" << aMaxNum << "): ";
        std::cin >> aValue;
        if (aValue >= aMinNum && aValue <= aMaxNum)
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
    return aValue;
}

int RollDie(std::mt19937& aGenerator)
{
    std::uniform_int_distribution<> distribution(1, 6);
    return distribution(aGenerator);
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
        ShowInstructions(GuessTheNumber);
    }
    int aAcceptRules = GetInput(
        0, 1,
        "Do you still want to play knowing the stakes? (0: No, 1: Yes)",
        "Give me a real answer. 0 for No, 1 for Yes."
    );
    if (!aAcceptRules)
    {
        return Menu;
    }

    int aPlayAgain = 1;
    int aLossStreak = 0;
    int aWinCounter = 0;
    while (aPlayAgain)
    {
        if (globalPlayerMoney <= 0)
        {
            return HandleBankruptcy();
        }

        Bet();

        int aGuess = GetInput(
            2, 12,
            "Lay your wager... what's the sum gonna be?",
            "Heh, don't play me for a fool... pick a number between 2 and 12."
        );

        DrawHUD();
        std::cout << "Hands off the table. Dice are talking now...";
        int aDie1 = RollDie(aGenerator);
        int aDie2 = RollDie(aGenerator);
        int sum = aDie1 + aDie2;
        std::cout << "\nThe dice hit the table...\n";
        std::cout << "Die 1: " << aDie1 << "\n";
        std::cout << "Die 2: " << aDie2 << "\n";
        std::cout << "Total showing: " << sum << "\n";
        if (aGuess == sum)
        {
            aLossStreak = 0;
            ++aWinCounter;
            int aPayout = (globalPlayerBet * 3) / 2;
            HandlePlayerMoney(aPayout);
            std::cout << GetWinTaunt(aWinCounter);
            std::cout << "House peels off a stack and slides it back: +" << (aPayout - globalPlayerBet)
                << ". Don't get cocky.\n";
        }
        else
        {
            aWinCounter = 0;
            ++aLossStreak;
            std::cout << GetLossTaunt(aLossStreak);
            std::cout << "House keeps your stake: -" << globalPlayerBet << ". Maybe the next alley's kinder.\n";
            globalPlayerBet = 0;
            system("pause");
            if (globalPlayerMoney <= 0)
            {
                return HandleBankruptcy();
            }
        }
        aPlayAgain = GetInput(
            0, 1,
            "\nYou feelin' bold enough for another round? (0: No, 1: Yes): ",
            "Eh-eh, only '0' for No or '1' for Yes. Try again..."
        );
        if (aPlayAgain)
        {
            DrawHUD();
            std::cout << "Back for more punishment? I like your style...\n\n";
        }
        else
        {
            std::cout << ("\nSmart. Live to lose another night.");
        }
    }
    return Menu;
}

GameState PlayOddOrEven(std::mt19937& aGenerator)
{
    int aSeeInstructions = GetInput(
        0, 1,
        "Do you want instructions? (0: No, 1: Yes)",
        "Don't test me, choose 0 for No or 1 for Yes"
    );
    if (aSeeInstructions)
    {
        ShowInstructions(OddOrEven);
    }
    int aAcceptRules = GetInput(
        0, 1,
        "Do you still want to play knowing the stakes? (0: No, 1: Yes)",
        "Give me a real answer. 0 for No, 1 for Yes."
    );
    if (!aAcceptRules)
    {
        return Menu;
    }
    std::cout << "\nName's Grease. I run this corner - quick calls, quicker losses.\n";
    std::cout << "Pick odd (1) or even (2). BOTH dice gotta match your call. No receipts, no witnesses.\n\n";
    int aPlayAgain = 1;
    int aLossStreak = 0;
    int aWinCounter = 0;
    while (aPlayAgain)
    {
        if (globalPlayerMoney <= 0)
        {
            return HandleBankruptcy();
        }
        Bet();
        int aGuess = GetInput(
            1, 2,
            "Whisper it to me, pal: odd (1) or even (2)  -  both bones gotta match",
            "Use your fingers if you gotta - 1 for odd, 2 for even."
        );
        DrawHUD();
        std::cout << "Hands off the felt. Let the bones breathe...\n";
        int aDie1 = RollDie(aGenerator);
        int aDie2 = RollDie(aGenerator);
        std::cout << "\nThe dice kiss concrete...\n";
        std::cout << "Die 1: " << aDie1 << "\n";
        std::cout << "Die 2: " << aDie2 << "\n";
        bool bothOdd = (((aDie1 % 2) != 0) && ((aDie2 % 2) != 0));
        bool bothEven = (((aDie1 % 2) == 0) && ((aDie2 % 2) == 0));
        bool split1 = (((aDie1 % 2) != 0) && ((aDie2 % 2) == 0));
        bool split2 = (((aDie1 % 2) == 0) && ((aDie2 % 2) != 0));
        bool win = (((aGuess == 1) && bothOdd) || ((aGuess == 2) && bothEven));
        if (win)
        {
            aLossStreak = 0;
            ++aWinCounter;
            int aPayout = globalPlayerBet * 3;
            HandlePlayerMoney(aPayout);
            std::cout << "Grease whistles low: \"" << (bothEven ? "Both even" : "Both odd") << ". You threaded it.\"\n";
            std::cout << GetWinTaunt(aWinCounter);
            std::cout << "He shoves your stack over: +" << (aPayout - globalPlayerBet) << ".\n";
        }
        else if (split1 || split2)
        {
            aWinCounter = 0;
            ++aLossStreak;
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
            aWinCounter = 0;
            ++aLossStreak;
            std::cout << "Grease shrugs: \"Hah! Not your day is it? House eats.\"\n";
            std::cout << "He pockets your bet like it owed him money: -" << globalPlayerBet << ".\n";
            globalPlayerBet = 0;
            system("pause");
            if (globalPlayerMoney <= 0)
            {
                return HandleBankruptcy();
            }
        }
        aPlayAgain = GetInput(
            0, 1,
            "\nYou sticking around this alley? (0: No, 1: Yes): ",
            "Eh-eh, it's binary, hotshot - '0' for No or '1' for Yes."
        );
        if (aPlayAgain)
        {
            DrawHUD();
            std::cout << "Grease taps the rail. \"Again.\"\n\n";
        }
        else
        {
            std::cout << "\nGrease waves you off. \"Keep your shoes clean.\"\n";
        }
    }
    return Menu;
}
