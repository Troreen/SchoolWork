#include <Timer.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <Windows.h>

int main() {
	CommonUtilities::Timer timer; 

	timer.Update();

	while (true)
	{
		Sleep(100);

		timer.Update();
		float deltaTime = timer.GetDeltaTime();
		double totalTime = timer.GetTotalTime();

		std::cout << "Delta Time: " << deltaTime << " s, Total Time: " << totalTime << " s" << std::endl;
	}
}
