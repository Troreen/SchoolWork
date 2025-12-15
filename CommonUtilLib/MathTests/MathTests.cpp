#include "pch.h"
#include "CppUnitTest.h"
#include "../CommonUtilLib/Line.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using CommonUtilities::Line;
using CommonUtilities::Vector2;

namespace MathTests
{
	TEST_CLASS(LineTests)
	{
		static bool NearlyEqual(float a, float b, float eps = 1e-6f)
		{
			return std::fabs(a - b) <= eps;
		}

		static void AssertVector2Equal(const Vector2<float>& expected, const Vector2<float>& actual, float eps = 1e-6f)
		{
			Assert::IsTrue(NearlyEqual(expected.x, actual.x, eps) && NearlyEqual(expected.y, actual.y, eps), L"Vector2 components differ");
		}

	public:
		TEST_METHOD(DefaultConstructor_Uninitialized)
		{
			Line<float> line;
			Vector2<float> normal = line.GetNormal();
			Vector2<float> direction = line.GetDirection();
			AssertVector2Equal(Vector2<float>(0.f, 0.f), normal);
			AssertVector2Equal(Vector2<float>(0.f, 0.f), direction);
			Assert::IsFalse(line.IsInside(Vector2<float>(0.f, 0.f))); // uninitialized always false
		}

		TEST_METHOD(Constructor_TwoPoints_HorizontalLine)
		{
			Vector2<float> p0(0.f, 0.f);
			Vector2<float> p1(10.f, 0.f);
			Line<float> line(p0, p1);
			AssertVector2Equal(Vector2<float>(0.f, 1.f), line.GetNormal()); // normal points up
			AssertVector2Equal(Vector2<float>(1.f, 0.f), line.GetDirection());

			// Inside tests (normal points up so inside is y <= 0)
			Assert::IsTrue(line.IsInside(Vector2<float>(5.f, 0.f))); // on line
			Assert::IsTrue(line.IsInside(Vector2<float>(5.f, -2.f))); // below
			Assert::IsFalse(line.IsInside(Vector2<float>(5.f, 2.f))); // above (outside)
		}

		TEST_METHOD(Constructor_TwoPoints_VerticalLine)
		{
			Vector2<float> p0(2.f, 3.f);
			Vector2<float> p1(2.f, 7.f);
			Line<float> line(p0, p1);
			AssertVector2Equal(Vector2<float>(-1.f, 0.f), line.GetNormal()); // normal points left
			AssertVector2Equal(Vector2<float>(0.f, 1.f), line.GetDirection()); // direction up

			// Inside (normal points left so inside is x >= 2)
			Assert::IsTrue(line.IsInside(Vector2<float>(2.f, 5.f))); // on line
			Assert::IsTrue(line.IsInside(Vector2<float>(3.f, 5.f))); // right of line
			Assert::IsFalse(line.IsInside(Vector2<float>(1.f, 5.f))); // left (outside)
		}

		TEST_METHOD(Constructor_TwoPoints_Degenerate)
		{
			Vector2<float> p(1.f, 1.f);
			Line<float> line(p, p);
			AssertVector2Equal(Vector2<float>(0.f, 0.f), line.GetNormal());
			Assert::IsFalse(line.IsInside(Vector2<float>(1.f, 1.f))); // uninitialized
		}

		TEST_METHOD(InitWith2Points_Normal)
		{
			Line<float> line;
			line.InitWith2Points(Vector2<float>(0.f, 0.f), Vector2<float>(0.f, 4.f));
			AssertVector2Equal(Vector2<float>(-1.f, 0.f), line.GetNormal());
			AssertVector2Equal(Vector2<float>(0.f, 1.f), line.GetDirection());
		}

		TEST_METHOD(InitWithPointAndDirection_Normal)
		{
			Line<float> line;
			// Direction (2,2) -> normal (-2,2) normalized -> (-1/sqrt2, 1/sqrt2)
			line.InitWithPointAndDirection(Vector2<float>(1.f, 1.f), Vector2<float>(2.f, 2.f));
			const float invSqrt2 = 1.f / std::sqrt(2.f);
			AssertVector2Equal(Vector2<float>(-invSqrt2, invSqrt2), line.GetNormal());
			AssertVector2Equal(Vector2<float>(invSqrt2, invSqrt2), line.GetDirection());
		}

		TEST_METHOD(InitWithPointAndDirection_Degenerate)
		{
			Line<float> line;
			line.InitWithPointAndDirection(Vector2<float>(1.f, 1.f), Vector2<float>(0.f, 0.f));
			AssertVector2Equal(Vector2<float>(0.f, 0.f), line.GetNormal());
			Assert::IsFalse(line.IsInside(Vector2<float>(1.f, 1.f)));
		}

		TEST_METHOD(InitWithPointAndNormal_Normalization)
		{
			Line<float> line;
			line.InitWithPointAndNormal(Vector2<float>(0.f, 0.f), Vector2<float>(0.f, 5.f)); // should normalize to (0,1)
			AssertVector2Equal(Vector2<float>(0.f, 1.f), line.GetNormal());
			AssertVector2Equal(Vector2<float>(1.f, 0.f), line.GetDirection());
		}

		TEST_METHOD(IsInside_OnLineAndSides)
		{
			Line<float> line;
			line.InitWith2Points(Vector2<float>(0.f, 0.f), Vector2<float>(5.f, 0.f)); // y=0, normal (0,1)
			Assert::IsTrue(line.IsInside(Vector2<float>(2.f, 0.f))); // on line
			Assert::IsTrue(line.IsInside(Vector2<float>(2.f, -1.f))); // below
			Assert::IsFalse(line.IsInside(Vector2<float>(2.f, 1.f))); // above
		}

		TEST_METHOD(GetNormal_ReturnsStoredNormal)
		{
			Line<float> line;
			line.InitWithPointAndNormal(Vector2<float>(3.f, 4.f), Vector2<float>(4.f, 0.f)); // normal becomes (1,0)
			AssertVector2Equal(Vector2<float>(1.f, 0.f), line.GetNormal());
		}

		TEST_METHOD(GetDirection_ComputedFromNormal)
		{
			Line<float> line;
			line.InitWithPointAndNormal(Vector2<float>(0.f, 0.f), Vector2<float>(0.f, -3.f)); // normal (0,-1)
			AssertVector2Equal(Vector2<float>(-1.f, 0.f), line.GetDirection());
		}
	};
}
