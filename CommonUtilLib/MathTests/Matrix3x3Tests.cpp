#include "pch.h"
#include "CppUnitTest.h"
#include <cmath>
#include <sstream>
#include <stdexcept>
#include "../CommonUtilLib/Matrix.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using CommonUtilities::Matrix3x3;
using CommonUtilities::Matrix4x4;
using CommonUtilities::Vector3;

namespace MatrixTests
{
	TEST_CLASS(Matrix3x3Tests)
	{
	public:
		float eps = 1e-5f;

		TEST_METHOD(Constructor_Default_IsIdentity)
		{
			Matrix3x3<float> m;

			for (int r = 0; r < 3; ++r)
			{
				for (int c = 0; c < 3; ++c)
				{
					float expected = (r == c) ? 1.0f : 0.0f;
					Assert::AreEqual(expected, m(r, c), eps, L"Default ctor should create identity matrix");
				}
			}
		}

		TEST_METHOD(Constructor_FillInitializerList_RowMajor)
		{
			Matrix3x3<float> m{ 1,2,3, 4,5,6, 7,8,9 };

			Assert::AreEqual(1.0f, m(0, 0), eps);
			Assert::AreEqual(2.0f, m(0, 1), eps);
			Assert::AreEqual(3.0f, m(0, 2), eps);
			Assert::AreEqual(4.0f, m(1, 0), eps);
			Assert::AreEqual(5.0f, m(1, 1), eps);
			Assert::AreEqual(6.0f, m(1, 2), eps);
			Assert::AreEqual(7.0f, m(2, 0), eps);
			Assert::AreEqual(8.0f, m(2, 1), eps);
			Assert::AreEqual(9.0f, m(2, 2), eps);
		}

		TEST_METHOD(Constructor_FillInitializerList_WrongCount_Throws)
		{
			Assert::ExpectException<std::invalid_argument>([]()
			{
				Matrix3x3<float> m{ 1,2,3 };
				(void)m;
			});
		}

		TEST_METHOD(Constructor_NestedInitializerList_RowMajor)
		{
			Matrix3x3<float> m{ {1,2,3}, {4,5,6}, {7,8,9} };

			Assert::AreEqual(1.0f, m(0, 0), eps);
			Assert::AreEqual(2.0f, m(0, 1), eps);
			Assert::AreEqual(3.0f, m(0, 2), eps);
			Assert::AreEqual(4.0f, m(1, 0), eps);
			Assert::AreEqual(5.0f, m(1, 1), eps);
			Assert::AreEqual(6.0f, m(1, 2), eps);
			Assert::AreEqual(7.0f, m(2, 0), eps);
			Assert::AreEqual(8.0f, m(2, 1), eps);
			Assert::AreEqual(9.0f, m(2, 2), eps);
		}

		TEST_METHOD(Constructor_NestedInitializerList_WrongRowCount_Throws)
		{
			Assert::ExpectException<std::invalid_argument>([]()
			{
				Matrix3x3<float> m{ {1,2,3}, {4,5,6} };
				(void)m;
			});
		}

		TEST_METHOD(Constructor_NestedInitializerList_WrongColumnCount_Throws)
		{
			Assert::ExpectException<std::invalid_argument>([]()
			{
				Matrix3x3<float> m{ {1,2,3}, {4,5}, {7,8,9} };
				(void)m;
			});
		}

		TEST_METHOD(Constructor_Copy_CopiesAllElements)
		{
			Matrix3x3<float> a{ 1,2,3, 4,5,6, 7,8,9 };
			Matrix3x3<float> b(a);

			for (int r = 0; r < 3; ++r)
			{
				for (int c = 0; c < 3; ++c)
				{
					Assert::AreEqual(a(r, c), b(r, c), eps);
				}
			}
		}

		TEST_METHOD(OperatorAssign_CopiesAllElements)
		{
			Matrix3x3<float> a{ 1,2,3, 4,5,6, 7,8,9 };
			Matrix3x3<float> b;
			b = a;

			for (int r = 0; r < 3; ++r)
			{
				for (int c = 0; c < 3; ++c)
				{
					Assert::AreEqual(a(r, c), b(r, c), eps);
				}
			}
		}

		TEST_METHOD(Constructor_FromMatrix4x4_CopiesTopLeft3x3)
		{
			Matrix4x4<float> m4;

			float value = 1.0f;
			for (int r = 0; r < 4; ++r)
			{
				for (int c = 0; c < 4; ++c)
				{
					m4(r, c) = value;
					value += 1.0f;
				}
			}

			Matrix3x3<float> m3(m4);

			for (int r = 0; r < 3; ++r)
			{
				for (int c = 0; c < 3; ++c)
				{
					Assert::AreEqual(m4(r, c), m3(r, c), eps, L"3x3 should copy top-left of 4x4");
				}
			}
		}

		TEST_METHOD(OperatorCall_ReadWrite_Works)
		{
			Matrix3x3<float> m;

			m(0, 1) = 3.5f;
			m(2, 2) = -7.25f;

			const Matrix3x3<float>& cm = m;

			Assert::AreEqual(3.5f, cm(0, 1), eps);
			Assert::AreEqual(-7.25f, cm(2, 2), eps);
		}

		TEST_METHOD(OperatorIndex_RowAccess_ReadWrite_Works)
		{
			Matrix3x3<float> m;

			m[1][2] = 42.0f;
			m[2][0] = -5.0f;

			const Matrix3x3<float>& cm = m;
			Assert::AreEqual(42.0f, cm[1][2], eps);
			Assert::AreEqual(-5.0f, cm[2][0], eps);
		}

		TEST_METHOD(GetRow_ReturnsCorrectRow)
		{
			Matrix3x3<float> m{ 1,2,3, 4,5,6, 7,8,9 };

			Vector3<float> r1 = m.GetRow(1);
			Assert::AreEqual(4.0f, r1.x, eps);
			Assert::AreEqual(5.0f, r1.y, eps);
			Assert::AreEqual(6.0f, r1.z, eps);
		}

		TEST_METHOD(GetColumn_ReturnsCorrectColumn)
		{
			Matrix3x3<float> m{ 1,2,3, 4,5,6, 7,8,9 };

			Vector3<float> c2 = m.GetColumn(2);
			Assert::AreEqual(3.0f, c2.x, eps);
			Assert::AreEqual(6.0f, c2.y, eps);
			Assert::AreEqual(9.0f, c2.z, eps);
		}

		TEST_METHOD(GetTranspose_ReturnsTransposedMatrix)
		{
			Matrix3x3<float> m{ 1,2,3, 4,5,6, 7,8,9 };

			Matrix3x3<float> t = m.GetTranspose();

			for (int r = 0; r < 3; ++r)
			{
				for (int c = 0; c < 3; ++c)
				{
					Assert::AreEqual(m(r, c), t(c, r), eps, L"Transpose mismatch");
				}
			}
		}

		TEST_METHOD(CreateRotationAroundX_CreatesCorrectMatrix)
		{
			const float angle = 3.1415926535f / 4.0f;
			const float c = std::cos(angle);
			const float s = std::sin(angle);

			Matrix3x3<float> r = Matrix3x3<float>::CreateRotationAroundX(angle);

			Assert::AreEqual(1.0f, r(0, 0), eps);
			Assert::AreEqual(0.0f, r(0, 1), eps);
			Assert::AreEqual(0.0f, r(0, 2), eps);

			Assert::AreEqual(c, r(1, 1), eps);
			Assert::AreEqual(s, r(1, 2), eps);
			Assert::AreEqual(-s, r(2, 1), eps);
			Assert::AreEqual(c, r(2, 2), eps);
		}

		TEST_METHOD(CreateRotationAroundY_CreatesCorrectMatrix)
		{
			const float angle = 3.1415926535f / 3.0f;
			const float c = std::cos(angle);
			const float s = std::sin(angle);

			Matrix3x3<float> r = Matrix3x3<float>::CreateRotationAroundY(angle);

			Assert::AreEqual(c, r(0, 0), eps);
			Assert::AreEqual(0.0f, r(0, 1), eps);
			Assert::AreEqual(-s, r(0, 2), eps);

			Assert::AreEqual(0.0f, r(1, 0), eps);
			Assert::AreEqual(1.0f, r(1, 1), eps);
			Assert::AreEqual(0.0f, r(1, 2), eps);

			Assert::AreEqual(s, r(2, 0), eps);
			Assert::AreEqual(0.0f, r(2, 1), eps);
			Assert::AreEqual(c, r(2, 2), eps);
		}

		TEST_METHOD(CreateRotationAroundZ_CreatesCorrectMatrix)
		{
			const float angle = -3.1415926535f / 6.0f;
			const float c = std::cos(angle);
			const float s = std::sin(angle);

			Matrix3x3<float> r = Matrix3x3<float>::CreateRotationAroundZ(angle);

			Assert::AreEqual(c, r(0, 0), eps);
			Assert::AreEqual(s, r(0, 1), eps);
			Assert::AreEqual(0.0f, r(0, 2), eps);

			Assert::AreEqual(-s, r(1, 0), eps);
			Assert::AreEqual(c, r(1, 1), eps);
			Assert::AreEqual(0.0f, r(1, 2), eps);

			Assert::AreEqual(0.0f, r(2, 0), eps);
			Assert::AreEqual(0.0f, r(2, 1), eps);
			Assert::AreEqual(1.0f, r(2, 2), eps);
		}

		TEST_METHOD(OperatorPlusEquals_AddsElementWise)
		{
			Matrix3x3<float> a{ 1,2,3, 4,5,6, 7,8,9 };
			Matrix3x3<float> b{ 9,8,7, 6,5,4, 3,2,1 };
			a += b;

			Assert::AreEqual(10.0f, a(0, 0), eps);
			Assert::AreEqual(10.0f, a(1, 1), eps);
			Assert::AreEqual(10.0f, a(2, 2), eps);
		}

		TEST_METHOD(OperatorMinusEquals_SubtractsElementWise)
		{
			Matrix3x3<float> a{ 1,2,3, 4,5,6, 7,8,9 };
			Matrix3x3<float> b{ 1,1,1, 1,1,1, 1,1,1 };
			a -= b;

			Assert::AreEqual(0.0f, a(0, 0), eps);
			Assert::AreEqual(1.0f, a(0, 1), eps);
			Assert::AreEqual(8.0f, a(2, 2), eps);
		}

		TEST_METHOD(OperatorTimesEquals_MatrixMultiplies)
		{
			// A * I == A
			Matrix3x3<float> a{ 1,2,3, 4,5,6, 7,8,9 };
			Matrix3x3<float> i;
			a *= i;

			Assert::AreEqual(1.0f, a(0, 0), eps);
			Assert::AreEqual(2.0f, a(0, 1), eps);
			Assert::AreEqual(3.0f, a(0, 2), eps);
			Assert::AreEqual(4.0f, a(1, 0), eps);
			Assert::AreEqual(5.0f, a(1, 1), eps);
			Assert::AreEqual(6.0f, a(1, 2), eps);
			Assert::AreEqual(7.0f, a(2, 0), eps);
			Assert::AreEqual(8.0f, a(2, 1), eps);
			Assert::AreEqual(9.0f, a(2, 2), eps);
		}

		TEST_METHOD(OperatorTimesEquals_ScalarMultiplies)
		{
			Matrix3x3<float> a{ 1,2,3, 4,5,6, 7,8,9 };
			a *= 2.0f;

			Assert::AreEqual(2.0f, a(0, 0), eps);
			Assert::AreEqual(10.0f, a(1, 1), eps);
			Assert::AreEqual(18.0f, a(2, 2), eps);
		}

		TEST_METHOD(OperatorEquals_And_NotEquals)
		{
			Matrix3x3<float> a{ 1,2,3, 4,5,6, 7,8,9 };
			Matrix3x3<float> b{ 1,2,3, 4,5,6, 7,8,9 };
			Matrix3x3<float> c{ 1,2,3, 4,5,6, 7,8,0 };

			Assert::IsTrue(a == b);
			Assert::IsFalse(a != b);

			Assert::IsFalse(a == c);
			Assert::IsTrue(a != c);
		}

		TEST_METHOD(FreeOperatorPlus_And_Minus_Work)
		{
			Matrix3x3<float> a{ 1,2,3, 4,5,6, 7,8,9 };
			Matrix3x3<float> b{ 9,8,7, 6,5,4, 3,2,1 };

			Matrix3x3<float> sum = a + b;
			Matrix3x3<float> diff = a - b;

			Assert::AreEqual(10.0f, sum(0, 0), eps);
			Assert::AreEqual(10.0f, sum(1, 1), eps);
			Assert::AreEqual(10.0f, sum(2, 2), eps);

			Assert::AreEqual(-8.0f, diff(0, 0), eps);
			Assert::AreEqual(0.0f, diff(1, 1), eps);
			Assert::AreEqual(8.0f, diff(2, 2), eps);
		}

		TEST_METHOD(FreeOperatorMultiply_MatrixMultiplies)
		{
			Matrix3x3<float> a{ 1,2,3, 4,5,6, 7,8,9 };
			Matrix3x3<float> i;
			Matrix3x3<float> result = a * i;

			for (int r = 0; r < 3; ++r)
			{
				for (int c = 0; c < 3; ++c)
				{
					Assert::AreEqual(a(r, c), result(r, c), eps);
				}
			}
		}

		TEST_METHOD(FreeOperatorScalarMultiply_Works)
		{
			Matrix3x3<float> a{ 1,2,3, 4,5,6, 7,8,9 };
			Matrix3x3<float> left = 2.0f * a;
			Matrix3x3<float> right = a * 2.0f;

			Assert::AreEqual(2.0f, left(0, 0), eps);
			Assert::AreEqual(10.0f, left(1, 1), eps);
			Assert::AreEqual(18.0f, left(2, 2), eps);

			for (int r = 0; r < 3; ++r)
			{
				for (int c = 0; c < 3; ++c)
				{
					Assert::AreEqual(left(r, c), right(r, c), eps);
				}
			}
		}

		TEST_METHOD(FreeOperatorVectorTimesMatrix_RowVectorConvention)
		{
			Matrix3x3<float> scale;
			scale(0, 0) = 2.0f;
			scale(1, 1) = 3.0f;
			scale(2, 2) = 4.0f;

			Vector3<float> v(1.0f, 2.0f, 3.0f);
			Vector3<float> result = v * scale;

			Assert::AreEqual(2.0f, result.x, eps);
			Assert::AreEqual(6.0f, result.y, eps);
			Assert::AreEqual(12.0f, result.z, eps);
		}

		TEST_METHOD(OperatorStreamOutput_FormatsAsRows)
		{
			Matrix3x3<float> m{ 1,2,3, 4,5,6, 7,8,9 };
			std::ostringstream oss;
			oss << m;

			const std::string s = oss.str();
			Assert::IsTrue(s.find("[1") != std::string::npos);
			Assert::IsTrue(s.find("9") != std::string::npos);
			Assert::IsTrue(s.find("\n") != std::string::npos);
		}
	};

	
}
