#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
    template <typename T>
    class Sphere
    {
    public:
        // Default constructor: there is no sphere, the radius is zero and the position is
        // the zero vector.
        Sphere();
        
        // Constructor that takes the center position and radius of the sphere.
        Sphere(const Vector3<T>& aCenter, T aRadius);
        
        // Init the sphere with a center and a radius, the same as the constructor above.
        void InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius);
        
        // Returns the center of the sphere.
        const Vector3<T>& GetCenter() const;
        
        // Returns the radius of the sphere.
        T GetRadius() const;
        
    private:
        Vector3<T> myCenter;
        T myRadius;
    };

    template <typename T>
    inline Sphere<T>::Sphere()
        : myCenter()
        , myRadius(static_cast<T>(0))
    {
    }

    template <typename T>
    inline Sphere<T>::Sphere(const Vector3<T>& aCenter, T aRadius)
        : myCenter(aCenter)
        , myRadius(aRadius)
    {
    }

    template <typename T>
    inline void Sphere<T>::InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius)
    {
        myCenter = aCenter;
        myRadius = aRadius;
    }

    template <typename T>
    inline const Vector3<T>& Sphere<T>::GetCenter() const
    {
        return myCenter;
    }

    template <typename T>
    inline T Sphere<T>::GetRadius() const
    {
        return myRadius;
    }
}