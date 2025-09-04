#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <map>
#include <utility>
#include <iomanip>
#include <cmath>
#include <cctype>


void ForNumsOneToTen() 
{
	std::cout << "Printing numbers 1-10: " << '\n';

	for (int i = 1; i <11 ; i++)
	{
		std::cout << i << '\n';
	}
	std::cout << '\n';
}

void ForNumsTenToOne()
{
	std::cout << "Printing numbers 10-1: " << '\n';

	for (int i = 10; i > 0; i--)
	{
		std::cout << i << '\n';
	}
	std::cout << '\n';
}

void ForEveryOtherNumOneToTen()
{
	std::cout << "Printing every other number 1-10: " << '\n';

	for (int i = 1; i < 11; i++)
	{
		if (i % 2 == 0)
		{
			std::cout << i << '\n';
		}
	}
	std::cout << '\n';
}

void ForEveryOtherNumTenToOne()
{
	std::cout << "Printing every other number 10-1: " << '\n';

	for (int i = 10; i > 0; i--)
	{
		if (i % 2 == 0)
		{
			std::cout << i << '\n';
		}
	}
	std::cout << '\n';
}

void ForOneToTenMil()
{
	std::cout << "Printing every number 1-10 000 000 incrementing exponentially: " << '\n';

	for (int i = 1; i < 10000000;)
	{
		std::cout << i << '\n';
		i = i + i;
	}
	std::cout << '\n';
}

void WhileOneToTen()
{
	std::cout << "Printing every number 1-10: " << '\n';
	
	int counter = 1;
	while (counter <= 10)
	{
		std::cout << counter << '\n';
		counter++;
	}
	std::cout << '\n';

}

void WhileTenToOne()
{
	std::cout << "Printing every number 10-1: " << '\n';

	int counter = 10;
	while (counter > 0)
	{
		std::cout << counter << '\n';
		counter--;
	}
	std::cout << '\n';
}

void WhileOneToTenMil()
{ 
	std::cout << "Printing every number 1-10 000 000 incrementing exponentially: " << '\n';

	int counter = 1;
	while (counter <= 10000000)
	{
		std::cout << counter << '\n';
		counter = counter + counter;
	}
	std::cout << '\n';
}

void PasswordThroughTxt()
{
	const std::string passwordFile = "C:\\Users\\tarik.bergstrom\\Documents\\password.txt";
	std::string storedPassword;
	std::string inputPassword;

	std::ifstream inFile(passwordFile);
	if (!inFile)
	{
		std::cerr << "Error: Could not open password file.\n";
		return;
	}
	std::getline(inFile, storedPassword);
	inFile.close();

	const int maxAttempts = 3;
	for (int attempt = 1; attempt <= maxAttempts; ++attempt)
	{
		std::cout << "Enter Password: ";
		std::getline(std::cin, inputPassword);

		if (inputPassword == storedPassword)
		{
			std::cout << "Access granted.\n";
			return;
		}
		else if (attempt < maxAttempts)
		{
			std::cout << "Wrong password, please try again.\n";
		}
	}
	std::cout << "You have entered a wrong password too many times, access denied.\n";
}

void StarRectangle()
{

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			std::cout << '*';
		}
		std::cout << '\n';
	}
}

void StarFrame()
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (i == 0 || i == 4)
			{
				std::cout << '*';
			}
			else if (j == 0 || j == 19)
			{
				std::cout << '*';
			}
			else
			{
				std::cout << ' ';

			}
		}
		std::cout << '\n';
	}
}

void StarRectangleInput()
{
	int width;
	int height;
	std::cout << "Enter the width of the rectangle: ";
	std::cin >> width;

	std::cout << "Enter the length of the rectangle: ";
	std::cin >> height;
	
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			std::cout << '*';
		}
		std::cout << '\n';
	}
}

void StarFrameInput()
{
	int width;
	int height;
	std::cout << "Enter the width of the frame: ";
	std::cin >> width;

	std::cout << "Enter the length of the frame: ";
	std::cin >> height;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i == 0 || i == height-1)
			{
				std::cout << '*';
			}
			else if (j == 0 || j == width-1)
			{
				std::cout << '*';
			}
			else
			{
				std::cout << ' ';

			}
		}
		std::cout << '\n';
	}
}

int RollDie(std::mt19937& aGenerator)
{
	std::uniform_int_distribution<> distr(1, 6);
	return distr(aGenerator);
}

void DiceRollUntilSnakeEyes(std::mt19937& aGenerator)
{
	int totalRolls = 0;
	int die1 = 0;
	int die2 = 0;

	while (true)
	{
		if (!(die1 == 1 && die2 == 1))
		{
			std::cout << "\n\nRolling the dice!..\n";
			die1 = RollDie(aGenerator);
			die2 = RollDie(aGenerator);
			std::cout << "Die 1: " << die1 << "\n";
			std::cout << "Die 2: " << die2 << "\n";
			totalRolls++;
		}
		else
		{
			break;
		}
	}
	std::cout << "\n\nThe dice show snake eyes, total amount of rolls: " << totalRolls;		
}

