#pragma once

namespace CommonUtilities
{
	template <typename T>
	class Matrix3x3;

	template <typename T>
	class Matrix4x4
	{
	public:
		// Creates the identity matrix.
		Matrix4x4<T>();

		// Copies the 3x3 matrix into the top left area of the 4x4 matrix.
		Matrix4x4<T>(const Matrix3x3<T>& aMatrix);
		
		// Copy Constructor.
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix) = default;
		
		//Assignment operator (compiler generated)
		Matrix4x4<T>& operator=(const Matrix4x4<T>& aMatrix) = default;
		
		// () operator for accessing element (row, column) for read/write or read, respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;
		
		// Creates a transposed copy of the matrix.
		Matrix4x4<T> GetTranspose() const;

		// Static functions for creating rotation matrices.
		static Matrix4x4<T> CreateRotationAroundX(const T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundY(const T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundZ(const T aAngleInRadians);
	
	private:
		T myMatrix[4][4];
	};

	template <typename T>
	Matrix4x4<T> operator+(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1);



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
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(const T aAngleInRadians)
	{
		Matrix4x4<T> rotationMatrix;
		const T cosAngle = std::cos(aAngleInRadians);
		const T sinAngle = std::sin(aAngleInRadians);
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
		const T cosAngle = std::cos(aAngleInRadians);
		const T sinAngle = std::sin(aAngleInRadians);
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
		const T cosAngle = std::cos(aAngleInRadians);
		const T sinAngle = std::sin(aAngleInRadians);
		rotationMatrix(0, 0) = cosAngle;
		rotationMatrix(0, 1) = sinAngle;
		rotationMatrix(1, 0) = -sinAngle;
		rotationMatrix(1, 1) = cosAngle;
		return rotationMatrix;
	}
}