#pragma once

#include <cmath>
#include <initializer_list>
#include <ostream>
#include <stdexcept>

#include "Vector4.hpp"

namespace CommonUtilities
{
	template <typename T>
	class Matrix3x3;

	template <typename T>
	class Matrix4x4
	{
	public:
		// Creates the identity matrix.
		Matrix4x4();

		// Fill constructor (row-major): { 16 elements }
		Matrix4x4(std::initializer_list<T> aElements);

		// Nested list constructor (row-major): { {4}, {4}, {4}, {4} }
		Matrix4x4(std::initializer_list<std::initializer_list<T>> aRows);

		// Copies the 3x3 matrix into the top left area of the 4x4 matrix.
		Matrix4x4(const Matrix3x3<T>& aMatrix);
		
		// Copy Constructor.
		Matrix4x4(const Matrix4x4& aMatrix) = default;
		
		//Assignment operator (compiler generated)
		Matrix4x4& operator=(const Matrix4x4& aMatrix) = default;
		
		// () operator for accessing element (row, column) for read/write or read, respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;

		// Index access (row): allows m[row][col]
		T* operator[](const int aRow);
		const T* operator[](const int aRow) const;

		Vector4<T> GetRow(const int aRow) const;
		Vector4<T> GetColumn(const int aColumn) const;
		
		// Creates a transposed copy of the matrix.
		Matrix4x4 GetTranspose() const;

		// Assumes the matrix is made up of nothing but rotations and translations.
		Matrix4x4<T> GetFastInverse() const;

		// Static functions for creating rotation matrices.
		static Matrix4x4 CreateRotationAroundX(const T aAngleInRadians);
		static Matrix4x4 CreateRotationAroundY(const T aAngleInRadians);
		static Matrix4x4 CreateRotationAroundZ(const T aAngleInRadians);

		// Compound assignment operators
		Matrix4x4& operator+=(const Matrix4x4& aMatrix);
		Matrix4x4& operator-=(const Matrix4x4& aMatrix);
		Matrix4x4& operator*=(const Matrix4x4& aMatrix);
		Matrix4x4& operator*=(const T& aScalar);

		bool operator==(const Matrix4x4& aOther) const;
		bool operator!=(const Matrix4x4& aOther) const { return !(*this == aOther); }
	
	private:
		T myMatrix[4][4];
	};

	// Free function operators
	template <typename T>
	Matrix4x4<T> operator+(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1);

	template <typename T>
	Matrix4x4<T> operator-(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1);

