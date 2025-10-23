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

		inline Type& operator[] (const CountType anIndex);
		inline const Type& operator[] (const CountType anIndex) const;

		inline void Add(const Type& anObject);
		inline void Insert(const CountType anIndex, const Type& anObject);
		inline void RemoveCyclic(const Type& anObject);
		inline void RemoveCyclicAtIndex(const CountType anIndex);

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
	Type& VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::operator[] (const CountType anIndex) 
	{
		assert(anIndex < mySize);
		return myData[anIndex];
	}

	template <typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	const Type& VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::operator[] (const CountType anIndex) const 
	{
		assert(anIndex < mySize);
		return myData[anIndex];
	}

	template <typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::Add(const Type& anObject)
	{
		assert(mySize < SIZE);
		myData[mySize] = anObject;
		++mySize;
	}


	template <typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::Insert(const CountType anIndex, const Type& anObject)
	{
		assert(mySize < SIZE);
		assert(anIndex <= mySize);
		for (CountType i = mySize; i > anIndex; --i)
		{
			myData[i] = myData[i - 1];
		}
		myData[anIndex] = anObject;
		++mySize;
	}

	template <typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::RemoveCyclic(const Type& anObject)
	{
		for (CountType i = 0; i < mySize; ++i)
		{
			if (myData[i] == anObject)
			{
				RemoveCyclicAtIndex(i);
				return;
			}
		}
	}

	template <typename Type, int SIZE, typename CountType, bool UseSafeModeFlag>
	void VectorOnStack<Type, SIZE, CountType, UseSafeModeFlag>::RemoveCyclicAtIndex(const CountType anIndex)
	{
		assert(anIndex < mySize);
		myData[anIndex] = myData[mySize - 1];
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