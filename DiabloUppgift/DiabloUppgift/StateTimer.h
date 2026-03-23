#pragma once

#include <chrono>

class StateTimer
{
public:
    StateTimer();

    void Reset();
    double GetElapsedSeconds() const;

private:
    std::chrono::steady_clock::time_point myStartTime;
};
