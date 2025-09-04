#include <iostream>
#include <random>
#include <string>

void ShowMenu();
void ShowInstructions();
void PlayGame(std::mt19937& generator);
void PrintGuesses(int guessCount);

int RNG(std::mt19937& aGenerator) {
    std::uniform_int_distribution<> distr(1, 100);
    return distr(aGenerator);
}

int GetInput(int aMinNum, int aMaxNum, std::string aPrompt) {
    std::cout << aPrompt << " (" << aMinNum << "-" << aMaxNum << "): ";
    int value;
    if (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return 0;
    }
    if (value < aMinNum || value > aMaxNum) {
        return 0;
    }
    return value;
}

void PrintString(std::string aString) {
    std::cout << aString << std::endl;
}

void GuessHigh() {
    PrintString("Your guess was too high, try again!");
}

void GuessLow() {
    PrintString("Your guess was too low, try again!");
}

void GuessRight() {
    PrintString("You guessed right!");
}

int main() {
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());

    while (true) {
        ShowMenu();
        int choice = GetInput(1, 3, "Enter your choice");
        if (choice == 1) {
            PlayGame(generator);
        } else if (choice == 2) {
            ShowInstructions();
        } else if (choice == 3) {
            PrintString("Exiting game. Goodbye!");
            return 0;
        } else {
            PrintString("Invalid choice, please try again.");
        }
    }
}

void ShowMenu() {
    PrintString("\n--- Guessing Game ---");
    PrintString("1. Play Game");
    PrintString("2. Instructions");
    PrintString("3. Exit");
    PrintString("---------------------\n");
}

void ShowInstructions() {
    PrintString("\n--- Instructions ---");
    PrintString("Guess a number between 1 and 100.");
    PrintString("The game will tell you if your guess is too high or too low.");
    PrintString("Keep guessing until you find the right number!");
    PrintString("--------------------\n");
}

void PlayGame(std::mt19937& generator) {
    int numberOfRounds = GetInput(1, 10, "How many rounds would you like to play?");
    if (numberOfRounds == 0) {
        PrintString("Invalid input, defaulting to 1 round.");
        numberOfRounds = 1;
    }

    int totalGuesses = 0;
    for (int i = 0; i < numberOfRounds; ++i) {
        std::cout << "\n--- Starting round " << i + 1 << " of " << numberOfRounds << " ---\n";
        int goal = RNG(generator);
        int guessCount = 0;
        while (true) {
            int input = GetInput(1, 100, "Make a guess");
            if (input == 0) {
                PrintString("Invalid input, please enter a number between 1 and 100.");
                continue;
            }
            guessCount++;
            if (input == goal) {
                GuessRight();
                PrintGuesses(guessCount);
                totalGuesses += guessCount;
                break; 
            }
            else if (input < goal) {
                GuessLow();
            }
            else {
                GuessHigh();
            }
        }
    }
    std::cout << "\n--- Game Over ---" << std::endl;
    std::cout << "You played " << numberOfRounds << " rounds with a total of " << totalGuesses << " guesses." << std::endl;
    std::cout << "-----------------" << std::endl;
}

void PrintGuesses(int guessCount) {
    std::cout << "It took you " << guessCount << " guesses to find the number!" << std::endl;
}