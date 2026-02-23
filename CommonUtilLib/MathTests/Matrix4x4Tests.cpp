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
using CommonUtilities::Vector4;

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
		void AssertVector4Equal(const Vector4<T>& v, const T x, const T y, const T z, const T w)
		{
			const T eps = GetEpsilon<T>();
			Assert::AreEqual(x, v.x, eps);
			Assert::AreEqual(y, v.y, eps);
			Assert::AreEqual(z, v.z, eps);
			Assert::AreEqual(w, v.w, eps);
		}

		template <typename T>
		void AssertMatrix4x4Equal(const Matrix4x4<T>& m, const std::array<T, 16>& expected)
		{
			const T eps = GetEpsilon<T>();
			for (int r = 1; r <= 4; ++r)
			{
				for (int c = 1; c <= 4; ++c)
				{
					const int index = (r - 1) * 4 + (c - 1);
					Assert::AreEqual(expected[index], m(r, c), eps);
				}
			}
		}

		template <typename T>
		void Constructor_Default_IsIdentity()
		{
			Matrix4x4<T> m;
			AssertMatrix4x4Equal(m, { static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) });
		}

		template <typename T>
		void Constructor_FromMatrix3x3_CopiesAndSetsHomogeneous()
		{
			Matrix3x3<T> m3{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7),
				static_cast<T>(11), static_cast<T>(13), static_cast<T>(17),
				static_cast<T>(19), static_cast<T>(23), static_cast<T>(29) };

			Matrix4x4<T> m4(m3);
			AssertMatrix4x4Equal(m4, { static_cast<T>(2), static_cast<T>(5), static_cast<T>(7), static_cast<T>(0),
				static_cast<T>(11), static_cast<T>(13), static_cast<T>(17), static_cast<T>(0),
				static_cast<T>(19), static_cast<T>(23), static_cast<T>(29), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) });
		}

		template <typename T>
		void Constructor_FillInitializerList_RowMajor()
		{
			Matrix4x4<T> m{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7), static_cast<T>(11),
				static_cast<T>(13), static_cast<T>(17), static_cast<T>(19), static_cast<T>(23),
				static_cast<T>(29), static_cast<T>(31), static_cast<T>(37), static_cast<T>(41),
				static_cast<T>(43), static_cast<T>(47), static_cast<T>(53), static_cast<T>(59) };

			AssertMatrix4x4Equal(m, { static_cast<T>(2), static_cast<T>(5), static_cast<T>(7), static_cast<T>(11),
				static_cast<T>(13), static_cast<T>(17), static_cast<T>(19), static_cast<T>(23),
				static_cast<T>(29), static_cast<T>(31), static_cast<T>(37), static_cast<T>(41),
				static_cast<T>(43), static_cast<T>(47), static_cast<T>(53), static_cast<T>(59) });
		}

		template <typename T>
		void Constructor_NestedInitializerList_RowMajor()
		{
			Matrix4x4<T> m{ { static_cast<T>(2), static_cast<T>(5), static_cast<T>(7), static_cast<T>(11) },
				{ static_cast<T>(13), static_cast<T>(17), static_cast<T>(19), static_cast<T>(23) },
				{ static_cast<T>(29), static_cast<T>(31), static_cast<T>(37), static_cast<T>(41) },
				{ static_cast<T>(43), static_cast<T>(47), static_cast<T>(53), static_cast<T>(59) } };

			AssertMatrix4x4Equal(m, { static_cast<T>(2), static_cast<T>(5), static_cast<T>(7), static_cast<T>(11),
				static_cast<T>(13), static_cast<T>(17), static_cast<T>(19), static_cast<T>(23),
				static_cast<T>(29), static_cast<T>(31), static_cast<T>(37), static_cast<T>(41),
				static_cast<T>(43), static_cast<T>(47), static_cast<T>(53), static_cast<T>(59) });
		}

		template <typename T>
		void Constructor_CopyAndAssign_CopyAllElements()
		{
			Matrix4x4<T> source{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7), static_cast<T>(11),
				static_cast<T>(13), static_cast<T>(17), static_cast<T>(19), static_cast<T>(23),
				static_cast<T>(29), static_cast<T>(31), static_cast<T>(37), static_cast<T>(41),
				static_cast<T>(43), static_cast<T>(47), static_cast<T>(53), static_cast<T>(59) };

			Matrix4x4<T> copy(source);
			Matrix4x4<T> assigned;
			assigned = source;

			AssertMatrix4x4Equal(copy, { static_cast<T>(2), static_cast<T>(5), static_cast<T>(7), static_cast<T>(11),
				static_cast<T>(13), static_cast<T>(17), static_cast<T>(19), static_cast<T>(23),
				static_cast<T>(29), static_cast<T>(31), static_cast<T>(37), static_cast<T>(41),
				static_cast<T>(43), static_cast<T>(47), static_cast<T>(53), static_cast<T>(59) });
			AssertMatrix4x4Equal(assigned, { static_cast<T>(2), static_cast<T>(5), static_cast<T>(7), static_cast<T>(11),
				static_cast<T>(13), static_cast<T>(17), static_cast<T>(19), static_cast<T>(23),
				static_cast<T>(29), static_cast<T>(31), static_cast<T>(37), static_cast<T>(41),
				static_cast<T>(43), static_cast<T>(47), static_cast<T>(53), static_cast<T>(59) });
		}

		template <typename T>
		void OperatorCall_OneIndexedReadWrite_AndConstOverload_Work()
		{
			Matrix4x4<T> m;
			m(1, 4) = static_cast<T>(2.5);
			m(4, 2) = static_cast<T>(-4.75);

			Assert::AreEqual(static_cast<T>(2.5), m(1, 4), GetEpsilon<T>());
			Assert::AreEqual(static_cast<T>(-4.75), m(4, 2), GetEpsilon<T>());

			const Matrix4x4<T>& cm = m;
			Assert::AreEqual(static_cast<T>(2.5), cm(1, 4), GetEpsilon<T>());
			Assert::AreEqual(static_cast<T>(-4.75), cm(4, 2), GetEpsilon<T>());
		}

		template <typename T>
		void OperatorIndex_RowAccess_ReadWrite_Works()
		{
			Matrix4x4<T> m;
			m[2][1] = static_cast<T>(9.25);
			m[0][3] = static_cast<T>(-2.0);

			const Matrix4x4<T>& cm = m;
			Assert::AreEqual(static_cast<T>(9.25), cm[2][1], GetEpsilon<T>());
			Assert::AreEqual(static_cast<T>(-2.0), cm[0][3], GetEpsilon<T>());
		}

		template <typename T>
		void GetRow_GetColumn_GetTranspose_Work()
		{
			Matrix4x4<T> m{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7), static_cast<T>(11),
				static_cast<T>(13), static_cast<T>(17), static_cast<T>(19), static_cast<T>(23),
				static_cast<T>(29), static_cast<T>(31), static_cast<T>(37), static_cast<T>(41),
				static_cast<T>(43), static_cast<T>(47), static_cast<T>(53), static_cast<T>(59) };

			const Vector4<T> r0 = m.GetRow(0);
			const Vector4<T> r1 = m.GetRow(1);
			const Vector4<T> r2 = m.GetRow(2);
			const Vector4<T> r3 = m.GetRow(3);
			const Vector4<T> c0 = m.GetColumn(0);
			const Vector4<T> c1 = m.GetColumn(1);
			const Vector4<T> c2 = m.GetColumn(2);
			const Vector4<T> c3 = m.GetColumn(3);
			const Matrix4x4<T> t = m.GetTranspose();

			AssertVector4Equal(r0, static_cast<T>(2), static_cast<T>(5), static_cast<T>(7), static_cast<T>(11));
			AssertVector4Equal(r1, static_cast<T>(13), static_cast<T>(17), static_cast<T>(19), static_cast<T>(23));
			AssertVector4Equal(r2, static_cast<T>(29), static_cast<T>(31), static_cast<T>(37), static_cast<T>(41));
			AssertVector4Equal(r3, static_cast<T>(43), static_cast<T>(47), static_cast<T>(53), static_cast<T>(59));

			AssertVector4Equal(c0, static_cast<T>(2), static_cast<T>(13), static_cast<T>(29), static_cast<T>(43));
			AssertVector4Equal(c1, static_cast<T>(5), static_cast<T>(17), static_cast<T>(31), static_cast<T>(47));
			AssertVector4Equal(c2, static_cast<T>(7), static_cast<T>(19), static_cast<T>(37), static_cast<T>(53));
			AssertVector4Equal(c3, static_cast<T>(11), static_cast<T>(23), static_cast<T>(41), static_cast<T>(59));

			AssertMatrix4x4Equal(t, { static_cast<T>(2), static_cast<T>(13), static_cast<T>(29), static_cast<T>(43),
				static_cast<T>(5), static_cast<T>(17), static_cast<T>(31), static_cast<T>(47),
				static_cast<T>(7), static_cast<T>(19), static_cast<T>(37), static_cast<T>(53),
				static_cast<T>(11), static_cast<T>(23), static_cast<T>(41), static_cast<T>(59) });
		}

		template <typename T>
		void GetFastInverse_RigidTransform_Inverts()
		{
			constexpr T pi = static_cast<T>(3.14159265358979323846);
			const T angle = pi / static_cast<T>(4);
			Matrix4x4<T> m = Matrix4x4<T>::CreateRotationAroundZ(angle);
			m(4, 1) = static_cast<T>(3);
			m(4, 2) = static_cast<T>(-4);
			m(4, 3) = static_cast<T>(2);

			const Matrix4x4<T> inv = m.GetFastInverse();
			const Matrix4x4<T> shouldBeIdentity1 = m * inv;
			const Matrix4x4<T> shouldBeIdentity2 = inv * m;

			AssertMatrix4x4Equal(shouldBeIdentity1, { static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) });
			AssertMatrix4x4Equal(shouldBeIdentity2, { static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) });
		}

		template <typename T>
		void CreateRotationAroundX_Y_Z_CreateExpectedMatrices()
		{
			constexpr T pi = static_cast<T>(3.14159265358979323846);
			const T ax = pi / static_cast<T>(4);
			const T ay = pi / static_cast<T>(3);
			const T az = -pi / static_cast<T>(6);

			const T cx = static_cast<T>(std::cos(ax));
			const T sx = static_cast<T>(std::sin(ax));
			const T cy = static_cast<T>(std::cos(ay));
			const T sy = static_cast<T>(std::sin(ay));
			const T cz = static_cast<T>(std::cos(az));
			const T sz = static_cast<T>(std::sin(az));

			const Matrix4x4<T> rx = Matrix4x4<T>::CreateRotationAroundX(ax);
			const Matrix4x4<T> ry = Matrix4x4<T>::CreateRotationAroundY(ay);
			const Matrix4x4<T> rz = Matrix4x4<T>::CreateRotationAroundZ(az);

			AssertMatrix4x4Equal(rx, { static_cast<T>(1), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
				static_cast<T>(0), cx, sx, static_cast<T>(0),
				static_cast<T>(0), -sx, cx, static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) });
			AssertMatrix4x4Equal(ry, { cy, static_cast<T>(0), -sy, static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
				sy, static_cast<T>(0), cy, static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) });
			AssertMatrix4x4Equal(rz, { cz, sz, static_cast<T>(0), static_cast<T>(0),
				-sz, cz, static_cast<T>(0), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) });
		}

		template <typename T>
		void CompoundAssignmentOperators_Work()
		{
			Matrix4x4<T> a{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7), static_cast<T>(11),
				static_cast<T>(13), static_cast<T>(17), static_cast<T>(19), static_cast<T>(23),
				static_cast<T>(29), static_cast<T>(31), static_cast<T>(37), static_cast<T>(41),
				static_cast<T>(43), static_cast<T>(47), static_cast<T>(53), static_cast<T>(59) };
			Matrix4x4<T> b{ static_cast<T>(61), static_cast<T>(67), static_cast<T>(71), static_cast<T>(73),
				static_cast<T>(79), static_cast<T>(83), static_cast<T>(89), static_cast<T>(97),
				static_cast<T>(101), static_cast<T>(103), static_cast<T>(107), static_cast<T>(109),
				static_cast<T>(113), static_cast<T>(127), static_cast<T>(131), static_cast<T>(137) };

			Matrix4x4<T> plus = a;
			plus += b;
			AssertMatrix4x4Equal(plus, { static_cast<T>(63), static_cast<T>(72), static_cast<T>(78), static_cast<T>(84),
				static_cast<T>(92), static_cast<T>(100), static_cast<T>(108), static_cast<T>(120),
				static_cast<T>(130), static_cast<T>(134), static_cast<T>(144), static_cast<T>(150),
				static_cast<T>(156), static_cast<T>(174), static_cast<T>(184), static_cast<T>(196) });

			Matrix4x4<T> minus = a;
			minus -= b;
			AssertMatrix4x4Equal(minus, { static_cast<T>(-59), static_cast<T>(-62), static_cast<T>(-64), static_cast<T>(-62),
				static_cast<T>(-66), static_cast<T>(-66), static_cast<T>(-70), static_cast<T>(-74),
				static_cast<T>(-72), static_cast<T>(-72), static_cast<T>(-70), static_cast<T>(-68),
				static_cast<T>(-70), static_cast<T>(-80), static_cast<T>(-78), static_cast<T>(-78) });

			Matrix4x4<T> multiplier;
			multiplier(1, 1) = static_cast<T>(2);
			multiplier(2, 2) = static_cast<T>(3);
			multiplier(3, 3) = static_cast<T>(5);
			multiplier(4, 4) = static_cast<T>(7);
			multiplier(1, 2) = static_cast<T>(0);
			multiplier(1, 3) = static_cast<T>(0);
			multiplier(1, 4) = static_cast<T>(0);
			multiplier(2, 1) = static_cast<T>(0);
			multiplier(2, 3) = static_cast<T>(0);
			multiplier(2, 4) = static_cast<T>(0);
			multiplier(3, 1) = static_cast<T>(0);
			multiplier(3, 2) = static_cast<T>(0);
			multiplier(3, 4) = static_cast<T>(0);
			multiplier(4, 1) = static_cast<T>(0);
			multiplier(4, 2) = static_cast<T>(0);
			multiplier(4, 3) = static_cast<T>(0);

			Matrix4x4<T> times = a;
			times *= multiplier;
			AssertMatrix4x4Equal(times, { static_cast<T>(4), static_cast<T>(15), static_cast<T>(35), static_cast<T>(77),
				static_cast<T>(26), static_cast<T>(51), static_cast<T>(95), static_cast<T>(161),
				static_cast<T>(58), static_cast<T>(93), static_cast<T>(185), static_cast<T>(287),
				static_cast<T>(86), static_cast<T>(141), static_cast<T>(265), static_cast<T>(413) });

			Matrix4x4<T> scaled = a;
			scaled *= static_cast<T>(1.5);
			AssertMatrix4x4Equal(scaled, { static_cast<T>(3), static_cast<T>(7.5), static_cast<T>(10.5), static_cast<T>(16.5),
				static_cast<T>(19.5), static_cast<T>(25.5), static_cast<T>(28.5), static_cast<T>(34.5),
				static_cast<T>(43.5), static_cast<T>(46.5), static_cast<T>(55.5), static_cast<T>(61.5),
				static_cast<T>(64.5), static_cast<T>(70.5), static_cast<T>(79.5), static_cast<T>(88.5) });
		}

		template <typename T>
		void EqualityOperators_Work()
		{
			Matrix4x4<T> a{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7), static_cast<T>(11),
				static_cast<T>(13), static_cast<T>(17), static_cast<T>(19), static_cast<T>(23),
				static_cast<T>(29), static_cast<T>(31), static_cast<T>(37), static_cast<T>(41),
				static_cast<T>(43), static_cast<T>(47), static_cast<T>(53), static_cast<T>(59) };
			Matrix4x4<T> b = a;
			Matrix4x4<T> c{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7), static_cast<T>(11),
				static_cast<T>(13), static_cast<T>(17), static_cast<T>(19), static_cast<T>(23),
				static_cast<T>(29), static_cast<T>(31), static_cast<T>(37), static_cast<T>(41),
				static_cast<T>(43), static_cast<T>(47), static_cast<T>(53), static_cast<T>(61) };

			Assert::IsTrue(a == b);
			Assert::IsFalse(a != b);
			Assert::IsFalse(a == c);
			Assert::IsTrue(a != c);
		}

		template <typename T>
		void EdgeCases_IdentityZeroTransposeAndNegativeScalar_Work()
		{
			const Matrix4x4<T> m{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7), static_cast<T>(11),
				static_cast<T>(13), static_cast<T>(17), static_cast<T>(19), static_cast<T>(23),
				static_cast<T>(29), static_cast<T>(31), static_cast<T>(37), static_cast<T>(41),
				static_cast<T>(43), static_cast<T>(47), static_cast<T>(53), static_cast<T>(59) };
			const Matrix4x4<T> identity;
			const Matrix4x4<T> zero{ static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0),
				static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0) };

			const Matrix4x4<T> leftIdentity = identity * m;
			const Matrix4x4<T> rightIdentity = m * identity;
			Assert::IsTrue(leftIdentity == m);
			Assert::IsTrue(rightIdentity == m);

			const Matrix4x4<T> plusZero = m + zero;
			const Matrix4x4<T> minusZero = m - zero;
			Assert::IsTrue(plusZero == m);
			Assert::IsTrue(minusZero == m);

			const Matrix4x4<T> transposedTwice = m.GetTranspose().GetTranspose();
			Assert::IsTrue(transposedTwice == m);

			Matrix4x4<T> compound = m;
			compound *= identity;
			Assert::IsTrue(compound == m);
			compound += zero;
			Assert::IsTrue(compound == m);
			compound -= zero;
			Assert::IsTrue(compound == m);

			const Matrix4x4<T> leftNeg = static_cast<T>(-2) * m;
			const Matrix4x4<T> rightNeg = m * static_cast<T>(-2);
			AssertMatrix4x4Equal(leftNeg, { static_cast<T>(-4), static_cast<T>(-10), static_cast<T>(-14), static_cast<T>(-22),
				static_cast<T>(-26), static_cast<T>(-34), static_cast<T>(-38), static_cast<T>(-46),
				static_cast<T>(-58), static_cast<T>(-62), static_cast<T>(-74), static_cast<T>(-82),
				static_cast<T>(-86), static_cast<T>(-94), static_cast<T>(-106), static_cast<T>(-118) });
			Assert::IsTrue(rightNeg == leftNeg);

			Matrix4x4<T> singleDiff = m;
			singleDiff(2, 2) += static_cast<T>(1);
			Assert::IsFalse(m == singleDiff);
			Assert::IsTrue(m != singleDiff);
		}

		template <typename T>
		void FreeFunctionOperators_Work()
		{
			Matrix4x4<T> a{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7), static_cast<T>(11),
				static_cast<T>(13), static_cast<T>(17), static_cast<T>(19), static_cast<T>(23),
				static_cast<T>(29), static_cast<T>(31), static_cast<T>(37), static_cast<T>(41),
				static_cast<T>(43), static_cast<T>(47), static_cast<T>(53), static_cast<T>(59) };
			Matrix4x4<T> b{ static_cast<T>(61), static_cast<T>(67), static_cast<T>(71), static_cast<T>(73),
				static_cast<T>(79), static_cast<T>(83), static_cast<T>(89), static_cast<T>(97),
				static_cast<T>(101), static_cast<T>(103), static_cast<T>(107), static_cast<T>(109),
				static_cast<T>(113), static_cast<T>(127), static_cast<T>(131), static_cast<T>(137) };

			const Matrix4x4<T> sum = a + b;
			const Matrix4x4<T> diff = a - b;
			const Matrix4x4<T> leftScaled = static_cast<T>(2) * a;
			const Matrix4x4<T> rightScaled = a * static_cast<T>(2);

			Matrix4x4<T> multiplier;
			multiplier(1, 1) = static_cast<T>(2);
			multiplier(2, 2) = static_cast<T>(3);
			multiplier(3, 3) = static_cast<T>(5);
			multiplier(4, 4) = static_cast<T>(7);
			multiplier(1, 2) = static_cast<T>(0);
			multiplier(1, 3) = static_cast<T>(0);
			multiplier(1, 4) = static_cast<T>(0);
			multiplier(2, 1) = static_cast<T>(0);
			multiplier(2, 3) = static_cast<T>(0);
			multiplier(2, 4) = static_cast<T>(0);
			multiplier(3, 1) = static_cast<T>(0);
			multiplier(3, 2) = static_cast<T>(0);
			multiplier(3, 4) = static_cast<T>(0);
			multiplier(4, 1) = static_cast<T>(0);
			multiplier(4, 2) = static_cast<T>(0);
			multiplier(4, 3) = static_cast<T>(0);

			const Matrix4x4<T> product = a * multiplier;

			AssertMatrix4x4Equal(sum, { static_cast<T>(63), static_cast<T>(72), static_cast<T>(78), static_cast<T>(84),
				static_cast<T>(92), static_cast<T>(100), static_cast<T>(108), static_cast<T>(120),
				static_cast<T>(130), static_cast<T>(134), static_cast<T>(144), static_cast<T>(150),
				static_cast<T>(156), static_cast<T>(174), static_cast<T>(184), static_cast<T>(196) });
			AssertMatrix4x4Equal(diff, { static_cast<T>(-59), static_cast<T>(-62), static_cast<T>(-64), static_cast<T>(-62),
				static_cast<T>(-66), static_cast<T>(-66), static_cast<T>(-70), static_cast<T>(-74),
				static_cast<T>(-72), static_cast<T>(-72), static_cast<T>(-70), static_cast<T>(-68),
				static_cast<T>(-70), static_cast<T>(-80), static_cast<T>(-78), static_cast<T>(-78) });
			AssertMatrix4x4Equal(leftScaled, { static_cast<T>(4), static_cast<T>(10), static_cast<T>(14), static_cast<T>(22),
				static_cast<T>(26), static_cast<T>(34), static_cast<T>(38), static_cast<T>(46),
				static_cast<T>(58), static_cast<T>(62), static_cast<T>(74), static_cast<T>(82),
				static_cast<T>(86), static_cast<T>(94), static_cast<T>(106), static_cast<T>(118) });
			AssertMatrix4x4Equal(rightScaled, { static_cast<T>(4), static_cast<T>(10), static_cast<T>(14), static_cast<T>(22),
				static_cast<T>(26), static_cast<T>(34), static_cast<T>(38), static_cast<T>(46),
				static_cast<T>(58), static_cast<T>(62), static_cast<T>(74), static_cast<T>(82),
				static_cast<T>(86), static_cast<T>(94), static_cast<T>(106), static_cast<T>(118) });
			AssertMatrix4x4Equal(product, { static_cast<T>(4), static_cast<T>(15), static_cast<T>(35), static_cast<T>(77),
				static_cast<T>(26), static_cast<T>(51), static_cast<T>(95), static_cast<T>(161),
				static_cast<T>(58), static_cast<T>(93), static_cast<T>(185), static_cast<T>(287),
				static_cast<T>(86), static_cast<T>(141), static_cast<T>(265), static_cast<T>(413) });

			const Matrix4x4<T> vm;
			const Vector4<T> v(static_cast<T>(1), static_cast<T>(2), static_cast<T>(3), static_cast<T>(1));
			const Vector4<T> transformed = v * vm;
			AssertVector4Equal(transformed, static_cast<T>(1), static_cast<T>(2), static_cast<T>(3), static_cast<T>(1));
		}

		template <typename T>
		void StreamOperator_FormatsRows()
		{
			Matrix4x4<T> m{ static_cast<T>(2), static_cast<T>(5), static_cast<T>(7), static_cast<T>(11),
				static_cast<T>(13), static_cast<T>(17), static_cast<T>(19), static_cast<T>(23),
				static_cast<T>(29), static_cast<T>(31), static_cast<T>(37), static_cast<T>(41),
				static_cast<T>(43), static_cast<T>(47), static_cast<T>(53), static_cast<T>(59) };

			std::ostringstream oss;
			oss << m;

			const std::string s = oss.str();
			Assert::IsTrue(s.find("[2") != std::string::npos);
			Assert::IsTrue(s.find("59") != std::string::npos);
			Assert::IsTrue(s.find("\n") != std::string::npos);
		}
	}

	TEST_CLASS(Matrix4x4Tests)
	{
	public:
		TEST_METHOD(Constructor_Default_IsIdentity_ForFloatAndDouble)
		{
			Constructor_Default_IsIdentity<float>();
			Constructor_Default_IsIdentity<double>();
		}

		TEST_METHOD(Constructor_FromMatrix3x3_CopiesAndSetsHomogeneous_ForFloatAndDouble)
		{
			Constructor_FromMatrix3x3_CopiesAndSetsHomogeneous<float>();
			Constructor_FromMatrix3x3_CopiesAndSetsHomogeneous<double>();
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
				Matrix4x4<float> m{ 1.0f, 2.0f, 3.0f };
				(void)m;
			});

			Assert::ExpectException<std::invalid_argument>([]()
			{
				Matrix4x4<double> m{ 1.0, 2.0, 3.0, 4.0 };
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
				Matrix4x4<float> m{ {1.0f, 2.0f, 3.0f, 4.0f}, {5.0f, 6.0f, 7.0f, 8.0f}, {9.0f, 10.0f, 11.0f, 12.0f} };
				(void)m;
			});

			Assert::ExpectException<std::invalid_argument>([]()
			{
				Matrix4x4<double> m{ {1.0, 2.0, 3.0, 4.0}, {5.0, 6.0, 7.0}, {8.0, 9.0, 10.0, 11.0}, {12.0, 13.0, 14.0, 15.0} };
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

			Matrix4x4<float> mf;
			Assert::ExpectException<std::out_of_range>([&mf]() { (void)mf(0, 1); });
			Assert::ExpectException<std::out_of_range>([&mf]() { (void)mf(5, 1); });
			Assert::ExpectException<std::out_of_range>([&mf]() { (void)mf(1, 0); });
			Assert::ExpectException<std::out_of_range>([&mf]() { (void)mf(1, 5); });

			const Matrix4x4<double> md;
			Assert::ExpectException<std::out_of_range>([&md]() { (void)md(0, 1); });
			Assert::ExpectException<std::out_of_range>([&md]() { (void)md(5, 1); });
			Assert::ExpectException<std::out_of_range>([&md]() { (void)md(1, 0); });
			Assert::ExpectException<std::out_of_range>([&md]() { (void)md(1, 5); });
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

		TEST_METHOD(GetFastInverse_RigidTransform_Inverts_ForFloatAndDouble)
		{
			GetFastInverse_RigidTransform_Inverts<float>();
			GetFastInverse_RigidTransform_Inverts<double>();
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
