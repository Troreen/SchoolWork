#include "pch.h"
#include "CppUnitTest.h"
#include "UtilityFunctions.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CommonUtilities;

namespace UtilityTest
{
    TEST_CLASS(MinFunctionTests)
    {
    public:
        TEST_METHOD(MinFirstParamSmallerReturnsFirst)
        {
            Assert::AreEqual(3, Min(3, 5), L"Expected 3");
        }

        TEST_METHOD(MinSecondParamSmallerReturnsSecond)
        {
            Assert::AreEqual(3, Min(5, 3), L"Expected 3");
        }

        TEST_METHOD(MinFirstNegativeReturnsSmaller)
        {
            Assert::AreEqual(-3, Min(-3, 5), L"Expected -3");
        }

        TEST_METHOD(MinSecondNegativeReturnsSmaller)
        {
            Assert::AreEqual(-3, Min(5, -3), L"Expected -3");
        }

        TEST_METHOD(MinBothNegativeFirstSmaller)
        {
            Assert::AreEqual(-3, Min(-3, -1), L"Expected -3");
        }

        TEST_METHOD(MinBothNegativeSecondSmaller)
        {
            Assert::AreEqual(-3, Min(-1, -3), L"Expected -3");
        }
    };

    TEST_CLASS(MaxFunctionTests)
    {
    public:
        TEST_METHOD(MaxFirstParamLargerReturnsFirst)
        {
            Assert::AreEqual(5, Max(5, 3), L"Expected 5");
        }

        TEST_METHOD(MaxSecondParamLargerReturnSecond)
        {
            Assert::AreEqual(5, Max(3, 5), L"Expected 5");
        }

        TEST_METHOD(MaxFirstNegativeReturnsLarger)
        {
            Assert::AreEqual(5, Max(-3, 5), L"Expected 5");
        }

        TEST_METHOD(MaxSecondNegativeReturnsLarger)
        {
            Assert::AreEqual(5, Max(5, -3), L"Expected 5");
        }

        TEST_METHOD(MaxBothNegativeFirstLarger)
        {
            Assert::AreEqual(-1, Max(-1, -3), L"Expected -1");
        }

        TEST_METHOD(MaxBothNegativeSecondLarger)
        {
            Assert::AreEqual(-1, Max(-3, -1), L"Expected -1");
        }
    };

    TEST_CLASS(ClampFunctionTests)
    {
        TEST_METHOD(ClampValueWithinRangeReturnsSameValue)
        {
            Assert::AreEqual(5, Clamp(5, 3, 7), L"Expected 5");
        }

        TEST_METHOD(ClampValueEqualsMinReturnsMin)
        {
			Assert::AreEqual(3, Clamp(3, 3, 7), L"Expected 3");
        }

        TEST_METHOD(ClampValueEqualsMaxReturnsMax)
        {
            Assert::AreEqual(7, Clamp(7, 3, 7), L"Expected 7");
        }
        
        TEST_METHOD(ClampValueBelowMinReturnsMin)
        {
            Assert::AreEqual(3, Clamp(2, 3, 7), L"Expected 3");
		}

        TEST_METHOD(ClampValueAboveMaxReturnsMax)
        {
            Assert::AreEqual(7, Clamp(8, 3, 7), L"Expected 7");
		}

        TEST_METHOD(ClampNegativeValues)
        {
            Assert::AreEqual(-5, Clamp(-7, -5, -3), L"Expected -5");
		}

        TEST_METHOD(ClampMixedValues)
        {
			Assert::AreEqual(0, Clamp(-2, 0, 5), L"Expected 0");
        }

        TEST_METHOD(ClampMinEqualsMax)
        {
			Assert::AreEqual(5, Clamp(3, 5, 5), L"Expected 5");
        }

        TEST_METHOD(ClampShouldAssertMinGreaterThanMax)
        {
            //Clamp(5, 7, 3);  // Uncomment to verify assert triggers
            Assert::IsTrue(true, L"Manual check: Clamp(5,7,3) should assert");
        }
    };

