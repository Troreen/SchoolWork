#pragma once 

#include <cmath>

namespace CommonUtilities
{
	template <typename T>
	class Matrix4x4;
	
	template <typename T>
	class Matrix3x3
	{
	public:
		// Creates the identity matrix.
		Matrix3x3<T>();

		// Copy Constructor.
		Matrix3x3<T>(const Matrix3x3<T>& aMatrix) = default;
		
		//Assignment operator (compiler generated)
		Matrix3x3<T>& operator=(const Matrix3x3<T>& aMatrix) = default;
		
		// Copies the top left 3x3 part of the Matrix4x4.
		Matrix3x3<T>(const Matrix4x4<T>& aMatrix);
		
		// () operator for accessing element (row, column) for read/write or read, respectively. 
		T& operator()(const int aRow, const int aColumn);
		
		const T& operator()(const int aRow, const int aColumn) const;
		
		// Creates a transposed copy of the matrix.
		Matrix3x3<T> GetTranspose() const;
		
		// Static functions for creating rotation matrices.
		static Matrix3x3<T> CreateRotationAroundX(const T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundY(const T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundZ(const T aAngleInRadians);

		// Compound assignment operators
		Matrix3x3<T>& operator+=(const Matrix3x3<T>& aMatrix);
		Matrix3x3<T>& operator-=(const Matrix3x3<T>& aMatrix);
		Matrix3x3<T>& operator*=(const Matrix3x3<T>& aMatrix);
		Matrix3x3<T>& operator*=(const T& aScalar);

		bool operator==(const Matrix3x3<T>& aOther) const;
		bool operator!=(const Matrix3x3<T>& aOther) const { return !(*this == aOther); }

	private:
		T myMatrix[3][3];
	};	 

	// Free function operators
	template <typename T>
	Matrix3x3<T> operator+(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1);

	template <typename T>
	Matrix3x3<T> operator-(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1);

	template <typename T>
	Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1);

	template <typename T>
	Matrix3x3<T> operator*(const T& aScalar, const Matrix3x3<T>& aMatrix);
	
	template <typename T>
	Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix, const T& aScalar);

	// --- Implementation ---

	template <typename T>
	inline Matrix3x3<T>::Matrix3x3()
	{
		for (int row = 0; row < 3; ++row)
		{
			for (int col = 0; col < 3; ++col)
			{
				myMatrix[row][col] = (row == col) ? static_cast<T>(1) : static_cast<T>(0);
			}
		}
	}

	template <typename T>
	inline Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix)
	{
		for (int row = 0; row < 3; ++row)
		{
			for (int col = 0; col < 3; ++col)
			{
				myMatrix[row][col] = aMatrix(row, col);
			}
		}
	}

	template <typename T>
	inline T& Matrix3x3<T>::operator()(const int aRow, const int aColumn)
	{
		return myMatrix[aRow][aColumn];
	}

	template <typename T>
	inline const T& Matrix3x3<T>::operator()(const int aRow, const int aColumn) const
	{
		return myMatrix[aRow][aColumn];
	}
	
	template <typename T>
	inline Matrix3x3<T> Matrix3x3<T>::GetTranspose() const
	{
		Matrix3x3<T> transposedMatrix;
		for (int row = 0; row < 3; ++row)
		{
			for (int col = 0; col < 3; ++col)
			{
				transposedMatrix(col, row) = myMatrix[row][col];
			}
		}
		return transposedMatrix;
	}

	template <typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundX(const T aAngleInRadians)
	{
		Matrix3x3<T> rotationMatrix;
		T cosAngle = static_cast<T>(std::cos(aAngleInRadians));
		T sinAngle = static_cast<T>(std::sin(aAngleInRadians));
		rotationMatrix(1, 1) = cosAngle;
		rotationMatrix(1, 2) = sinAngle;
		rotationMatrix(2, 1) = -sinAngle;
		rotationMatrix(2, 2) = cosAngle;
		return rotationMatrix;
	}

	template <typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundY(const T aAngleInRadians)
	{
		Matrix3x3<T> rotationMatrix;
		T cosAngle = static_cast<T>(std::cos(aAngleInRadians));
		T sinAngle = static_cast<T>(std::sin(aAngleInRadians));
		rotationMatrix(0, 0) = cosAngle;
		rotationMatrix(0, 2) = -sinAngle;
		rotationMatrix(2, 0) = sinAngle;
		rotationMatrix(2, 2) = cosAngle;
		return rotationMatrix;
	}

	template <typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundZ(const T aAngleInRadians)
	{
		Matrix3x3<T> rotationMatrix;
		T cosAngle = static_cast<T>(std::cos(aAngleInRadians));
		T sinAngle = static_cast<T>(std::sin(aAngleInRadians));
		rotationMatrix(0, 0) = cosAngle;
		rotationMatrix(0, 1) = sinAngle;
		rotationMatrix(1, 0) = -sinAngle;
		rotationMatrix(1, 1) = cosAngle;
		return rotationMatrix;
	}

	// Compound operators
	template <typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator+=(const Matrix3x3<T>& aMatrix)
	{
		for (int r = 0; r < 3; ++r)
		{
			for (int c = 0; c < 3; ++c)
			{
				myMatrix[r][c] += aMatrix.myMatrix[r][c];
			}
		}
		return *this;
	}

	template <typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator-=(const Matrix3x3<T>& aMatrix)
	{
		for (int r = 0; r < 3; ++r)
		{
			for (int c = 0; c < 3; ++c)
			{
				myMatrix[r][c] -= aMatrix.myMatrix[r][c];
			}
		}
		return *this;
	}

	template <typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator*=(const Matrix3x3<T>& aMatrix)
	{
		Matrix3x3<T> result;
		for (int r = 0; r < 3; ++r)
		{
			for (int c = 0; c < 3; ++c)
			{
				T sum = static_cast<T>(0);
				for (int k = 0; k < 3; ++k)
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
	inline Matrix3x3<T>& Matrix3x3<T>::operator*=(const T& aScalar)
	{
		for (int r = 0; r < 3; ++r)
		{
			for (int c = 0; c < 3; ++c)
			{
				myMatrix[r][c] *= aScalar;
			}
		}
		return *this;
	}

	template <typename T>
	inline bool Matrix3x3<T>::operator==(const Matrix3x3<T>& aOther) const
	{
		for (int r = 0; r < 3; ++r)
		{
			for (int c = 0; c < 3; ++c)
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
	inline Matrix3x3<T> operator+(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> result = aMatrix0;
		result += aMatrix1;
		return result;
	}

	template <typename T>
	inline Matrix3x3<T> operator-(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> result = aMatrix0;
		result -= aMatrix1;
		return result;
	}

	template <typename T>
	inline Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> result;
		for (int r = 0; r < 3; ++r)
		{
			for (int c = 0; c < 3; ++c)
			{
				T sum = static_cast<T>(0);
				for (int k = 0; k < 3; ++k)
				{
					sum += aMatrix0(r, k) * aMatrix1(k, c);
				}
				result(r, c) = sum;
			}
		}
		return result;
	}

	template <typename T>
	inline Matrix3x3<T> operator*(const T& aScalar, const Matrix3x3<T>& aMatrix)
	{
		Matrix3x3<T> result = aMatrix;
		result *= aScalar;
		return result;
	}
	
	template <typename T>
	inline Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix, const T& aScalar)
	{
		Matrix3x3<T> result = aMatrix;
		result *= aScalar;
		return result;
	}

}