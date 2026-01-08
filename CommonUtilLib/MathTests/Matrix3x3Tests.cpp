#include "pch.h"
#include "CppUnitTest.h"
#include <cmath>
#include "../CommonUtilLib/Matrix.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using CommonUtilities::Matrix3x3;
using CommonUtilities::Matrix4x4;
using CommonUtilities::Vector3;

namespace MatrixTests
{
	namespace CommonUtilLibTests
	{
		TEST_CLASS(Matrix3x3Tests)
		{
		public:
			TEST_METHOD(Constructor_Default_IsIdentity)
			{
				Matrix3x3<float> m;

				for (int r = 0; r < 3; ++r)
				{
					for (int c = 0; c < 3; ++c)
					{
						float expected = (r == c) ? 1.0f : 0.0f;
						Assert::AreEqual(expected, m(r, c), 1e-6f, L"Default ctor should create identity matrix");
					}
				}
			}

			TEST_METHOD(Constructor_FillInitializerList_RowMajor)
			{
				Matrix3x3<float> m{ 1,2,3, 4,5,6, 7,8,9 };

				Assert::AreEqual(1.0f, m(0, 0), 1e-6f);
				Assert::AreEqual(2.0f, m(0, 1), 1e-6f);
				Assert::AreEqual(3.0f, m(0, 2), 1e-6f);
				Assert::AreEqual(4.0f, m(1, 0), 1e-6f);
				Assert::AreEqual(5.0f, m(1, 1), 1e-6f);
				Assert::AreEqual(6.0f, m(1, 2), 1e-6f);
				Assert::AreEqual(7.0f, m(2, 0), 1e-6f);
				Assert::AreEqual(8.0f, m(2, 1), 1e-6f);
				Assert::AreEqual(9.0f, m(2, 2), 1e-6f);
			}

			TEST_METHOD(Constructor_NestedInitializerList_RowMajor)
			{
				Matrix3x3<float> m{ {1,2,3}, {4,5,6}, {7,8,9} };

				Assert::AreEqual(1.0f, m(0, 0), 1e-6f);
				Assert::AreEqual(2.0f, m(0, 1), 1e-6f);
				Assert::AreEqual(3.0f, m(0, 2), 1e-6f);
				Assert::AreEqual(4.0f, m(1, 0), 1e-6f);
				Assert::AreEqual(5.0f, m(1, 1), 1e-6f);
				Assert::AreEqual(6.0f, m(1, 2), 1e-6f);
				Assert::AreEqual(7.0f, m(2, 0), 1e-6f);
				Assert::AreEqual(8.0f, m(2, 1), 1e-6f);
				Assert::AreEqual(9.0f, m(2, 2), 1e-6f);
			}

			TEST_METHOD(Constructor_Copy_CopiesAllElements)
			{
				Matrix3x3<float> a{ 1,2,3, 4,5,6, 7,8,9 };
				Matrix3x3<float> b(a);

				for (int r = 0; r < 3; ++r)
				{
					for (int c = 0; c < 3; ++c)
					{
						Assert::AreEqual(a(r, c), b(r, c), 1e-6f);
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
						Assert::AreEqual(a(r, c), b(r, c), 1e-6f);
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
						Assert::AreEqual(m4(r, c), m3(r, c), 1e-6f, L"3x3 should copy top-left of 4x4");
					}
				}
			}

			TEST_METHOD(OperatorCall_ReadWrite_Works)
			{
				Matrix3x3<float> m;

				m(0, 1) = 3.5f;
				m(2, 2) = -7.25f;

				const Matrix3x3<float>& cm = m;

				Assert::AreEqual(3.5f, cm(0, 1), 1e-6f);
				Assert::AreEqual(-7.25f, cm(2, 2), 1e-6f);
			}

			TEST_METHOD(OperatorIndex_RowAccess_ReadWrite_Works)
			{
				Matrix3x3<float> m;

				m[1][2] = 42.0f;
				m[2][0] = -5.0f;

				const Matrix3x3<float>& cm = m;
				Assert::AreEqual(42.0f, cm[1][2], 1e-6f);
				Assert::AreEqual(-5.0f, cm[2][0], 1e-6f);
			}

			TEST_METHOD(GetRow_ReturnsCorrectRow)
			{
				Matrix3x3<float> m{ 1,2,3, 4,5,6, 7,8,9 };

				Vector3<float> r1 = m.GetRow(1);
				Assert::AreEqual(4.0f, r1.x, 1e-6f);
				Assert::AreEqual(5.0f, r1.y, 1e-6f);
				Assert::AreEqual(6.0f, r1.z, 1e-6f);
			}

			TEST_METHOD(GetColumn_ReturnsCorrectColumn)
			{
				Matrix3x3<float> m{ 1,2,3, 4,5,6, 7,8,9 };

				Vector3<float> c2 = m.GetColumn(2);
				Assert::AreEqual(3.0f, c2.x, 1e-6f);
				Assert::AreEqual(6.0f, c2.y, 1e-6f);
				Assert::AreEqual(9.0f, c2.z, 1e-6f);
			}

			TEST_METHOD(GetTranspose_ReturnsTransposedMatrix)
			{
				Matrix3x3<float> m{ 1,2,3, 4,5,6, 7,8,9 };

				Matrix3x3<float> t = m.GetTranspose();

				for (int r = 0; r < 3; ++r)
				{
					for (int c = 0; c < 3; ++c)
					{
						Assert::AreEqual(m(r, c), t(c, r), 1e-6f, L"Transpose mismatch");
					}
				}
			}

			TEST_METHOD(CreateRotationAroundX_CreatesCorrectMatrix)
			{
				const float angle = 3.1415926535f / 4.0f;
				const float c = std::cos(angle);
				const float s = std::sin(angle);

				Matrix3x3<float> r = Matrix3x3<float>::CreateRotationAroundX(angle);

				Assert::AreEqual(1.0f, r(0, 0), 1e-5f);
				Assert::AreEqual(0.0f, r(0, 1), 1e-5f);
				Assert::AreEqual(0.0f, r(0, 2), 1e-5f);

				Assert::AreEqual(c, r(1, 1), 1e-5f);
				Assert::AreEqual(s, r(1, 2), 1e-5f);
				Assert::AreEqual(-s, r(2, 1), 1e-5f);
				Assert::AreEqual(c, r(2, 2), 1e-5f);
			}

			TEST_METHOD(CreateRotationAroundY_CreatesCorrectMatrix)
			{
				const float angle = 3.1415926535f / 3.0f;
				const float c = std::cos(angle);
				const float s = std::sin(angle);

				Matrix3x3<float> r = Matrix3x3<float>::CreateRotationAroundY(angle);

				Assert::AreEqual(c, r(0, 0), 1e-5f);
				Assert::AreEqual(0.0f, r(0, 1), 1e-5f);
				Assert::AreEqual(-s, r(0, 2), 1e-5f);

				Assert::AreEqual(0.0f, r(1, 0), 1e-5f);
				Assert::AreEqual(1.0f, r(1, 1), 1e-5f);
				Assert::AreEqual(0.0f, r(1, 2), 1e-5f);

				Assert::AreEqual(s, r(2, 0), 1e-5f);
				Assert::AreEqual(0.0f, r(2, 1), 1e-5f);
				Assert::AreEqual(c, r(2, 2), 1e-5f);
			}

			TEST_METHOD(CreateRotationAroundZ_CreatesCorrectMatrix)
			{
				const float angle = -3.1415926535f / 6.0f;
				const float c = std::cos(angle);
				const float s = std::sin(angle);

				Matrix3x3<float> r = Matrix3x3<float>::CreateRotationAroundZ(angle);

				Assert::AreEqual(c, r(0, 0), 1e-5f);
				Assert::AreEqual(s, r(0, 1), 1e-5f);
				Assert::AreEqual(0.0f, r(0, 2), 1e-5f);

				Assert::AreEqual(-s, r(1, 0), 1e-5f);
				Assert::AreEqual(c, r(1, 1), 1e-5f);
				Assert::AreEqual(0.0f, r(1, 2), 1e-5f);

				Assert::AreEqual(0.0f, r(2, 0), 1e-5f);
				Assert::AreEqual(0.0f, r(2, 1), 1e-5f);
				Assert::AreEqual(1.0f, r(2, 2), 1e-5f);
			}

			TEST_METHOD(OperatorPlusEquals_AddsElementWise)
			{
				Matrix3x3<float> a{ 1,2,3, 4,5,6, 7,8,9 };
				Matrix3x3<float> b{ 9,8,7, 6,5,4, 3,2,1 };
				a += b;

				Assert::AreEqual(10.0f, a(0, 0), 1e-6f);
				Assert::AreEqual(10.0f, a(1, 1), 1e-6f);
				Assert::AreEqual(10.0f, a(2, 2), 1e-6f);
			}

			TEST_METHOD(OperatorMinusEquals_SubtractsElementWise)
			{
				Matrix3x3<float> a{ 1,2,3, 4,5,6, 7,8,9 };
				Matrix3x3<float> b{ 1,1,1, 1,1,1, 1,1,1 };
				a -= b;

				Assert::AreEqual(0.0f, a(0, 0), 1e-6f);
				Assert::AreEqual(1.0f, a(0, 1), 1e-6f);
				Assert::AreEqual(8.0f, a(2, 2), 1e-6f);
			}

			TEST_METHOD(OperatorTimesEquals_MatrixMultiplies)
			{
				// A * I == A
				Matrix3x3<float> a{ 1,2,3, 4,5,6, 7,8,9 };
				Matrix3x3<float> i;
				a *= i;

				Assert::AreEqual(1.0f, a(0, 0), 1e-6f);
				Assert::AreEqual(2.0f, a(0, 1), 1e-6f);
				Assert::AreEqual(3.0f, a(0, 2), 1e-6f);
				Assert::AreEqual(4.0f, a(1, 0), 1e-6f);
				Assert::AreEqual(5.0f, a(1, 1), 1e-6f);
				Assert::AreEqual(6.0f, a(1, 2), 1e-6f);
				Assert::AreEqual(7.0f, a(2, 0), 1e-6f);
				Assert::AreEqual(8.0f, a(2, 1), 1e-6f);
				Assert::AreEqual(9.0f, a(2, 2), 1e-6f);
			}

			TEST_METHOD(OperatorTimesEquals_ScalarMultiplies)
			{
				Matrix3x3<float> a{ 1,2,3, 4,5,6, 7,8,9 };
				a *= 2.0f;

				Assert::AreEqual(2.0f, a(0, 0), 1e-6f);
				Assert::AreEqual(10.0f, a(1, 1), 1e-6f);
				Assert::AreEqual(18.0f, a(2, 2), 1e-6f);
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
		};

		// Matrix4x4 tests kept as-is
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
						float expected = (r == c) ? 1.0f : 0.0f;
						Assert::AreEqual(expected, m(r, c), 1e-6f, L"Default ctor should create identity matrix");
					}
				}
			}

			TEST_METHOD(Constructor_FromMatrix3x3_CopiesAndSetsHomogeneous)
			{
				Matrix3x3<float> m3;

				// Fill 3x3 with distinct values
				float value = 1.0f;
				for (int r = 0; r < 3; ++r)
				{
					for (int c = 0; c < 3; ++c)
					{
						m3(r, c) = value;
						value += 1.0f;
					}
				}

				Matrix4x4<float> m4(m3);

				// Top-left 3x3 should match
				for (int r = 0; r < 3; ++r)
				{
					for (int c = 0; c < 3; ++c)
					{
						Assert::AreEqual(m3(r, c), m4(r, c), 1e-6f, L"4x4 should copy top-left from 3x3");
					}
				}

				// Last row and column should form homogeneous coordinates row/column
				for (int i = 0; i < 4; ++i)
				{
					float expected = (i == 3) ? 1.0f : 0.0f;
					Assert::AreEqual(expected, m4(i, 3), 1e-6f, L"Last column should be (0,0,0,1)^T");
					Assert::AreEqual(expected, m4(3, i), 1e-6f, L"Last row should be (0,0,0,1)");
				}
			}

			TEST_METHOD(OperatorIndex_ReadWrite_Works)
			{
				Matrix4x4<float> m;

				m(0, 1) = 2.5f;
				m(3, 2) = -4.75f;

				const Matrix4x4<float>& cm = m;

				Assert::AreEqual(2.5f, cm(0, 1), 1e-6f);
				Assert::AreEqual(-4.75f, cm(3, 2), 1e-6f);
			}

			TEST_METHOD(GetTranspose_ReturnsTransposedMatrix)
			{
				Matrix4x4<float> m;

				float value = 1.0f;
				for (int r = 0; r < 4; ++r)
				{
					for (int c = 0; c < 4; ++c)
					{
						m(r, c) = value;
						value += 1.0f;
					}
				}

				Matrix4x4<float> t = m.GetTranspose();

				for (int r = 0; r < 4; ++r)
				{
					for (int c = 0; c < 4; ++c)
					{
						Assert::AreEqual(m(r, c), t(c, r), 1e-6f, L"Transpose mismatch");
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
		};
	}
}
