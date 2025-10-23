#include "pch.h"
#include "CppUnitTest.h"
#include "../CommonUtilities/VectorOnStack.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using CommonUtilities::VectorOnStack;

namespace VectorOnStackTest
{
	TEST_CLASS(VectorOnStackTest)
	{
	public:
		TEST_METHOD(Constructor_Default_InitializesEmpty)
		{
			VectorOnStack<int, 5> vec;
			Assert::AreEqual((unsigned short)0, vec.Size());
		}

		TEST_METHOD(CopyConstructor_CopiesElements)
		{
			VectorOnStack<int, 5> vec;
			vec.Add(1);
			vec.Add(2);

			VectorOnStack<int, 5> copy(vec);
			Assert::AreEqual(vec.Size(), copy.Size());
			Assert::AreEqual(vec[0], copy[0]);
			Assert::AreEqual(vec[1], copy[1]);
		}

		TEST_METHOD(AssignmentOperator_CopiesElements)
		{
			VectorOnStack<int, 5> src;
			src.Add(3);
			src.Add(4);

			VectorOnStack<int, 5> dst;
			dst.Add(99);
			dst = src;
			Assert::AreEqual(src.Size(), dst.Size());
			Assert::AreEqual(src[0], dst[0]);
			Assert::AreEqual(src[1], dst[1]);
		}

		TEST_METHOD(AssignmentOperator_SelfAssignment_NoChange)
		{
			VectorOnStack<int, 5> vec;
			vec.Add(10);
			vec.Add(20);
			auto before0 = vec[0];
			auto before1 = vec[1];
			vec = vec; // self-assign
			Assert::AreEqual((unsigned short)2, vec.Size());
			Assert::AreEqual(before0, vec[0]);
			Assert::AreEqual(before1, vec[1]);
		}

		TEST_METHOD(Add_AddsElements_UntilCapacity)
		{
			VectorOnStack<int, 3> vec;
			vec.Add(10);
			vec.Add(20);
			vec.Add(30);
			Assert::AreEqual((unsigned short)3, vec.Size());
			Assert::AreEqual(10, vec[0]);
			Assert::AreEqual(20, vec[1]);
			Assert::AreEqual(30, vec[2]);
		}

		TEST_METHOD(Insert_InsertsAtBeginning)
		{
			VectorOnStack<int, 4> vec;
			vec.Add(2);
			vec.Add(3);
			vec.Insert(0, 1);
			Assert::AreEqual((unsigned short)3, vec.Size());
			Assert::AreEqual(1, vec[0]);
			Assert::AreEqual(2, vec[1]);
			Assert::AreEqual(3, vec[2]);
		}

		TEST_METHOD(Insert_InsertsAtEnd)
		{
			VectorOnStack<int, 4> vec;
			vec.Add(1);
			vec.Add(2);
			vec.Insert(vec.Size(), 3); // insert at end
			Assert::AreEqual((unsigned short)3, vec.Size());
			Assert::AreEqual(1, vec[0]);
			Assert::AreEqual(2, vec[1]);
			Assert::AreEqual(3, vec[2]);
		}

		TEST_METHOD(RemoveCyclic_RemovesFirstMatch)
		{
			VectorOnStack<int, 4> vec;
			vec.Add(5);
			vec.Add(6);
			vec.Add(7);
			vec.RemoveCyclic(6);
			Assert::AreEqual((unsigned short)2, vec.Size());
			Assert::AreEqual(5, vec[0]);
			Assert::AreEqual(7, vec[1]);
		}

		TEST_METHOD(RemoveCyclic_NoMatch_NoChange)
		{
			VectorOnStack<int, 4> vec;
			vec.Add(1);
			vec.Add(2);
			vec.RemoveCyclic(99); // not present
			Assert::AreEqual((unsigned short)2, vec.Size());
			Assert::AreEqual(1, vec[0]);
			Assert::AreEqual(2, vec[1]);
		}

		TEST_METHOD(RemoveCyclicAtIndex_RemovesAndReplaces)
		{
			VectorOnStack<int, 3> vec;
			vec.Add(8);
			vec.Add(9);
			vec.Add(10);
			vec.RemoveCyclicAtIndex(1);
			Assert::AreEqual((unsigned short)2, vec.Size());
			Assert::AreEqual(8, vec[0]);
			Assert::AreEqual(10, vec[1]);
		}

		TEST_METHOD(RemoveCyclicAtIndex_LastIndex_RemovesLast)
		{
			VectorOnStack<int, 3> vec;
			vec.Add(1);
			vec.Add(2);
			vec.Add(3);
			vec.RemoveCyclicAtIndex((unsigned short)2);
			Assert::AreEqual((unsigned short)2, vec.Size());
			// Remaining two elements should be 1 and 2 in some order; index 0 must be 1, index 1 must be 2
			Assert::AreEqual(1, vec[0]);
			Assert::AreEqual(2, vec[1]);
		}

		TEST_METHOD(Clear_ResetsSize)
		{
			VectorOnStack<int, 2> vec;
			vec.Add(11);
			vec.Clear();
			Assert::AreEqual((unsigned short)0, vec.Size());
		}

		TEST_METHOD(Size_ReturnsCurrentSize)
		{
			VectorOnStack<int, 2> vec;
			Assert::AreEqual((unsigned short)0, vec.Size());
			vec.Add(12);
			Assert::AreEqual((unsigned short)1, vec.Size());
			vec.Add(13);
			Assert::AreEqual((unsigned short)2, vec.Size());
		}

		TEST_METHOD(IndexOperator_ReadWrite)
		{
			VectorOnStack<int, 2> vec;
			vec.Add(0);
			vec[0] = 42; // non-const operator[] returns reference
			Assert::AreEqual(42, vec[0]);
		}

		TEST_METHOD(ConstIndexOperator_ReadOnly)
		{
			VectorOnStack<int, 2> vec;
			vec.Add(15);
			vec.Add(16);
			const VectorOnStack<int, 2>& cvec = vec;
			Assert::AreEqual(15, cvec[0]);
			Assert::AreEqual(16, cvec[1]);
		}
	};
}
