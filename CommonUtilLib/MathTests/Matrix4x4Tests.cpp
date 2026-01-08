#include "pch.h"
#include "CppUnitTest.h"

#include <cmath>
#include <sstream>

#include "../CommonUtilLib/Matrix.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using CommonUtilities::Matrix3x3;
using CommonUtilities::Matrix4x4;
using CommonUtilities::Vector4;

namespace MatrixTests
{
	namespace
	{
		static void AssertMatrix4x4Equal(const Matrix4x4<float>& a, const Matrix4x4<float>& b, float eps = 1e-6f)
		{
			for (int r = 0; r < 4; ++r)
			{
				for (int c = 0; c < 4; ++c)
				{
					Assert::AreEqual(a(r, c), b(r, c), eps);
				}
			}
		}
	}

	TEST_CLASS(Matrix4x4Tests)
	{
	public:
		TEST_METHOD(Constructor_Default_IsIdentity)
		{
			Matrix4x4<float> m;
			for (int r = 0; r < 4; ++r)
			{
				for (int c = 0; c < 4; ++c)
				{
					const float expected = (r == c) ? 1.0f : 0.0f;
					Assert::AreEqual(expected, m(r, c), 1e-6f);
				}
			}
		}

		TEST_METHOD(Constructor_FromMatrix3x3_CopiesAndSetsHomogeneous)
		{
			Matrix3x3<float> m3{ 1,2,3, 4,5,6, 7,8,9 };
			Matrix4x4<float> m4(m3);

			for (int r = 0; r < 3; ++r)	// top-left copy
			{
				for (int c = 0; c < 3; ++c)
				{
					Assert::AreEqual(m3(r, c), m4(r, c), 1e-6f);
				}
			}

			// last row/col should be (0,0,0,1)
			for (int i = 0; i < 4; ++i)
			{
				const float expected = (i == 3) ? 1.0f : 0.0f;
				Assert::AreEqual(expected, m4(i, 3), 1e-6f);
				Assert::AreEqual(expected, m4(3, i), 1e-6f);
			}
		}

		TEST_METHOD(Constructor_FillInitializerList_RowMajor)
		{
			Matrix4x4<float> m{ 1,2,3,4,  5,6,7,8,  9,10,11,12,  13,14,15,16 };

			Assert::AreEqual(1.0f, m(0, 0), 1e-6f);
			Assert::AreEqual(4.0f, m(0, 3), 1e-6f);
			Assert::AreEqual(6.0f, m(1, 1), 1e-6f);
			Assert::AreEqual(12.0f, m(2, 3), 1e-6f);
			Assert::AreEqual(13.0f, m(3, 0), 1e-6f);
			Assert::AreEqual(16.0f, m(3, 3), 1e-6f);
		}

		TEST_METHOD(Constructor_NestedInitializerList_RowMajor)
		{
			Matrix4x4<float> m{ {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} };

			Assert::AreEqual(1.0f, m(0, 0), 1e-6f);
			Assert::AreEqual(4.0f, m(0, 3), 1e-6f);
			Assert::AreEqual(6.0f, m(1, 1), 1e-6f);
			Assert::AreEqual(12.0f, m(2, 3), 1e-6f);
			Assert::AreEqual(13.0f, m(3, 0), 1e-6f);
			Assert::AreEqual(16.0f, m(3, 3), 1e-6f);
		}

		TEST_METHOD(Constructor_Copy_CopiesAllElements)
		{
			Matrix4x4<float> a{ 1,2,3,4,  5,6,7,8,  9,10,11,12,  13,14,15,16 };
			Matrix4x4<float> b(a);
			AssertMatrix4x4Equal(a, b);
		}

		TEST_METHOD(OperatorAssign_CopiesAllElements)
		{
			Matrix4x4<float> a{ 1,2,3,4,  5,6,7,8,  9,10,11,12,  13,14,15,16 };
			Matrix4x4<float> b;
			b = a;
			AssertMatrix4x4Equal(a, b);
		}

		TEST_METHOD(OperatorCall_ReadWrite_Works)
		{
			Matrix4x4<float> m;
			m(0, 1) = 2.5f;
			m(3, 2) = -4.75f;

			const Matrix4x4<float>& cm = m;
			Assert::AreEqual(2.5f, cm(0, 1), 1e-6f);
			Assert::AreEqual(-4.75f, cm(3, 2), 1e-6f);
		}

		TEST_METHOD(OperatorIndex_RowAccess_ReadWrite_Works)
		{
			Matrix4x4<float> m;
			m[2][1] = 9.25f;
			m[0][3] = -2.0f;

			const Matrix4x4<float>& cm = m;
			Assert::AreEqual(9.25f, cm[2][1], 1e-6f);
			Assert::AreEqual(-2.0f, cm[0][3], 1e-6f);
		}

		TEST_METHOD(GetRow_ReturnsCorrectRow)
		{
			Matrix4x4<float> m{ 1,2,3,4,  5,6,7,8,  9,10,11,12,  13,14,15,16 };
			Vector4<float> r2 = m.GetRow(2);

			Assert::AreEqual(9.0f, r2.x, 1e-6f);
			Assert::AreEqual(10.0f, r2.y, 1e-6f);
			Assert::AreEqual(11.0f, r2.z, 1e-6f);
			Assert::AreEqual(12.0f, r2.w, 1e-6f);
		}

		TEST_METHOD(GetColumn_ReturnsCorrectColumn)
		{
			Matrix4x4<float> m{ 1,2,3,4,  5,6,7,8,  9,10,11,12,  13,14,15,16 };
			Vector4<float> c1 = m.GetColumn(1);

			Assert::AreEqual(2.0f, c1.x, 1e-6f);
			Assert::AreEqual(6.0f, c1.y, 1e-6f);
			Assert::AreEqual(10.0f, c1.z, 1e-6f);
			Assert::AreEqual(14.0f, c1.w, 1e-6f);
		}

		TEST_METHOD(GetTranspose_ReturnsTransposedMatrix)
		{
			Matrix4x4<float> m{ 1,2,3,4,  5,6,7,8,  9,10,11,12,  13,14,15,16 };
			Matrix4x4<float> t = m.GetTranspose();

			for (int r = 0; r < 4; ++r)
			{
				for (int c = 0; c < 4; ++c)
				{
					Assert::AreEqual(m(r, c), t(c, r), 1e-6f);
				}
			}
		}

		TEST_METHOD(CreateRotationAroundX_CreatesCorrectMatrix)
		{
			const float angle = 3.1415926535f / 4.0f;
			const float c = std::cos(angle);
			const float s = std::sin(angle);
			Matrix4x4<float> r = Matrix4x4<float>::CreateRotationAroundX(angle);

			Assert::AreEqual(1.0f, r(0, 0), 1e-5f);
			Assert::AreEqual(0.0f, r(0, 1), 1e-5f);
			Assert::AreEqual(0.0f, r(0, 2), 1e-5f);
			Assert::AreEqual(0.0f, r(0, 3), 1e-5f);

			Assert::AreEqual(0.0f, r(1, 0), 1e-5f);
			Assert::AreEqual(c, r(1, 1), 1e-5f);
			Assert::AreEqual(s, r(1, 2), 1e-5f);
			Assert::AreEqual(0.0f, r(1, 3), 1e-5f);

			Assert::AreEqual(0.0f, r(2, 0), 1e-5f);
			Assert::AreEqual(-s, r(2, 1), 1e-5f);
			Assert::AreEqual(c, r(2, 2), 1e-5f);
			Assert::AreEqual(0.0f, r(2, 3), 1e-5f);

			Assert::AreEqual(0.0f, r(3, 0), 1e-5f);
			Assert::AreEqual(0.0f, r(3, 1), 1e-5f);
			Assert::AreEqual(0.0f, r(3, 2), 1e-5f);
			Assert::AreEqual(1.0f, r(3, 3), 1e-5f);
		}

		TEST_METHOD(CreateRotationAroundY_CreatesCorrectMatrix)
		{
			const float angle = 3.1415926535f / 3.0f;
			const float c = std::cos(angle);
			const float s = std::sin(angle);
			Matrix4x4<float> r = Matrix4x4<float>::CreateRotationAroundY(angle);

			Assert::AreEqual(c, r(0, 0), 1e-5f);
			Assert::AreEqual(0.0f, r(0, 1), 1e-5f);
			Assert::AreEqual(-s, r(0, 2), 1e-5f);
			Assert::AreEqual(0.0f, r(0, 3), 1e-5f);

			Assert::AreEqual(0.0f, r(1, 0), 1e-5f);
			Assert::AreEqual(1.0f, r(1, 1), 1e-5f);
			Assert::AreEqual(0.0f, r(1, 2), 1e-5f);
			Assert::AreEqual(0.0f, r(1, 3), 1e-5f);

			Assert::AreEqual(s, r(2, 0), 1e-5f);
			Assert::AreEqual(0.0f, r(2, 1), 1e-5f);
			Assert::AreEqual(c, r(2, 2), 1e-5f);
			Assert::AreEqual(0.0f, r(2, 3), 1e-5f);

			Assert::AreEqual(0.0f, r(3, 0), 1e-5f);
			Assert::AreEqual(0.0f, r(3, 1), 1e-5f);
			Assert::AreEqual(0.0f, r(3, 2), 1e-5f);
			Assert::AreEqual(1.0f, r(3, 3), 1e-5f);
		}

		TEST_METHOD(CreateRotationAroundZ_CreatesCorrectMatrix)
		{
			const float angle = -3.1415926535f / 6.0f;
			const float c = std::cos(angle);
			const float s = std::sin(angle);
			Matrix4x4<float> r = Matrix4x4<float>::CreateRotationAroundZ(angle);

			Assert::AreEqual(c, r(0, 0), 1e-5f);
			Assert::AreEqual(s, r(0, 1), 1e-5f);
			Assert::AreEqual(0.0f, r(0, 2), 1e-5f);
			Assert::AreEqual(0.0f, r(0, 3), 1e-5f);

			Assert::AreEqual(-s, r(1, 0), 1e-5f);
			Assert::AreEqual(c, r(1, 1), 1e-5f);
			Assert::AreEqual(0.0f, r(1, 2), 1e-5f);
			Assert::AreEqual(0.0f, r(1, 3), 1e-5f);

			Assert::AreEqual(0.0f, r(2, 0), 1e-5f);
			Assert::AreEqual(0.0f, r(2, 1), 1e-5f);
			Assert::AreEqual(1.0f, r(2, 2), 1e-5f);
			Assert::AreEqual(0.0f, r(2, 3), 1e-5f);

			Assert::AreEqual(0.0f, r(3, 0), 1e-5f);
			Assert::AreEqual(0.0f, r(3, 1), 1e-5f);
			Assert::AreEqual(0.0f, r(3, 2), 1e-5f);
			Assert::AreEqual(1.0f, r(3, 3), 1e-5f);
		}

		TEST_METHOD(OperatorPlusEquals_AddsElementWise)
		{
			Matrix4x4<float> a{ 1,2,3,4,  5,6,7,8,  9,10,11,12,  13,14,15,16 };
			Matrix4x4<float> b{ 16,15,14,13,  12,11,10,9,  8,7,6,5,  4,3,2,1 };
			a += b;

			Assert::AreEqual(17.0f, a(0, 0), 1e-6f);
			Assert::AreEqual(17.0f, a(1, 1), 1e-6f);
			Assert::AreEqual(17.0f, a(2, 2), 1e-6f);
			Assert::AreEqual(17.0f, a(3, 3), 1e-6f);
		}

		TEST_METHOD(OperatorMinusEquals_SubtractsElementWise)
		{
			Matrix4x4<float> a{ 1,2,3,4,  5,6,7,8,  9,10,11,12,  13,14,15,16 };
			Matrix4x4<float> b{ 1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1 };
			a -= b;

			Assert::AreEqual(0.0f, a(0, 0), 1e-6f);
			Assert::AreEqual(1.0f, a(0, 1), 1e-6f);
			Assert::AreEqual(15.0f, a(3, 3), 1e-6f);
		}

		TEST_METHOD(OperatorTimesEquals_MatrixMultiplies)
		{
			Matrix4x4<float> a{ 1,2,3,4,  5,6,7,8,  9,10,11,12,  13,14,15,16 };
			Matrix4x4<float> i;
			a *= i;

			Matrix4x4<float> expected{ 1,2,3,4,  5,6,7,8,  9,10,11,12,  13,14,15,16 };
			AssertMatrix4x4Equal(expected, a);
		}

		TEST_METHOD(OperatorTimesEquals_ScalarMultiplies)
		{
			Matrix4x4<float> a{ 1,2,3,4,  5,6,7,8,  9,10,11,12,  13,14,15,16 };
			a *= 2.0f;

			Assert::AreEqual(2.0f, a(0, 0), 1e-6f);
			Assert::AreEqual(12.0f, a(1, 1), 1e-6f);
			Assert::AreEqual(22.0f, a(2, 2), 1e-6f);
			Assert::AreEqual(32.0f, a(3, 3), 1e-6f);
		}

		TEST_METHOD(OperatorEquals_And_NotEquals)
		{
			Matrix4x4<float> a{ 1,2,3,4,  5,6,7,8,  9,10,11,12,  13,14,15,16 };
			Matrix4x4<float> b{ 1,2,3,4,  5,6,7,8,  9,10,11,12,  13,14,15,16 };
			Matrix4x4<float> c{ 1,2,3,4,  5,6,7,8,  9,10,11,12,  13,14,15,0 };

			Assert::IsTrue(a == b);
			Assert::IsFalse(a != b);

			Assert::IsFalse(a == c);
			Assert::IsTrue(a != c);
		}

		TEST_METHOD(OperatorStreamOutput_FormatsAsRows)
		{
			Matrix4x4<float> m{ 1,2,3,4,  5,6,7,8,  9,10,11,12,  13,14,15,16 };
			std::ostringstream oss;
			oss << m;

			const std::string s = oss.str();
			Assert::IsTrue(s.find("[1") != std::string::npos);
			Assert::IsTrue(s.find("16") != std::string::npos);
			Assert::IsTrue(s.find("\n") != std::string::npos);
		}
	};
}
