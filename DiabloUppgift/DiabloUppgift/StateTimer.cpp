#include "StateTimer.h"

StateTimer::StateTimer()
    : myStartTime(std::chrono::steady_clock::now())
{
}

void StateTimer::Reset()
{
    myStartTime = std::chrono::steady_clock::now();
}

double StateTimer::GetElapsedSeconds() const
{
    const auto now = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed = now - myStartTime;
    return elapsed.count();
}
