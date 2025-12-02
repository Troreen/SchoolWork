#pragma once
#include <cmath>

namespace Tga
{
	template <typename T>
	class Vector2;
}

namespace CommonUtilities
{
	template <typename T>
	class Vector2
	{
	public:
		T x;
		T y;

		Vector2();
		Vector2(const T& aX, const T& aY);
		Vector2(const Vector2<T>&) = default;
		Vector2& operator=(const Vector2<T>&) = default;
		~Vector2() = default;

		template<class TargetType>
		Vector2<TargetType> ToType() const;
		Tga::Vector2<T> ToTga() const;

		Vector2<T> operator-() const;
		T LengthSqr() const;
		T Length() const;
		Vector2<T> GetNormalized() const;
		void Normalize();
		T Dot(const Vector2<T>& aVector) const;
	};

	template <typename T> 
	Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>& aVector1);

	template <typename T> 
	Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>& aVector1);
	
	template <typename T> 
	Vector2<T> operator*(const Vector2<T>& aVector0, const Vector2<T>& aVector1);
	
	template <typename T> 
	Vector2<T> operator*(const Vector2<T>& aVector, const T& aScalar);
	
	template <typename T> 
	Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector);
	
	template <typename T> 
	Vector2<T> operator/(const Vector2<T>& aVector, const T& aScalar);
	
	template <typename T> 
	void operator+=(Vector2<T>& aVector0, const Vector2<T>& aVector1);
	
	template <typename T> 
	void operator-=(Vector2<T>& aVector0, const Vector2<T>& aVector1);
	
	template <typename T> 
	void operator*=(Vector2<T>& aVector, const T& aScalar);
	
	template <typename T> 
	void operator/=(Vector2<T>& aVector, const T& aScalar);

	//================
	// Implementations
	//================
	
	template<typename T>
	inline Vector2<T>::Vector2()
	{
		x = static_cast<T>(0);
		y = static_cast<T>(0);
	}

	template<typename T>
	inline Vector2<T>::Vector2(const T& aX, const T& aY)
	{
		x = aX;
		y = aY;
	}

	template<typename T>
	inline Tga::Vector2<T> Vector2<T>::ToTga() const
	{
		return Tga::Vector2<T>(x, y);
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::operator-() const
	{
		return Vector2<T>(-x, -y);
	}

	template<typename T>
	inline T Vector2<T>::LengthSqr() const
	{
		return x * x + y * y;
	}

	template<typename T>
	inline T Vector2<T>::Length() const
	{
		return static_cast<T>(std::sqrt(LengthSqr()));
	}

	template<typename T>
	inline Vector2<T> Vector2<T>::GetNormalized() const
	{
		const T zero = static_cast<T>(0);
		T len = Length();
		if (len == zero)
		{
			return Vector2<T>(zero, zero);
		}
		const T inv = static_cast<T>(1) / len;
		return Vector2<T>(x * inv, y * inv);
	}

	template<typename T>
	inline void Vector2<T>::Normalize()
	{
		const T zero = static_cast<T>(0);
		T len = Length();
		if (len == zero)
		{
			return;
		}
		const T inv = static_cast<T>(1) / len;
		x *= inv;
		y *= inv;
	}

	template<typename T>
	inline T Vector2<T>::Dot(const Vector2<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y;
	}

	template<typename T>
	inline Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return Vector2<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y);
	}

	template<typename T>
	inline Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return Vector2<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y);
	}

	template<typename T>
	inline Vector2<T> operator*(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		return Vector2<T>(aVector0.x * aVector1.x, aVector0.y * aVector1.y);
	}

	template<typename T>
	inline Vector2<T> operator*(const Vector2<T>& aVector, const T& aScalar)
	{
		return Vector2<T>(aVector.x * aScalar, aVector.y * aScalar);
	}

	template<typename T>
	inline Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector)
	{
		return Vector2<T>(aVector.x * aScalar, aVector.y * aScalar);
	}

	template<typename T>
	inline Vector2<T> operator/(const Vector2<T>& aVector, const T& aScalar)
	{
		return Vector2<T>(aVector.x / aScalar, aVector.y / aScalar);
	}

	template<typename T>
	inline void operator+=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
	}

	template<typename T>
	inline void operator-=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
	}

	template<typename T>
	inline void operator*=(Vector2<T>& aVector, const T& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
	}

	template<typename T>
	inline void operator/=(Vector2<T>& aVector, const T& aScalar)
	{
		aVector.x /= aScalar;
		aVector.y /= aScalar;
	}

	template<typename T>
	template<class TargetType>
	inline Vector2<TargetType> Vector2<T>::ToType() const
	{
		return Vector2<TargetType>(static_cast<TargetType>(x), static_cast<TargetType>(y));
	}
}
