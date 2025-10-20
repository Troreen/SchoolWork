#include <cassert>

namespace CommonUtilities
{
	template <typename T>
	T Min (const T aFirstValue, const T aSecondValue)
	{
		return (aFirstValue < aSecondValue) ? aFirstValue : aSecondValue;
	}

	template <typename T>
	T Max (const T aFirstValue, const T aSecondValue)
	{
		return (aFirstValue > aSecondValue) ? aFirstValue : aSecondValue;
	}

	template <typename T>
	T Abs (const T aValue)
	{
		return (aValue < static_cast<T>(0)) ? -aValue : aValue;
	}

	template <typename T>
	T Clamp (const T& aValue, const T& aMinValue, const T& aMaxValue)
	{
		assert(aMinValue <= aMaxValue && "Clamp: aMinValue must be less than or equal to aMaxValue");
		return Max(aMinValue, Min(aValue, aMaxValue));
	}

	template <typename T>
	T Lerp (const T aStartValue, const T aEndValue, const float aLerpFactor)
	{
		return static_cast<T>(aStartValue + aLerpFactor * (aEndValue - aStartValue));
	}

	template <typename T>
	void Swap (T& aFirstValue, T& aSecondValue)
	{
		T temp = aFirstValue;
		aFirstValue = aSecondValue;
		aSecondValue = temp;
	}
}