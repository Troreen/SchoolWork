#include <iostream>

int userInputNum()
{
	int usernum;
	std::cin >> usernum;

	while (std::cin.fail()) {
		std::cin.clear(); // Clear error state
		std::cin.ignore(10009, '\n'); // Ignore bad input
		std::cout << "Invalid input! Please enter a number: ";
		std::cin >> usernum;
	}

	return usernum;

}

int main()
{

	/*
	Övnign 2
	td::cout << "Enter a character!\n";
	char userchar;
	std::cin >> userchar;
	std::cin.ignore(10000, '\n');
	std::cout << "The character you have entered is: " << userchar << "\n";
	std::cout << "Press ENTER to exit the program\n";
	getchar();
	*/


	/*
	Övning 3
	std::cout << "Enter a number: ";
	int num = userInputNum();
	std::cout << "You entered: " << num << '\n';
	*/

	/*
	Övning 4+5
	std::cout << "Enter the first number: ";
	int num1 = userInputNum();

	std::cout << "Enter the second number: ";
	int num2 = userInputNum();

	std::cout << "The result is: " << num1 << '+' << num2 << '=' << num1 + num2 << '\n';
	std::cout << "The result is: " << num1 << '-' << num2 << '=' << num1 - num2 << '\n';
	std::cout << "The result is: " << num1 << '*' << num2 << '=' << num1 * num2 << '\n';
	std::cout << "The result is: " << num1 << '/' << num2 << '=' << num1 / num2 << '\n';
	std::cout << "The result is: " << num1 << '%' << num2 << '=' << num1 % num2 << '\n';
	*/

	std::cout << "Enter the first number: ";
	int num1 = userInputNum();

	std::cout << "Enter the second number: ";
	int num2 = userInputNum();

	std::cout << "Enter the third number: ";
	int num3 = userInputNum();

	std::cout << "The result is: " << num1 << '*' << num2 << '+' << num3 << '=' << num1 * (num2 + num3) << '\n';
	
}



