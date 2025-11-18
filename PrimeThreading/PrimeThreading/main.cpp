#include <iostream>
#include <conio.h>
#include <limits.h>
#include <thread>
#include <mutex>
#include <atomic>

std::mutex g_printMutex;
std::atomic<bool> g_shouldStop(false);

bool IsPrime(int aNumber)
{
	if (aNumber <= 1) return false;
	if (aNumber == 2) return true;
	if (aNumber % 2 == 0) return false;
	for (int i = 2; i <= aNumber / i; i++)
	{
		if (aNumber % i == 0)
			return false;
	}
	return true;
}

void PrintPrime(int n)
{
	std::lock_guard<std::mutex> lock(g_printMutex);
	std::cout << n << " is prime." << std::endl;

}

void PrimeWorker(int aStart, int aStep)
{
	for (int i = aStart; i < INT_MAX && !g_shouldStop.load(); i += aStep)
	{
		if (IsPrime(i))
		{
			PrintPrime(i);
		}
	}
}

int main() 
{	
	std::thread t1(PrimeWorker, 0, 4);
	std::thread t2(PrimeWorker, 1, 4);
	std::thread t3(PrimeWorker, 2, 4);
	std::thread t4(PrimeWorker, 3, 4);
	
	while (!g_shouldStop.load())
	{

		if (_kbhit())
		{
			int ch = _getch();
			if (ch == '\r')
			{
				g_shouldStop.store(true);
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	}

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	system("pause");

    return 0;
}
