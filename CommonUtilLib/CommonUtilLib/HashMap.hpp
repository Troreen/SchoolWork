#pragma once 
#include "MurmurHash.hpp"
#include <cstddef>

namespace CommonUtilities
{
    template <class Key, class Value>
    class HashMap
    {
    public:
        //Creates a HashMap with space for aCapacity elements. It does not need to be able to grow. If it becomes full, it becomes full.
        HashMap(int aCapacity);

        HashMap(const HashMap& aOther);

        //Frees all memory allocated 
        ~HashMap();

        HashMap& operator=(const HashMap& aOther);
        
        //Inserts aValue under the key aKey. If there was a value before with
        //aKey as the key, it will be overwritten. If successful, true is returned.
        //If the map is full, nothing is done and false is returned.
        bool Insert(const Key &aKey, const Value &aValue);
        
        //Removes the element with the key aKey, if it exists. If it exists,
        //true is returned. If it does not exist, nothing is done and false
        //is returned.
        bool Remove(const Key &aKey);
        
        //Returns a pointer to the value with the key aKey, if it exists. If it does not
        //exist, nullptr is returned.
        const Value *Get(const Key &aKey) const;
        
        //Same as above, but returns a non-const pointer instead.
        Value *Get(const Key &aKey);

    private:
        struct HashNode
        {
            Key myKey;
            Value myValue;
            HashNode* myNext;

            HashNode(const Key& aKey, const Value& aValue, HashNode* aNext)
                : myKey(aKey), myValue(aValue), myNext(aNext)
            {
            }
        };

        static bool KeysAreEqual(const Key& aFirst, const Key& aSecond);
        int GetBucketIndex(const Key& aKey) const;
        void Clear();

        int myCapacity;
        int mySize;
        int myBucketCount;
        HashNode** myBuckets;
    };

    template <class Key, class Value>
    HashMap<Key, Value>::HashMap(int aCapacity)
        : myCapacity(aCapacity > 0 ? aCapacity : 0)
        , mySize(0)
        , myBucketCount(aCapacity > 0 ? aCapacity : 1)
        , myBuckets(new HashNode*[static_cast<size_t>(myBucketCount)]{})
    {
    }

    template <class Key, class Value>
    HashMap<Key, Value>::HashMap(const HashMap& aOther)
        : myCapacity(aOther.myCapacity)
        , mySize(0)
        , myBucketCount(aOther.myBucketCount)
        , myBuckets(new HashNode*[static_cast<size_t>(myBucketCount)]{})
    {
        for (int bucketIndex = 0; bucketIndex < aOther.myBucketCount; ++bucketIndex)
        {
            const HashNode* current = aOther.myBuckets[bucketIndex];
            while (current != nullptr)
            {
                Insert(current->myKey, current->myValue);
                current = current->myNext;
            }
        }
    }

    template <class Key, class Value>
    HashMap<Key, Value>& HashMap<Key, Value>::operator=(const HashMap& aOther)
    {
        if (this == &aOther)
        {
            return *this;
        }

        Clear();
        delete[] myBuckets;

        myCapacity = aOther.myCapacity;
        mySize = 0;
        myBucketCount = aOther.myBucketCount;
        myBuckets = new HashNode*[static_cast<size_t>(myBucketCount)]{};

        for (int bucketIndex = 0; bucketIndex < aOther.myBucketCount; ++bucketIndex)
        {
            const HashNode* current = aOther.myBuckets[bucketIndex];
            while (current != nullptr)
            {
                Insert(current->myKey, current->myValue);
                current = current->myNext;
            }
        }

        return *this;
    }

    template <class Key, class Value>
    HashMap<Key, Value>::~HashMap()
    {
        Clear();
        delete[] myBuckets;
    }

    template <class Key, class Value>
    bool HashMap<Key, Value>::Insert(const Key& aKey, const Value& aValue)
    {
        const int bucketIndex = GetBucketIndex(aKey);
        HashNode* current = myBuckets[bucketIndex];

        while (current != nullptr)
        {
            if (KeysAreEqual(current->myKey, aKey))
            {
                current->myValue = aValue;
                return true;
            }

            current = current->myNext;
        }

        if (mySize >= myCapacity)
        {
            return false;
        }

        myBuckets[bucketIndex] = new HashNode(aKey, aValue, myBuckets[bucketIndex]);
        ++mySize;
        return true;
    }

    template <class Key, class Value>
    bool HashMap<Key, Value>::Remove(const Key& aKey)
    {
        const int bucketIndex = GetBucketIndex(aKey);
        HashNode* current = myBuckets[bucketIndex];
        HashNode* previous = nullptr;

        while (current != nullptr)
        {
            if (KeysAreEqual(current->myKey, aKey))
            {
                if (previous == nullptr)
                {
                    myBuckets[bucketIndex] = current->myNext;
                }
                else
                {
                    previous->myNext = current->myNext;
                }

                delete current;
                --mySize;
                return true;
            }

            previous = current;
            current = current->myNext;
        }

        return false;
    }

    template <class Key, class Value>
    const Value* HashMap<Key, Value>::Get(const Key& aKey) const
    {
        const int bucketIndex = GetBucketIndex(aKey);
        const HashNode* current = myBuckets[bucketIndex];

        while (current != nullptr)
        {
            if (KeysAreEqual(current->myKey, aKey))
            {
                return &current->myValue;
            }

            current = current->myNext;
        }

        return nullptr;
    }

    template <class Key, class Value>
    Value* HashMap<Key, Value>::Get(const Key& aKey)
    {
        return const_cast<Value*>(static_cast<const HashMap<Key, Value>&>(*this).Get(aKey));
    }

    template <class Key, class Value>
    bool HashMap<Key, Value>::KeysAreEqual(const Key& aFirst, const Key& aSecond)
    {
        return !(aFirst < aSecond) && !(aSecond < aFirst);
    }

    template <class Key, class Value>
    void HashMap<Key, Value>::Clear()
    {
        for (int bucketIndex = 0; bucketIndex < myBucketCount; ++bucketIndex)
        {
            HashNode* current = myBuckets[bucketIndex];
            while (current != nullptr)
            {
                HashNode* next = current->myNext;
                delete current;
                current = next;
            }

            myBuckets[bucketIndex] = nullptr;
        }

        mySize = 0;
    }

    template <class Key, class Value>
    int HashMap<Key, Value>::GetBucketIndex(const Key& aKey) const
    {
        return static_cast<int>(Hash(aKey) % static_cast<uint32_t>(myBucketCount));
    }
}