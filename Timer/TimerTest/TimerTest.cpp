#include "pch.h"
#include "CppUnitTest.h"

#include <chrono>
#include <thread>
#include <cmath>

#include "..\\Timer\\Timer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using CommonUtilities::Timer;

namespace
{
    inline bool approx(double a, double b, double tol)
    {
        return std::fabs(a - b) <= tol;
    }
}

namespace TimerTest
{
    TEST_CLASS(TimerTest)
    {
    public:

        TEST_METHOD(InitialState)
        {
            Timer t;
            // Delta should be exactly 0.0f on creation
            Assert::IsTrue(t.GetDeltaTime() == 0.0f, L"Delta time should be 0 on creation");
            // Total time should be ~0
            Assert::IsTrue(approx(t.GetTotalTime(), 0.0, 1e-3), L"Total time should be ~0 on creation");
        }

        TEST_METHOD(DeltaAfterSleep)
        {
            Timer t;
            constexpr int sleepMs = 50;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
            t.Update();
            const float dt = t.GetDeltaTime();
            // Allow generous tolerance because of scheduling and CI variability
            Assert::IsTrue(approx(dt, sleepMs / 1000.0, 0.025), L"Delta after ~50ms should be close to 0.05s");
        }

        TEST_METHOD(TotalAfterTwoSleeps)
        {
            Timer t;
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
            t.Update();
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
            t.Update();
            const double total = t.GetTotalTime();
            // Expect ~0.07s with generous tolerance
            Assert::IsTrue(approx(total, 0.07, 0.035), L"Total after two frames should be close to sum of sleeps");
        }

        TEST_METHOD(DeltaWithoutSleepIsSmall)
        {
            Timer t;
            t.Update();
            t.Update();
            const float dt = t.GetDeltaTime();
            Assert::IsTrue(dt >= 0.0f, L"Delta must be non-negative");
            Assert::IsTrue(dt < 0.5f, L"Delta should be reasonably small when no sleep between updates");
        }

        TEST_METHOD(TotalTimeMonotonic)
        {
            Timer t;
            t.Update();
            const double total1 = t.GetTotalTime();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            t.Update();
            const double total2 = t.GetTotalTime();
            Assert::IsTrue(total2 > total1, L"Total time should increase over time");
        }
    };
}
