namespace Tga
{
	template <typename T>
	class Vector3;
}
namespace CommonUtilities
{
	template <typename T>
	class Vector3
	{
	public:
		T x; //Note: this variable is explicitly public.
		T y; //Note: this variable is explicitly public.
		T z; //Note: this variable is explicitly public.

		//Creates a null-vector
		Vector3();

		//Creates a vector (aX, aY, aZ)
		Vector3(const T& aX, const T& aY, const T& aZ);

		//Copy constructor (compiler generated)
		Vector3(const Vector3<T>& aVector) = default;

		//Assignment operator (compiler generated)
		Vector3<T>& operator=(const Vector3<T>& aVector3) = default;

		//Destructor (compiler generated)
		~Vector3() = default;

		// Returns a CommonUtilities Vector3 copy with another datatype as elements,
		// ex: converts from Vector3<int> to Vector3<float>
		// useful to explicitly do operations, such as addition, for an explicit type
		template<class TargetType>
		Vector3<TargetType> ToType() const;

		//Returns a copy of the vector as a Tga vector, use to interface with TGE
		Tga::Vector3<T> ToTga() const;

		//Returns a negated copy of the vector
		Vector3<T> operator-() const;

		//Returns the squared length of the vector, optimization compared to length
		T LengthSqr() const;

		//Returns the length of the vector, int vector is not required to work
		T Length() const;

		//Returns a normalized copy of this vector. Need not function for int vectors
		//Handle normalization of zero-vector by returning the zero vector
		Vector3<T> GetNormalized() const;

		//Normalizes the vector. Need not function for int vectors
		//Handle normalization of zero-vector by not modifying the vector.
		void Normalize();

		//Returns the dot product of this and aVector
		T Dot(const Vector3<T>& aVector) const;

		//Returns the cross product of this and aVector. Only for Vector3
		Vector3<T> Cross(const Vector3<T>& aVector) const;

	};

	//Returns the vector sum of aVector0 and aVector1
	template <typename T>
	Vector3<T> operator+(const Vector3<T>& aVector0, const Vector3<T>& aVector1);

	//Returns the vector difference of aVector0 and aVector1
	template <typename T>
	Vector3<T> operator-(const Vector3<T>& aVector0, const Vector3<T>& aVector1);

	//Returns the vector aVector0 component-multiplied by aVector1
	template <typename T>
	Vector3<T> operator*(const Vector3<T>& aVector0, const Vector3<T>& aVector1);

	//Returns the vector aVector multiplied by the scalar aScalar. Vector * Scalar
	template <typename T>
	Vector3<T> operator*(const Vector3<T>& aVector, const T& aScalar);

	//Returns the vector aVector multiplied by the scalar aScalar. Scalar * Vector
	template <typename T>
	Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector);

	//Returns the vector aVector divided by the scalar aScalar
	template <typename T>
	Vector3<T> operator/(const Vector3<T>& aVector, const T& aScalar);

	//Equivalent to setting aVector0 to (aVector0 + aVector1)
	template <typename T>
	void operator+=(Vector3<T>& aVector0, const Vector3<T>& aVector1);

	//Equivalent to setting aVector0 to (aVector0 - aVector1)
	template <typename T>
	void operator-=(Vector3<T>& aVector0, const Vector3<T>& aVector1);

	//Equivalent to setting aVector to (aVector * aScalar)
	template <typename T>
	void operator*=(Vector3<T>& aVector, const T& aScalar);

	//Equivalent to setting aVector to (aVector / aScalar)
	template <typename T>
	void operator/=(Vector3<T>& aVector, const T& aScalar);

	//=====================================
	//  Implementations below this line
	//=====================================

}