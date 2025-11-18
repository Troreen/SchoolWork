#pragma once
#include <cmath>

namespace Tga
{
	template <typename T>
	class Vector4;
}

namespace CommonUtilities
{
	template <typename T>
	class Vector4
	{
	public:
		T x;
		T y;
		T z;
		T w;

		Vector4();
		Vector4(const T& aX, const T& aY, const T& aZ, const T& aW);
		Vector4(const Vector4<T>&) = default;
		Vector4& operator=(const Vector4<T>&) = default;
		~Vector4() = default;

		template<class TargetType>
		Vector4<TargetType> ToType() const;
		Tga::Vector4<T> ToTga() const;

		Vector4<T> operator-() const;
		T LengthSqr() const;
		T Length() const;
		Vector4<T> GetNormalized() const;
		void Normalize();
		T Dot(const Vector4<T>& aVector) const;
	};

	template <typename T>
	Vector4<T> operator+(const Vector4<T>& aVector0, const Vector4<T>& aVector1);
	
	template <typename T> 
	Vector4<T> operator-(const Vector4<T>& aVector0, const Vector4<T>& aVector1);
	
	template <typename T> 
	Vector4<T> operator*(const Vector4<T>& aVector0, const Vector4<T>& aVector1);
	
	template <typename T>
	Vector4<T> operator*(const Vector4<T>& aVector, const T& aScalar);
	
	template <typename T> 
	Vector4<T> operator*(const T& aScalar, const Vector4<T>& aVector);
	
	template <typename T> 
	Vector4<T> operator/(const Vector4<T>& aVector, const T& aScalar);
	
	template <typename T>
	void operator+=(Vector4<T>& aVector0, const Vector4<T>& aVector1);
	
	template <typename T> 
	void operator-=(Vector4<T>& aVector0, const Vector4<T>& aVector1);
	
	template <typename T> 
	void operator*=(Vector4<T>& aVector, const T& aScalar);
	
	template <typename T>
	void operator/=(Vector4<T>& aVector, const T& aScalar);

	//================
	// Implementations
	//================
	template<typename T>
	inline Vector4<T>::Vector4() 
		: x(static_cast<T>(0)), 
		y(static_cast<T>(0)), 
		z(static_cast<T>(0)), 
		w(static_cast<T>(0)) 
	{
	}

	template<typename T>
	inline Vector4<T>::Vector4(const T& aX, const T& aY, const T& aZ, const T& aW) 
		: x(aX), 
		y(aY), 
		z(aZ), 
		w(aW) 
	{
	}

	template<typename T>
	inline Tga::Vector4<T> Vector4<T>::ToTga() const 
	{ 
		return Tga::Vector4<T>(x, y, z, w); 
	}

	template<typename T>
	inline Vector4<T> Vector4<T>::operator-() const 
	{ 
		return Vector4<T>(-x, -y, -z, -w); 
	}

	template<typename T>
	inline T Vector4<T>::LengthSqr() const 
	{ 
		return x * x + y * y + z * z + w * w; 
	}

	template<typename T>
	inline T Vector4<T>::Length() const 
	{ 
		return static_cast<T>(std::sqrt(LengthSqr())); 
	}

	template<typename T>
	inline Vector4<T> Vector4<T>::GetNormalized() const
	{
		const T zero = static_cast<T>(0);
		T len = Length();
		if (len == zero) return Vector4<T>(zero, zero, zero, zero);
		const T inv = static_cast<T>(1) / len;
		return Vector4<T>(x * inv, y * inv, z * inv, w * inv);
	}

	template<typename T>
	inline void Vector4<T>::Normalize()
	{
		const T zero = static_cast<T>(0);
		T len = Length();
		if (len == zero) return;
		const T inv = static_cast<T>(1) / len;
		x *= inv; y *= inv; z *= inv; w *= inv;
	}

	template<typename T>
	inline T Vector4<T>::Dot(const Vector4<T>& aVector) const 
	{ 
		return x * aVector.x + y * aVector.y + z * aVector.z + w * aVector.w; 
	}

	template<typename T>
	inline Vector4<T> operator+(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{ 
		return Vector4<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z, aVector0.w + aVector1.w); 
	}

	template<typename T>
	inline Vector4<T> operator-(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{ 
		return Vector4<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z, aVector0.w - aVector1.w); 
	}

	template<typename T>
	inline Vector4<T> operator*(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{ 
		return Vector4<T>(aVector0.x * aVector1.x, aVector0.y * aVector1.y, aVector0.z * aVector1.z, aVector0.w * aVector1.w); 
	}

	template<typename T>
	inline Vector4<T> operator*(const Vector4<T>& aVector, const T& aScalar)
	{ 
		return Vector4<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar, aVector.w * aScalar); 
	}

	template<typename T>
	inline Vector4<T> operator*(const T& aScalar, const Vector4<T>& aVector)
	{ 
		return Vector4<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar, aVector.w * aScalar); 
	}

	template<typename T>
	inline Vector4<T> operator/(const Vector4<T>& aVector, const T& aScalar)
	{ 
		return Vector4<T>(aVector.x / aScalar, aVector.y / aScalar, aVector.z / aScalar, aVector.w / aScalar); 
	}

	template<typename T>
	inline void operator+=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{ 
		aVector0.x += aVector1.x; aVector0.y += aVector1.y; aVector0.z += aVector1.z; aVector0.w += aVector1.w; 
	}

	template<typename T>
	inline void operator-=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{ 
		aVector0.x -= aVector1.x; aVector0.y -= aVector1.y; aVector0.z -= aVector1.z; aVector0.w -= aVector1.w; 
	}

	template<typename T>
	inline void operator*=(Vector4<T>& aVector, const T& aScalar)
	{ 
		aVector.x *= aScalar; aVector.y *= aScalar; aVector.z *= aScalar; aVector.w *= aScalar; 
	}

	template<typename T>
	inline void operator/=(Vector4<T>& aVector, const T& aScalar)
	{ 
		aVector.x /= aScalar; aVector.y /= aScalar; aVector.z /= aScalar; aVector.w /= aScalar; 
	}

	template<typename T>
	template<class TargetType>
	inline Vector4<TargetType> Vector4<T>::ToType() const
	{ 
		return Vector4<TargetType>(static_cast<TargetType>(x), static_cast<TargetType>(y), static_cast<TargetType>(z), static_cast<TargetType>(w)); 
	}
}
