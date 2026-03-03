#include "pch.h"
#include "CppUnitTest.h"

#include <array>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <string>

#include "../CommonUtilLib/Matrix.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using CommonUtilities::Matrix3x3;
using CommonUtilities::Matrix4x4;
using CommonUtilities::Vector3;

namespace MatrixTests
{
	namespace
	{
		template <typename T>
		constexpr T GetEpsilon();

		template <>
		constexpr float GetEpsilon<float>()
		{
			return 1e-5f;
		}

		template <>
		constexpr double GetEpsilon<double>()
		{
			return 1e-12;
		}

		template <typename T>
		void AssertVector3Equal(const Vector3<T>& v, const T expectedX, const T expectedY, const T expectedZ)
		{
			const T eps = GetEpsilon<T>();
			Assert::AreEqual(expectedX, v.x, eps);
			Assert::AreEqual(expectedY, v.y, eps);
			Assert::AreEqual(expectedZ, v.z, eps);
		}

		template <typename T>
		void AssertMatrix3x3Equal(const Matrix3x3<T>& m, const std::array<T, 9>& expected)
		{
			const T eps = GetEpsilon<T>();
			for (int r = 1; r <= 3; ++r)
			{
				for (int c = 1; c <= 3; ++c)
				{
					const int index = (r - 1) * 3 + (c - 1);
					Assert::AreEqual(expected[index], m(r, c), eps);
				}
			}
		}

		template <typename T>
		void Constructor_Default_IsIdentity()
		{
			Matrix3x3<T> m;
			AssertMatrix3x3Equal(m, { static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) });
		}

		template <typename T>
		void Constructor_FromMatrix4x4_CopiesTopLeft3x3()
		{
			Matrix4x4<T> m4{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7), static_cast<T>(11),
				static_cast<T>(13), static_cast<T>(17), static_cast<T>(19), static_cast<T>(23),
				static_cast<T>(29), static_cast<T>(31), static_cast<T>(37), static_cast<T>(41),
				static_cast<T>(43), static_cast<T>(47), static_cast<T>(53), static_cast<T>(59) };