    TEST_CLASS(AbsFunctionTests)
    {
        TEST_METHOD(AbsPositiveValueReturnsSameValue)
        {
            Assert::AreEqual(5, Abs(5), L"Expected 5");
		}

        TEST_METHOD(AbsNegativeValueReturnsPositive)
        {
            Assert::AreEqual(5, Abs(-5), L"Expected 5");
        }

        TEST_METHOD(AbsZeroReturnsZero)
        {
            Assert::AreEqual(0, Abs(0), L"Expected 0");
        }


    };

    TEST_CLASS(SwapFunctionTests)
    {
        TEST_METHOD(SwapPositiveIntegersValuesAreSwapped)
        {
            int a = 3;
            int b = 5;
            Swap(a, b);
            Assert::AreEqual(5, a, L"Expected a to be 5");
            Assert::AreEqual(3, b, L"Expected b to be 3");
		}

        TEST_METHOD(SwapFloatPositiveValuesAreSwapped)
        {
            float a = 3.0f;
            float b = 5.0f;
            Swap(a, b);
            Assert::AreEqual(5.0f, a, L"Expected a to be 5.0");
            Assert::AreEqual(3.0f, b, L"Expected b to be 3.0");
        }

        TEST_METHOD(SwapNegativeIntegersValuesAreSwapped)
        {
            int a = -3;
            int b = -5;
            Swap(a, b);
            Assert::AreEqual(-5, a, L"Expected a to be -5");
			Assert::AreEqual(-3, b, L"Expected b to be -3");
        }
        
        TEST_METHOD(SwapFloatNegativeValuesAreSwapped)
        {
            float a = -3.0f;
            float b = -5.0f;
            Swap(a, b);
            Assert::AreEqual(-5.0f, a, L"Expected a to be -5.0");
            Assert::AreEqual(-3.0f, b, L"Expected b to be -3.0");
        }

        TEST_METHOD(SwapMixedSignIntegersValuesAreSwapped)
        {
            int a = -3;
            int b = 5;
            Swap(a, b);
            Assert::AreEqual(5, a, L"Expected a to be 5");
            Assert::AreEqual(-3, b, L"Expected b to be -3");
        }

        TEST_METHOD(SwapMixedSignFloatsValuesAreSwapped)
        {
            float a = -3.0f;
            float b = 5.0f;
            Swap(a, b);
            Assert::AreEqual(5.0f, a, L"Expected a to be 5.0");
            Assert::AreEqual(-3.0f, b, L"Expected b to be -3.0");
		}

        TEST_METHOD(SwapSameValuesRemainUnchanged)
        {
            int a = 3;
            int b = 3;
            Swap(a, b);
            Assert::AreEqual(3, a, L"Expected a to be 3");
            Assert::AreEqual(3, b, L"Expected b to be 3");
        }

        TEST_METHOD(SwapStringValuesAreSwapped)
        {
            std::string a = "Hello";
            std::string b = "World";
            Swap(a, b);
            Assert::AreEqual(std::string("World"), a, L"Expected a to be 'World'");
            Assert::AreEqual(std::string("Hello"), b, L"Expected b to be 'Hello'");
		}

    };

    TEST_CLASS(LerpFunctionTests)
    {
        TEST_METHOD(LerpStartToEndFactorZeroReturnsStart)
        {
            Assert::AreEqual(3.0f, Lerp(3.0f, 7.0f, 0.0f), L"Expected 3.0f");
		}

        TEST_METHOD(LerpStartToEndFactorOneReturnsEnd)
        {
            Assert::AreEqual(7.0f, Lerp(3.0f, 7.0f, 1.0f), L"Expected 7.0f");
        }

        TEST_METHOD(LerpStartToEndFactorHalfReturnsMidpoint)
        {
            Assert::AreEqual(5.0f, Lerp(3.0f, 7.0f, 0.5f), L"Expected 5.0f");
		}
    };

}