void DiceRollTenK(std::mt19937& aGenerator)
{
    std::map<int, int> rollCounts;

    for (int i = 0; i < 10000; ++i)
    {
        int die1 = RollDie(aGenerator);
        int die2 = RollDie(aGenerator);
		int sum = die1 + die2;
        rollCounts[sum]++;
    }

	int totalRolls = 0;
	int maxCount = 0;
	for (const auto& entry : rollCounts)
	{
		totalRolls += entry.second;
		if (entry.second > maxCount)
		{
			maxCount = entry.second;
		}
	}

	const int MAX_WIDTH = 50;

	std::cout << "Dice sum distribution (" << totalRolls << " rolls)\n";
	std::cout << "Each bar is normalised to max width " << MAX_WIDTH << ".\n\n";

	for (int s = 2; s <= 12; ++s)
	{
		if (rollCounts.find(s) == rollCounts.end())
		{
			rollCounts[s] = 0;
		}
	}

	for (const auto& entry : rollCounts)
	{
		int sum = entry.first;
		int count = entry.second;

		double percentage = (totalRolls > 0) ? (100.0 * static_cast<double>(count) / static_cast<double>(totalRolls)) : 0.0;

		int stars = 0;
		if (maxCount > 0)
		{
			double scaled = static_cast<double>(count) / static_cast<double>(maxCount) * MAX_WIDTH;
			stars = static_cast<int>(std::round(scaled));
			if (count > 0 && stars == 0)
			{
				stars = 1;
			}
			if (stars > MAX_WIDTH) 
			{
				stars = MAX_WIDTH;
			}
		}

		std::string bar(stars, '*');

		std::cout << "Sum " << std::setw(2) << sum << " | " << bar << "  (" << percentage << "%)" << "  [" << count << "]\n";
	}

}

void PrintString(std::string aString)
{
	std::cout << aString;
}

void PrintSum(float aNum1, float aNum2)
{
	std::cout << aNum1 + aNum2;
}

int Add(int aNum1, int aNum2)
{
	return aNum1 + aNum2;
}

int GetInput(int aMinNum, int aMaxNum, std::string aPrompt)
{
	std::cout << aPrompt << '(' << aMinNum << '-' << aMaxNum << "): ";

	int value;
	if (!(std::cin >> value))
	{
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		return aMaxNum + 1;
	}

	if (value < aMinNum || value > aMaxNum)
	{
		return aMaxNum + 1;
	}
	return value;


}

void PrintDigit(int aNum)
{
	int number;
	if (!(std::cin >> number))
	{
		std::cout << "Error!";
	}
	switch (number)
	{
	default:
		break;
	}
}

float Min(float a, float b) {
	if (a < b) {
		return a;
	}
	else {
		return b;
	}
}

float Max(float a, float b) {

	return (a > b) ? a : b;
}

float PowerOf(float base, int exponent) {
	if (exponent == 0) {
		return 1.0f;
	}

	float result = 1.0f;
	for (int i = 0; i < exponent; ++i) {
		result *= base;
	}
	return result;
}

void PrintReverse(const std::string& str) {
	for (int i = str.length() - 1; i >= 0; --i) {
		std::cout << str[i];
	}
	std::cout << std::endl;
}

std::string ToLowerCase(const std::string& str) {
	std::string lowerStr = "";
	for (char c : str) {
		lowerStr += tolower(c);
	}
	return lowerStr;
}

int main()
{

	///For loops
	//ForNumsOneToTen();
	//ForNumsTenToOne();
	//ForEveryOtherNumOneToTen();
	//ForEveryOtherNumTenToOne();
	//ForOneToTenMil();

	///While loops
	//WhileOneToTen();
	//WhileTenToOne();
	//WhileOneToTenMil();
	
	///Password
	//PasswordThroughTxt();

	///Graphic
	//StarRectangle();
	//StarFrame();
	//StarRectangleInput();
	//StarFrameInput();

	///Dice
	//std::random_device randomDevice;
	//std::mt19937 generator(randomDevice());
	//DiceRollUntilSnakeEyes(generator);
	//DiceRollTenK(generator);

	///More functions
	//PrintString("Halloj");
	//PrintSum(3.5, 4.5);
	//std::cout << Add(3, 5);
	//int number = GetInput(1, 10, "Enter a number");
	//std::cout << "You entered " << number << '\n';

	///If and switch
	// skipping this


	///Maths
	//float num1 = 15.5f;
	//float num2 = 7.8f;

	//std::cout << "The minimum of " << num1 << " and " << num2 << " is: " << Min(num1, num2) << std::endl;
	//std::cout << "The maximum of " << num1 << " and " << num2 << " is: " << Max(num1, num2) << std::endl;

	//float base = 2.5f;
	//int exponent = 3;

	//std::cout << base << " raised to the power of " << exponent << " is: " << PowerOf(base, exponent) << std::endl;

	///Strings
	//PrintReverse("Hello world!");

	//std::string originalString = "This Is A TEST String.";
	//std::string processedString = ToLowerCase(originalString);

	//std::cout << "Original string: " << originalString << std::endl;
	//std::cout << "Lowercase string: " << processedString << std::endl;



}