			Matrix3x3<T> m3(m4);
			AssertMatrix3x3Equal(m3, { static_cast<T>(2), static_cast<T>(5), static_cast<T>(7),
				static_cast<T>(13), static_cast<T>(17), static_cast<T>(19),
				static_cast<T>(29), static_cast<T>(31), static_cast<T>(37) });
		}

		template <typename T>
		void Constructor_FillInitializerList_RowMajor()
		{
			Matrix3x3<T> m{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7),
				static_cast<T>(11), static_cast<T>(13), static_cast<T>(17),
				static_cast<T>(19), static_cast<T>(23), static_cast<T>(29) };
			AssertMatrix3x3Equal(m, { static_cast<T>(2), static_cast<T>(5), static_cast<T>(7),
				static_cast<T>(11), static_cast<T>(13), static_cast<T>(17),
				static_cast<T>(19), static_cast<T>(23), static_cast<T>(29) });
		}

		template <typename T>
		void Constructor_NestedInitializerList_RowMajor()
		{
			Matrix3x3<T> m{ { static_cast<T>(2), static_cast<T>(5), static_cast<T>(7) },
				{ static_cast<T>(11), static_cast<T>(13), static_cast<T>(17) },
				{ static_cast<T>(19), static_cast<T>(23), static_cast<T>(29) } };
			AssertMatrix3x3Equal(m, { static_cast<T>(2), static_cast<T>(5), static_cast<T>(7),
				static_cast<T>(11), static_cast<T>(13), static_cast<T>(17),
				static_cast<T>(19), static_cast<T>(23), static_cast<T>(29) });
		}

		template <typename T>
		void Constructor_CopyAndAssign_CopyAllElements()
		{
			Matrix3x3<T> source{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7),
				static_cast<T>(11), static_cast<T>(13), static_cast<T>(17),
				static_cast<T>(19), static_cast<T>(23), static_cast<T>(29) };

			Matrix3x3<T> copy(source);
			Matrix3x3<T> assigned;
			assigned = source;

			AssertMatrix3x3Equal(copy, { static_cast<T>(2), static_cast<T>(5), static_cast<T>(7),
				static_cast<T>(11), static_cast<T>(13), static_cast<T>(17),
				static_cast<T>(19), static_cast<T>(23), static_cast<T>(29) });
			AssertMatrix3x3Equal(assigned, { static_cast<T>(2), static_cast<T>(5), static_cast<T>(7),
				static_cast<T>(11), static_cast<T>(13), static_cast<T>(17),
				static_cast<T>(19), static_cast<T>(23), static_cast<T>(29) });
		}

		template <typename T>
		void OperatorCall_OneIndexedReadWrite_AndConstOverload_Work()
		{
			Matrix3x3<T> m;
			m(1, 3) = static_cast<T>(2.5);
			m(3, 2) = static_cast<T>(-4.75);

			Assert::AreEqual(static_cast<T>(2.5), m(1, 3), GetEpsilon<T>());
			Assert::AreEqual(static_cast<T>(-4.75), m(3, 2), GetEpsilon<T>());

			const Matrix3x3<T>& cm = m;
			Assert::AreEqual(static_cast<T>(2.5), cm(1, 3), GetEpsilon<T>());
			Assert::AreEqual(static_cast<T>(-4.75), cm(3, 2), GetEpsilon<T>());
		}

		template <typename T>
		void OperatorIndex_RowAccess_ReadWrite_Works()
		{
			Matrix3x3<T> m;
			m[2][1] = static_cast<T>(9.25);
			m[0][2] = static_cast<T>(-2.0);

			const Matrix3x3<T>& cm = m;
			Assert::AreEqual(static_cast<T>(9.25), cm[2][1], GetEpsilon<T>());
			Assert::AreEqual(static_cast<T>(-2.0), cm[0][2], GetEpsilon<T>());
		}

		template <typename T>
		void GetRow_GetColumn_GetTranspose_Work()
		{
			Matrix3x3<T> m{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7),
				static_cast<T>(11), static_cast<T>(13), static_cast<T>(17),
				static_cast<T>(19), static_cast<T>(23), static_cast<T>(29) };

			const Vector3<T> r0 = m.GetRow(0);
			const Vector3<T> r1 = m.GetRow(1);
			const Vector3<T> r2 = m.GetRow(2);
			const Vector3<T> c0 = m.GetColumn(0);
			const Vector3<T> c1 = m.GetColumn(1);
			const Vector3<T> c2 = m.GetColumn(2);
			const Matrix3x3<T> t = m.GetTranspose();

			AssertVector3Equal(r0, static_cast<T>(2), static_cast<T>(5), static_cast<T>(7));
			AssertVector3Equal(r1, static_cast<T>(11), static_cast<T>(13), static_cast<T>(17));
			AssertVector3Equal(r2, static_cast<T>(19), static_cast<T>(23), static_cast<T>(29));

			AssertVector3Equal(c0, static_cast<T>(2), static_cast<T>(11), static_cast<T>(19));
			AssertVector3Equal(c1, static_cast<T>(5), static_cast<T>(13), static_cast<T>(23));
			AssertVector3Equal(c2, static_cast<T>(7), static_cast<T>(17), static_cast<T>(29));

			AssertMatrix3x3Equal(t, { static_cast<T>(2), static_cast<T>(11), static_cast<T>(19),
				static_cast<T>(5), static_cast<T>(13), static_cast<T>(23),
				static_cast<T>(7), static_cast<T>(17), static_cast<T>(29) });
		}

		template <typename T>
		void CreateRotationAroundX_Y_Z_CreateExpectedMatrices()
		{
			constexpr T pi = static_cast<T>(3.14159265358979323846);
			const T angleX = pi / static_cast<T>(4);
			const T angleY = pi / static_cast<T>(3);
			const T angleZ = -pi / static_cast<T>(6);

			const T cx = static_cast<T>(std::cos(angleX));
			const T sx = static_cast<T>(std::sin(angleX));
			const T cy = static_cast<T>(std::cos(angleY));
			const T sy = static_cast<T>(std::sin(angleY));
			const T cz = static_cast<T>(std::cos(angleZ));
			const T sz = static_cast<T>(std::sin(angleZ));

			const Matrix3x3<T> rx = Matrix3x3<T>::CreateRotationAroundX(angleX);
			const Matrix3x3<T> ry = Matrix3x3<T>::CreateRotationAroundY(angleY);
			const Matrix3x3<T> rz = Matrix3x3<T>::CreateRotationAroundZ(angleZ);

			AssertMatrix3x3Equal(rx, { static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
				static_cast<T>(0), cx, sx,
				static_cast<T>(0), -sx, cx });
			AssertMatrix3x3Equal(ry, { cy, static_cast<T>(0), -sy,
				static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
				sy, static_cast<T>(0), cy });
			AssertMatrix3x3Equal(rz, { cz, sz, static_cast<T>(0),
				-sz, cz, static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) });
		}

		template <typename T>
		void CompoundAssignmentOperators_Work()
		{
			Matrix3x3<T> a{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7),
				static_cast<T>(11), static_cast<T>(13), static_cast<T>(17),
				static_cast<T>(19), static_cast<T>(23), static_cast<T>(29) };
			Matrix3x3<T> b{ static_cast<T>(31), static_cast<T>(37), static_cast<T>(41),
				static_cast<T>(43), static_cast<T>(47), static_cast<T>(53),
				static_cast<T>(59), static_cast<T>(61), static_cast<T>(67) };

			Matrix3x3<T> plus = a;
			plus += b;
			AssertMatrix3x3Equal(plus, { static_cast<T>(33), static_cast<T>(42), static_cast<T>(48),
				static_cast<T>(54), static_cast<T>(60), static_cast<T>(70),
				static_cast<T>(78), static_cast<T>(84), static_cast<T>(96) });

			Matrix3x3<T> minus = a;
			minus -= b;
			AssertMatrix3x3Equal(minus, { static_cast<T>(-29), static_cast<T>(-32), static_cast<T>(-34),
				static_cast<T>(-32), static_cast<T>(-34), static_cast<T>(-36),
				static_cast<T>(-40), static_cast<T>(-38), static_cast<T>(-38) });

			Matrix3x3<T> multiplier;
			multiplier(1, 1) = static_cast<T>(2);
			multiplier(2, 2) = static_cast<T>(3);
			multiplier(3, 3) = static_cast<T>(5);

			Matrix3x3<T> times = a;
			times *= multiplier;
			AssertMatrix3x3Equal(times, { static_cast<T>(4), static_cast<T>(15), static_cast<T>(35),
				static_cast<T>(22), static_cast<T>(39), static_cast<T>(85),
				static_cast<T>(38), static_cast<T>(69), static_cast<T>(145) });

			Matrix3x3<T> scaled = a;
			scaled *= static_cast<T>(1.5);
			AssertMatrix3x3Equal(scaled, { static_cast<T>(3), static_cast<T>(7.5), static_cast<T>(10.5),
				static_cast<T>(16.5), static_cast<T>(19.5), static_cast<T>(25.5),
				static_cast<T>(28.5), static_cast<T>(34.5), static_cast<T>(43.5) });
		}

		template <typename T>
		void EqualityOperators_Work()
		{
			Matrix3x3<T> a{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7),
				static_cast<T>(11), static_cast<T>(13), static_cast<T>(17),
				static_cast<T>(19), static_cast<T>(23), static_cast<T>(29) };
			Matrix3x3<T> b = a;
			Matrix3x3<T> c{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7),
				static_cast<T>(11), static_cast<T>(13), static_cast<T>(17),
				static_cast<T>(19), static_cast<T>(23), static_cast<T>(31) };

			Assert::IsTrue(a == b);
			Assert::IsFalse(a != b);
			Assert::IsFalse(a == c);
			Assert::IsTrue(a != c);
		}

		template <typename T>
		void EdgeCases_IdentityZeroTransposeAndNegativeScalar_Work()
		{
			const Matrix3x3<T> m{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7),
				static_cast<T>(11), static_cast<T>(13), static_cast<T>(17),
				static_cast<T>(19), static_cast<T>(23), static_cast<T>(29) };
			const Matrix3x3<T> identity;
			const Matrix3x3<T> zero{ static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0), static_cast<T>(0) };

			const Matrix3x3<T> leftIdentity = identity * m;
			const Matrix3x3<T> rightIdentity = m * identity;
			Assert::IsTrue(leftIdentity == m);
			Assert::IsTrue(rightIdentity == m);

			const Matrix3x3<T> plusZero = m + zero;
			const Matrix3x3<T> minusZero = m - zero;
			Assert::IsTrue(plusZero == m);
			Assert::IsTrue(minusZero == m);

			const Matrix3x3<T> transposedTwice = m.GetTranspose().GetTranspose();
			Assert::IsTrue(transposedTwice == m);

			Matrix3x3<T> compound = m;
			compound *= identity;
			Assert::IsTrue(compound == m);
			compound += zero;
			Assert::IsTrue(compound == m);
			compound -= zero;
			Assert::IsTrue(compound == m);

			const Matrix3x3<T> leftNeg = static_cast<T>(-2) * m;
			const Matrix3x3<T> rightNeg = m * static_cast<T>(-2);
			AssertMatrix3x3Equal(leftNeg, { static_cast<T>(-4), static_cast<T>(-10), static_cast<T>(-14),
				static_cast<T>(-22), static_cast<T>(-26), static_cast<T>(-34),
				static_cast<T>(-38), static_cast<T>(-46), static_cast<T>(-58) });
			Assert::IsTrue(rightNeg == leftNeg);

			Matrix3x3<T> singleDiff = m;
			singleDiff(2, 2) += static_cast<T>(1);
			Assert::IsFalse(m == singleDiff);
			Assert::IsTrue(m != singleDiff);
		}

		template <typename T>
		void FreeFunctionOperators_Work()
		{
			Matrix3x3<T> a{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7),
				static_cast<T>(11), static_cast<T>(13), static_cast<T>(17),
				static_cast<T>(19), static_cast<T>(23), static_cast<T>(29) };
			Matrix3x3<T> b{ static_cast<T>(31), static_cast<T>(37), static_cast<T>(41),
				static_cast<T>(43), static_cast<T>(47), static_cast<T>(53),
				static_cast<T>(59), static_cast<T>(61), static_cast<T>(67) };

			const Matrix3x3<T> sum = a + b;
			const Matrix3x3<T> diff = a - b;
			const Matrix3x3<T> leftScaled = static_cast<T>(2) * a;
			const Matrix3x3<T> rightScaled = a * static_cast<T>(2);

			Matrix3x3<T> multiplier;
			multiplier(1, 1) = static_cast<T>(2);
			multiplier(2, 2) = static_cast<T>(3);
			multiplier(3, 3) = static_cast<T>(5);

			const Matrix3x3<T> product = a * multiplier;

			AssertMatrix3x3Equal(sum, { 
                static_cast<T>(33), static_cast<T>(42), static_cast<T>(48),
				static_cast<T>(54), static_cast<T>(60), static_cast<T>(70),
				static_cast<T>(78), static_cast<T>(84), static_cast<T>(96) });
			AssertMatrix3x3Equal(diff, { 
                static_cast<T>(-29), static_cast<T>(-32), static_cast<T>(-34),
				static_cast<T>(-32), static_cast<T>(-34), static_cast<T>(-36),
				static_cast<T>(-40), static_cast<T>(-38), static_cast<T>(-38) });
			AssertMatrix3x3Equal(leftScaled, { 
                static_cast<T>(4), static_cast<T>(10), static_cast<T>(14),
				static_cast<T>(22), static_cast<T>(26), static_cast<T>(34),
				static_cast<T>(38), static_cast<T>(46), static_cast<T>(58) });
			AssertMatrix3x3Equal(rightScaled, { 
                static_cast<T>(4), static_cast<T>(10), static_cast<T>(14),
				static_cast<T>(22), static_cast<T>(26), static_cast<T>(34),
				static_cast<T>(38), static_cast<T>(46), static_cast<T>(58) });
			AssertMatrix3x3Equal(product, { 
                static_cast<T>(4), static_cast<T>(15), static_cast<T>(35),
				static_cast<T>(22), static_cast<T>(39), static_cast<T>(85),
				static_cast<T>(38), static_cast<T>(69), static_cast<T>(145) });

			const Vector3<T> v(static_cast<T>(1), static_cast<T>(2), static_cast<T>(3));
			const Matrix3x3<T> vm{ static_cast<T>(1), static_cast<T>(2), static_cast<T>(3),
				static_cast<T>(4), static_cast<T>(5), static_cast<T>(6),
				static_cast<T>(7), static_cast<T>(8), static_cast<T>(9) };
			const Vector3<T> transformed = v * vm;
			AssertVector3Equal(transformed, static_cast<T>(30), static_cast<T>(36), static_cast<T>(42));
		}

		template <typename T>
		void StreamOperator_FormatsRows()
		{
			Matrix3x3<T> m{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7),
				static_cast<T>(11), static_cast<T>(13), static_cast<T>(17),
				static_cast<T>(19), static_cast<T>(23), static_cast<T>(29) };

			std::ostringstream oss;
			oss << m;

			const std::string s = oss.str();
			Assert::IsTrue(s.find("[2") != std::string::npos);
			Assert::IsTrue(s.find("29") != std::string::npos);
			Assert::IsTrue(s.find("\n") != std::string::npos);
		}
	}

	TEST_CLASS(Matrix3x3Tests)
	{
	public:
		TEST_METHOD(Constructor_Default_IsIdentity_ForFloatAndDouble)
		{
			Constructor_Default_IsIdentity<float>();
			Constructor_Default_IsIdentity<double>();
		}

		TEST_METHOD(Constructor_FromMatrix4x4_CopiesTopLeft3x3_ForFloatAndDouble)
		{
			Constructor_FromMatrix4x4_CopiesTopLeft3x3<float>();
			Constructor_FromMatrix4x4_CopiesTopLeft3x3<double>();
		}

		TEST_METHOD(Constructor_FillInitializerList_RowMajor_ForFloatAndDouble)
		{
			Constructor_FillInitializerList_RowMajor<float>();
			Constructor_FillInitializerList_RowMajor<double>();
		}

		TEST_METHOD(Constructor_FillInitializerList_WrongCount_Throws)
		{
			Assert::ExpectException<std::invalid_argument>([]()
			{
				Matrix3x3<float> m{ 1.0f, 2.0f, 3.0f };
				(void)m;
			});

			Assert::ExpectException<std::invalid_argument>([]()
			{
				Matrix3x3<double> m{ 1.0, 2.0, 3.0, 4.0 };
				(void)m;
			});
		}

		TEST_METHOD(Constructor_NestedInitializerList_RowMajor_ForFloatAndDouble)
		{
			Constructor_NestedInitializerList_RowMajor<float>();
			Constructor_NestedInitializerList_RowMajor<double>();
		}

		TEST_METHOD(Constructor_NestedInitializerList_InvalidShape_Throws)
		{
			Assert::ExpectException<std::invalid_argument>([]()
			{
				Matrix3x3<float> m{ {1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f} };
				(void)m;
			});

			Assert::ExpectException<std::invalid_argument>([]()
			{
				Matrix3x3<double> m{ {1.0, 2.0, 3.0}, {4.0, 5.0}, {6.0, 7.0, 8.0} };
				(void)m;
			});
		}

		TEST_METHOD(Constructor_CopyAndAssign_CopyAllElements_ForFloatAndDouble)
		{
			Constructor_CopyAndAssign_CopyAllElements<float>();
			Constructor_CopyAndAssign_CopyAllElements<double>();
		}

		TEST_METHOD(OperatorCall_OneIndexedReadWrite_AndConstOverload_Work_ForFloatAndDouble)
		{
			OperatorCall_OneIndexedReadWrite_AndConstOverload_Work<float>();
			OperatorCall_OneIndexedReadWrite_AndConstOverload_Work<double>();

			Matrix3x3<float> mf;
			Assert::ExpectException<std::out_of_range>([&mf]() { (void)mf(0, 1); });
			Assert::ExpectException<std::out_of_range>([&mf]() { (void)mf(4, 1); });
			Assert::ExpectException<std::out_of_range>([&mf]() { (void)mf(1, 0); });
			Assert::ExpectException<std::out_of_range>([&mf]() { (void)mf(1, 4); });

			const Matrix3x3<double> md;
			Assert::ExpectException<std::out_of_range>([&md]() { (void)md(0, 1); });
			Assert::ExpectException<std::out_of_range>([&md]() { (void)md(4, 1); });
			Assert::ExpectException<std::out_of_range>([&md]() { (void)md(1, 0); });
			Assert::ExpectException<std::out_of_range>([&md]() { (void)md(1, 4); });
		}

		TEST_METHOD(OperatorIndex_RowAccess_ReadWrite_Works_ForFloatAndDouble)
		{
			OperatorIndex_RowAccess_ReadWrite_Works<float>();
			OperatorIndex_RowAccess_ReadWrite_Works<double>();
		}

		TEST_METHOD(GetRow_GetColumn_GetTranspose_Work_ForFloatAndDouble)
		{
			GetRow_GetColumn_GetTranspose_Work<float>();
			GetRow_GetColumn_GetTranspose_Work<double>();
		}

		TEST_METHOD(CreateRotationAroundX_Y_Z_CreateExpectedMatrices_ForFloatAndDouble)
		{
			CreateRotationAroundX_Y_Z_CreateExpectedMatrices<float>();
			CreateRotationAroundX_Y_Z_CreateExpectedMatrices<double>();
		}

		TEST_METHOD(CompoundAssignmentOperators_Work_ForFloatAndDouble)
		{
			CompoundAssignmentOperators_Work<float>();
			CompoundAssignmentOperators_Work<double>();
		}

		TEST_METHOD(EqualityOperators_Work_ForFloatAndDouble)
		{
			EqualityOperators_Work<float>();
			EqualityOperators_Work<double>();
		}

		TEST_METHOD(EdgeCases_IdentityZeroTransposeAndNegativeScalar_Work_ForFloatAndDouble)
		{
			EdgeCases_IdentityZeroTransposeAndNegativeScalar_Work<float>();
			EdgeCases_IdentityZeroTransposeAndNegativeScalar_Work<double>();
		}

		TEST_METHOD(FreeFunctionOperators_Work_ForFloatAndDouble)
		{
			FreeFunctionOperators_Work<float>();
			FreeFunctionOperators_Work<double>();
		}

		TEST_METHOD(StreamOperator_FormatsRows_ForFloatAndDouble)
		{
			StreamOperator_FormatsRows<float>();
			StreamOperator_FormatsRows<double>();
		}
	};
}
