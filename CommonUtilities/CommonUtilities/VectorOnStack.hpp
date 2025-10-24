#pragma once
#include <cassert>

namespace CommonUtilities
{
	template <typename Type, int SIZE, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	class VectorOnStack
	{
	public:
		VectorOnStack();
		VectorOnStack(const VectorOnStack& aVectorOnStack);
		~VectorOnStack();

		VectorOnStack& operator= (const VectorOnStack& aVectorOnStack);

		inline Type& operator[] (int anIndex);
		inline const Type& operator[] (int anIndex) const;

		inline void Add(const Type& anObject);
		inline void Insert(int anIndex, const Type& anObject);
		inline void RemoveCyclic(const Type& anObject);
		inline void RemoveCyclicAtIndex(int anIndex);

		inline void Clear();
		__forceinline CountType Size() const;

	private:
		Type myData[SIZE];
		CountType mySize;
	};

	template <typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::VectorOnStack()
		: mySize(0)
	{
	}

	template <typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::~VectorOnStack()
	{
		mySize = 0;
	}

	template <typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::VectorOnStack(const VectorOnStack& aVectorOnStack)
		: mySize(aVectorOnStack.mySize)
	{
		for (CountType i = 0; i < mySize; ++i)
		{
			myData[i] = aVectorOnStack.myData[i];
		}
	}

	template <typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>& VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::operator= (const VectorOnStack& aVectorOnStack)
	{
		if (this != &aVectorOnStack)
		{
			mySize = aVectorOnStack.mySize;
			for (CountType i = 0; i < mySize; ++i)
			{
				myData[i] = aVectorOnStack.myData[i];
			}
		}
		return *this;
	}

	template <typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	Type& VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::operator[] (int anIndex)
	{
		assert(anIndex >= 0);
		assert(static_cast<CountType>(anIndex) < mySize);
		return myData[static_cast<CountType>(anIndex)];
	}

	template <typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	const Type& VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::operator[] (int anIndex) const
	{
		assert(anIndex >= 0);
		assert(static_cast<CountType>(anIndex) < mySize);
		return myData[static_cast<CountType>(anIndex)];
	}

	template <typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::Add(const Type& anObject)
	{
		assert(mySize < SIZE);
		myData[mySize] = anObject;
		++mySize;
	}

	template <typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::Insert(int anIndex, const Type& anObject)
	{
		assert(mySize < SIZE);
		assert(anIndex >= 0);
		assert(static_cast<CountType>(anIndex) <= mySize);

		const CountType idx = static_cast<CountType>(anIndex);
		for (CountType i = mySize; i > idx; --i)
		{
			myData[i] = myData[i - 1];
		}
		myData[idx] = anObject;
		++mySize;
	}

	template <typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::RemoveCyclic(const Type& anObject)
	{
		for (CountType i = 0; i < mySize; ++i)
		{
			if (myData[i] == anObject)
			{
				RemoveCyclicAtIndex(static_cast<int>(i));
				return;
			}
		}
	}

	template <typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::RemoveCyclicAtIndex(int anIndex)
	{
		assert(anIndex >= 0);
		assert(static_cast<CountType>(anIndex) < mySize);
		const CountType idx = static_cast<CountType>(anIndex);
		myData[idx] = myData[mySize - 1];
		--mySize;
	}

	template <typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::Clear()
	{
		mySize = 0;
	}

	template <typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	CountType VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::Size() const
	{
		return mySize;
	}
} // namespace CommonUtilities