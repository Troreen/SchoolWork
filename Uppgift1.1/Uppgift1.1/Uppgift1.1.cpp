#include <iostream>
#include <random>


char GetYesNo()
{
    char input;
    while (true)
    {
        std::cout << "\nYou feelin' bold enough for another round? (y/n): ";
        if (!(std::cin >> input))
        {
            std::cin.clear();
        }
        std::cin.ignore(10000, '\n');
        if (input == 'y' || input == 'Y' || input == 'n' || input == 'N')
        {
            return input;
        }
        std::cout << "Eh-eh, only 'y' or 'n'. Try again... \n";
    }
}

const char* GetLossTaunt(int aStreak)
{
    if (aStreak == 1)
    {
        return "Ha! First swing and a miss. Beginner's luck didn't show up tonight.\n";
    }
    else if (aStreak == 2)
    {
        return "Ouch, two in a row. Starting to look like you're outta your league, pal...\n";
    }
    else if (aStreak == 3)
    {
        return "Three down! You're bleedin' chips if this were the real table.\n";
    }
    else if (aStreak == 4)
    {
        return "Four misses straight? Kid, the dice don't even respect you anymore.\n";
    }
    else if (aStreak >= 5 && aStreak < 8)
    {
        return "This is gettin' painful to watch. Ever thought about quittin' while you're still breathin'?\n";
    }
    else
    {
        return "Seven losses deep and you're still here? Either you're fearless... or just plain foolish.\n";
    }
}

const char* GetWinTaunt(int aWinCounter)
{
    if (aWinCounter % 3 == 1)
    {
        return "Well, look at that - you actually nailed it. Don't let it swell your head.\n";
    }
    else if (aWinCounter % 3 == 2)
    {
        return "Heh, even a busted watch is right twice a day. Nice shot, slick.\n";
    }
    else
    {
        return "Lucky break... savor it. Luck's got short legs, doesn't run far.\n";
    }
}

int GetUserGuess()
{
    int guess;
    while (true)
    {
        std::cout << "Lay your wager... what's the sum gonna be (2-12)? ";
        std::cin >> guess;
        if (guess >= 2 && guess <= 12)
        {
            break;
        }
        std::cout << "Heh, don't play me for a fool... pick a number between 2 and 12.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    return guess;
}

int RollDie(std::mt19937& aGenerator)
{
    std::uniform_int_distribution<> distribution(1, 6);
    return distribution(aGenerator);
}

int main()
{
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    char playAgain = 'y';
    system("cls");

    std::cout << "Step right up, kid...\n";
    std::cout << "====================\n";
    std::cout << "The name's Slim, and this here's my game of bones.\n";
    std::cout << "Two dice, cold and cruel. You guess the sum (2-12).\n";
    std::cout << "Hit it right, and maybe you walk away with your pride intact.\n";
    std::cout << "Miss it, and the house laughs last...\n\n";

    int lossStreak = 0;
    int winCounter = 0;

    while (playAgain == 'y' || playAgain == 'Y')
    {
        int guess = GetUserGuess();
        int die1 = RollDie(generator);
        int die2 = RollDie(generator);
        int sum = die1 + die2;

        std::cout << "\nThe dice hit the table...\n";
        std::cout << "Die 1: " << die1 << "\n";
        std::cout << "Die 2: " << die2 << "\n";
        std::cout << "Total showing: " << sum << "\n";

        if (guess == sum)
        {
            lossStreak = 0;
            ++winCounter;
            std::cout << GetWinTaunt(winCounter);
        }
        else
        {
            winCounter = 0;
            ++lossStreak;
            std::cout << GetLossTaunt(lossStreak);
        }

        playAgain = GetYesNo();


        if (playAgain == 'y' || playAgain == 'Y')
        {
            system("cls");
            std::cout << "Back for more punishment? I like your style...\n\n";
        }
    }

    std::cout << "\nThat's it, huh? Walking away while you still can.\n";
    std::cout << "Fair enough, kid. The alley's always open if you change your mind...\n";
    system("pause");
    return 0;
}




