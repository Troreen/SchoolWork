#include "pch.h"
#include "CppUnitTest.h"

#include <string>

#include "../CommonUtilLib/Queue.hpp"
#include "../CommonUtilLib/Stack.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathTests
{
    TEST_CLASS(StackQueueTests)
    {
    public:
        TEST_METHOD(Stack_DefaultState_IsEmpty)
        {
            Stack<int> stack;

            Assert::AreEqual(0, stack.GetSize());
            Assert::IsTrue(stack.IsEmpty());
            Assert::AreEqual(0, stack.GetCapacity());
        }

        TEST_METHOD(Stack_PushAndPop_FollowsLifo)
        {
            Stack<int> stack;
            stack.Push(10);
            stack.Push(20);
            stack.Push(30);

            Assert::AreEqual(3, stack.GetSize());
            Assert::AreEqual(30, stack.GetTop());

            stack.Pop();
            Assert::AreEqual(2, stack.GetSize());
            Assert::AreEqual(20, stack.GetTop());

            stack.Pop();
            stack.Pop();
            Assert::IsTrue(stack.IsEmpty());
        }

        TEST_METHOD(Stack_Copy_IsIndependent)
        {
            Stack<std::string> original;
            original.Push("first");
            original.Push("second");

            Stack<std::string> copy = original;
            copy.Pop();
            copy.Push("changed");

            Assert::AreEqual(std::string("second"), original.GetTop());
            Assert::AreEqual(std::string("changed"), copy.GetTop());
        }

        TEST_METHOD(Stack_ReserveAndClear_Work)
        {
            Stack<int> stack;
            stack.Reserve(16);

            Assert::IsTrue(stack.GetCapacity() >= 16);

            stack.Push(1);
            stack.Push(2);
            stack.Push(3);
            stack.Clear();

            Assert::AreEqual(0, stack.GetSize());
            Assert::IsTrue(stack.IsEmpty());
            Assert::IsTrue(stack.GetCapacity() >= 16);
        }

        TEST_METHOD(Queue_DefaultState_IsEmpty)
        {
            Queue<int> queue;

            Assert::AreEqual(0, queue.GetSize());
            Assert::IsTrue(queue.IsEmpty());
            Assert::AreEqual(0, queue.GetCapacity());
        }

        TEST_METHOD(Queue_EnqueueDequeue_FollowsFifo)
        {
            Queue<int> queue;
            queue.Enqueue(10);
            queue.Enqueue(20);
            queue.Enqueue(30);

            Assert::AreEqual(3, queue.GetSize());
            Assert::AreEqual(10, queue.GetFront());

            Assert::AreEqual(10, queue.Dequeue());
            Assert::AreEqual(20, queue.GetFront());
            Assert::AreEqual(20, queue.Dequeue());
            Assert::AreEqual(30, queue.Dequeue());
            Assert::IsTrue(queue.IsEmpty());
        }

        TEST_METHOD(Queue_WrapAround_MaintainsOrder)
        {
            Queue<int> queue;
            queue.Reserve(4);

            queue.Enqueue(1);
            queue.Enqueue(2);
            queue.Enqueue(3);
            queue.Enqueue(4);

            Assert::AreEqual(1, queue.Dequeue());
            Assert::AreEqual(2, queue.Dequeue());

            queue.Enqueue(5);
            queue.Enqueue(6);

            Assert::AreEqual(3, queue.Dequeue());
            Assert::AreEqual(4, queue.Dequeue());
            Assert::AreEqual(5, queue.Dequeue());
            Assert::AreEqual(6, queue.Dequeue());
            Assert::IsTrue(queue.IsEmpty());
        }

        TEST_METHOD(Queue_CopyAndClear_Work)
        {
            Queue<std::string> original;
            original.Enqueue("a");
            original.Enqueue("b");

            Queue<std::string> copy(original);
            Assert::AreEqual(std::string("a"), copy.Dequeue());
            copy.Enqueue("c");

            Assert::AreEqual(std::string("a"), original.Dequeue());
            Assert::AreEqual(std::string("b"), original.Dequeue());

            copy.Clear();
            Assert::IsTrue(copy.IsEmpty());
        }
    };
}
