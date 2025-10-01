#include "Door.h"
#include "Room.h"
#include <cassert>

Door::Door(Room* aRoomA,
           Room* aRoomB,
           Direction aDirectionFromA,
           Direction aDirectionFromB,
           bool aIsLocked,
           int aDexterityToUnlock,
           int aStrengthToBreak)
    : myRoomA(aRoomA),
      myRoomB(aRoomB),
      myDirectionFromA(aDirectionFromA),
      myDirectionFromB(aDirectionFromB),
      myIsLocked(aIsLocked),
      myDexterityToUnlock(aDexterityToUnlock),
      myStrengthToBreak(aStrengthToBreak)
{
}

Door::~Door() = default;

Room* Door::GetOtherRoom(Room* aFromRoom) const
{
    if (aFromRoom == myRoomA)
    {
        return myRoomB;
    }

    if (aFromRoom == myRoomB)
    {
        return myRoomA;
    }

    assert(false && "Invalid room provided to GetOtherRoom");
    return nullptr;
}

Direction Door::GetDirectionFromRoom(const Room* aFromRoom) const
{
    if (aFromRoom == myRoomA)
    {
        return myDirectionFromA;
    }

    if (aFromRoom == myRoomB)
    {
        return myDirectionFromB;
    }

    assert(false && "Invalid room provided to GetDirectionFromRoom");
    return Direction::DirectionCount;
}

int Door::GetDexterityToUnlock() const
{
    return myDexterityToUnlock;
}

int Door::GetStrengthToBreak() const
{
    return myStrengthToBreak;
}

bool Door::CanPass() const
{
    return !myIsLocked;
}

bool Door::IsLocked() const
{
    return myIsLocked;
}

void Door::SetLocked(bool aIsLocked)
{
    myIsLocked = aIsLocked;
}

const bool Door::TryUnlock(const Player& aPlayer)
{
    if (aPlayer.GetDexterity() >= myDexterityToUnlock)
    {
        myIsLocked = false;
        return true;
    }
    return false;
}

const bool Door::TryBreak(const Player& aPlayer)
{
    if (aPlayer.GetStrength() >= myStrengthToBreak)
    {
        myIsLocked = false;
        return true;
    }
    return false;
}