	template <typename T>
	Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1);

	template <typename T>
	Matrix4x4<T> operator*(const T& aScalar, const Matrix4x4<T>& aMatrix);

	template <typename T>
	Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix, const T& aScalar);

	// Vector * Matrix (row-vector convention): v' = v * M
	template <typename T>
	Vector4<T> operator*(const Vector4<T>& aVector, const Matrix4x4<T>& aMatrix);

	template <typename T>
	std::ostream& operator<<(std::ostream& aStream, const Matrix4x4<T>& aMatrix);

	// --- Implementation ---

	template <typename T>
	inline Matrix4x4<T>::Matrix4x4()
	{
		for (int row = 0; row < 4; ++row)
		{
			for (int col = 0; col < 4; ++col)
			{
				myMatrix[row][col] = (row == col) ? static_cast<T>(1) : static_cast<T>(0);
			}
		}
	}

	template <typename T>
	inline Matrix4x4<T>::Matrix4x4(const Matrix3x3<T>& aMatrix)
	{
		for (int row = 0; row < 3; ++row)
		{
			for (int col = 0; col < 3; ++col)
			{
				myMatrix[row][col] = aMatrix(row, col);
			}
		}

		for (int i = 0; i < 4; ++i)
		{
			myMatrix[i][3] = (i == 3) ? static_cast<T>(1) : static_cast<T>(0);
			myMatrix[3][i] = (i == 3) ? static_cast<T>(1) : static_cast<T>(0);
		}
	}

	template <typename T>
	inline Matrix4x4<T>::Matrix4x4(std::initializer_list<T> aElements)
	{
		if (aElements.size() != 16)
		{
			throw std::invalid_argument("Matrix4x4 requires exactly 16 elements");
		}

		auto it = aElements.begin();
		for (int r = 0; r < 4; ++r)
		{
			for (int c = 0; c < 4; ++c)
			{
				myMatrix[r][c] = *it++;
			}
		}
	}

	template <typename T>
	inline Matrix4x4<T>::Matrix4x4(std::initializer_list<std::initializer_list<T>> aRows)
	{
		if (aRows.size() != 4)
		{
			throw std::invalid_argument("Matrix4x4 requires exactly 4 rows");
		}

		int r = 0;
		for (const auto& row : aRows)
		{
			if (row.size() != 4)
			{
				throw std::invalid_argument("Matrix4x4 requires exactly 4 columns per row");
			}

			int c = 0;
			for (const auto& value : row)
			{
				myMatrix[r][c++] = value;
			}
			++r;
		}
	}

	template <typename T>
	inline T& Matrix4x4<T>::operator()(const int aRow, const int aColumn)
	{
		return myMatrix[aRow][aColumn];
	}

	template <typename T>
	inline const T& Matrix4x4<T>::operator()(const int aRow, const int aColumn) const
	{
		return myMatrix[aRow][aColumn];
	}

	template <typename T>
	inline T* Matrix4x4<T>::operator[](const int aRow)
	{
		return myMatrix[aRow];
	}

	template <typename T>
	inline const T* Matrix4x4<T>::operator[](const int aRow) const
	{
		return myMatrix[aRow];
	}

	template <typename T>
	inline Vector4<T> Matrix4x4<T>::GetRow(const int aRow) const
	{
		return Vector4<T>(myMatrix[aRow][0], myMatrix[aRow][1], myMatrix[aRow][2], myMatrix[aRow][3]);
	}

	template <typename T>
	inline Vector4<T> Matrix4x4<T>::GetColumn(const int aColumn) const
	{
		return Vector4<T>(myMatrix[0][aColumn], myMatrix[1][aColumn], myMatrix[2][aColumn], myMatrix[3][aColumn]);
	}

	template <typename T>
	inline Matrix4x4<T> Matrix4x4<T>::GetTranspose() const
	{
		Matrix4x4<T> transposedMatrix;
		for (int row = 0; row < 4; ++row)
		{
			for (int col = 0; col < 4; ++col)
			{
				transposedMatrix(col, row) = myMatrix[row][col];
			}
		}
		return transposedMatrix;
	}

	template <typename T>
	inline Matrix4x4<T> Matrix4x4<T>::GetFastInverse() const
	{
		// Assumes row-vector convention: v' = v * M
		// For rigid transforms: M = [ R  0 ]
		//                     [ t  1 ]
		// Inverse is:          [ R^T 0 ]
		//                     [ -t R^T 1 ]
		Matrix4x4<T> inverseMatrix;

		// Transpose the rotation part (top-left 3x3)
		for (int row = 0; row < 3; ++row)
		{
			for (int col = 0; col < 3; ++col)
			{
				inverseMatrix(row, col) = (*this)(col, row);
			}
		}

		// Compute inverse translation row: -t * R^T
		const T tx = (*this)(3, 0);
		const T ty = (*this)(3, 1);
		const T tz = (*this)(3, 2);
		inverseMatrix(3, 0) = -(tx * inverseMatrix(0, 0) + ty * inverseMatrix(1, 0) + tz * inverseMatrix(2, 0));
		inverseMatrix(3, 1) = -(tx * inverseMatrix(0, 1) + ty * inverseMatrix(1, 1) + tz * inverseMatrix(2, 1));
		inverseMatrix(3, 2) = -(tx * inverseMatrix(0, 2) + ty * inverseMatrix(1, 2) + tz * inverseMatrix(2, 2));

		// Last column/row are already set correctly by identity constructor.
		return inverseMatrix;
	}
	
	template <typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(const T aAngleInRadians)
	{
		Matrix4x4<T> rotationMatrix;
		const T cosAngle = static_cast<T>(std::cos(aAngleInRadians));
		const T sinAngle = static_cast<T>(std::sin(aAngleInRadians));
		rotationMatrix(1, 1) = cosAngle;
		rotationMatrix(1, 2) = sinAngle;
		rotationMatrix(2, 1) = -sinAngle;
		rotationMatrix(2, 2) = cosAngle;
		return rotationMatrix;
	}

	template <typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(const T aAngleInRadians)
	{
		Matrix4x4<T> rotationMatrix;
		const T cosAngle = static_cast<T>(std::cos(aAngleInRadians));
		const T sinAngle = static_cast<T>(std::sin(aAngleInRadians));
		rotationMatrix(0, 0) = cosAngle;
		rotationMatrix(0, 2) = -sinAngle;
		rotationMatrix(2, 0) = sinAngle;
		rotationMatrix(2, 2) = cosAngle;
		return rotationMatrix;
	}

	template <typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(const T aAngleInRadians)
	{
		Matrix4x4<T> rotationMatrix;
		const T cosAngle = static_cast<T>(std::cos(aAngleInRadians));
		const T sinAngle = static_cast<T>(std::sin(aAngleInRadians));
		rotationMatrix(0, 0) = cosAngle;
		rotationMatrix(0, 1) = sinAngle;
		rotationMatrix(1, 0) = -sinAngle;
		rotationMatrix(1, 1) = cosAngle;
		return rotationMatrix;
	}

	// Compound operators
	template <typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator+=(const Matrix4x4<T>& aMatrix)
	{
		for (int r = 0; r < 4; ++r)
		{
			for (int c = 0; c < 4; ++c)
			{
				myMatrix[r][c] += aMatrix.myMatrix[r][c];
			}
		}
		return *this;
	}

	template <typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator-=(const Matrix4x4<T>& aMatrix)
	{
		for (int r = 0; r < 4; ++r)
		{
			for (int c = 0; c < 4; ++c)
			{
				myMatrix[r][c] -= aMatrix.myMatrix[r][c];
			}
		}
		return *this;
	}

	template <typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator*=(const Matrix4x4<T>& aMatrix)
	{
		Matrix4x4<T> result;
		for (int r = 0; r < 4; ++r)
		{
			for (int c = 0; c < 4; ++c)
			{
				T sum = static_cast<T>(0);
				for (int k = 0; k < 4; ++k)
				{
					sum += myMatrix[r][k] * aMatrix.myMatrix[k][c];
				}
				result.myMatrix[r][c] = sum;
			}
		}
		*this = result;
		return *this;
	}

	template <typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator*=(const T& aScalar)
	{
		for (int r = 0; r < 4; ++r)
		{
			for (int c = 0; c < 4; ++c)
			{
				myMatrix[r][c] *= aScalar;
			}
		}
		return *this;
	}

	template <typename T>
	inline bool Matrix4x4<T>::operator==(const Matrix4x4<T>& aOther) const
	{
		for (int r = 0; r < 4; ++r)
		{
			for (int c = 0; c < 4; ++c)
			{
				if (myMatrix[r][c] != aOther.myMatrix[r][c])
				{
					return false;
				}
			}
		}
		return true;
	}

	// Free function implementations
	template <typename T>
	inline Matrix4x4<T> operator+(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> result = aMatrix0;
		result += aMatrix1;
		return result;
	}

	template <typename T>
	inline Matrix4x4<T> operator-(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> result = aMatrix0;
		result -= aMatrix1;
		return result;
	}

	template <typename T>
	inline Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		Matrix4x4<T> result;
		for (int r = 0; r < 4; ++r)
		{
			for (int c = 0; c < 4; ++c)
			{
				T sum = static_cast<T>(0);
				for (int k = 0; k < 4; ++k)
				{
					sum += aMatrix0(r, k) * aMatrix1(k, c);
				}
				result(r, c) = sum;
			}
		}
		return result;
	}

	template <typename T>
	inline Matrix4x4<T> operator*(const T& aScalar, const Matrix4x4<T>& aMatrix)
	{
		Matrix4x4<T> result = aMatrix;
		result *= aScalar;
		return result;
	}

	template <typename T>
	inline Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix, const T& aScalar)
	{
		Matrix4x4<T> result = aMatrix;
		result *= aScalar;
		return result;
	}

	template <typename T>
	inline Vector4<T> operator*(const Vector4<T>& aVector, const Matrix4x4<T>& aMatrix)
	{
		return Vector4<T>(
			aVector.x * aMatrix(0, 0) + aVector.y * aMatrix(1, 0) + aVector.z * aMatrix(2, 0) + aVector.w * aMatrix(3, 0),
			aVector.x * aMatrix(0, 1) + aVector.y * aMatrix(1, 1) + aVector.z * aMatrix(2, 1) + aVector.w * aMatrix(3, 1),
			aVector.x * aMatrix(0, 2) + aVector.y * aMatrix(1, 2) + aVector.z * aMatrix(2, 2) + aVector.w * aMatrix(3, 2),
			aVector.x * aMatrix(0, 3) + aVector.y * aMatrix(1, 3) + aVector.z * aMatrix(2, 3) + aVector.w * aMatrix(3, 3));
	}

	template <typename T>
	inline std::ostream& operator<<(std::ostream& aStream, const Matrix4x4<T>& aMatrix)
	{
		for (int r = 0; r < 4; ++r)
		{
			aStream << "[";
			for (int c = 0; c < 4; ++c)
			{
				aStream << aMatrix(r, c);
				if (c < 3)
				{
					aStream << ", ";
				}
			}
			aStream << "]";
			if (r < 3)
			{
				aStream << "\n";
			}
		}
		return aStream;
	}
}