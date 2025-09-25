#include "Door.h"
#include "Room.h"
#include <cassert>

Door::Door(Room* aRoomA,
           Room* aRoomB,
           Direction aDirectionFromA,
           Direction aDirectionFromB,
           bool aIsLocked,
           bool aIsBreakable)
    : myRoomA(aRoomA),
      myRoomB(aRoomB),
      myDirectionFromA(aDirectionFromA),
      myDirectionFromB(aDirectionFromB),
      myIsLocked(aIsLocked),
      myIsBreakable(aIsBreakable)
{
}

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

bool Door::IsBreakableDoor() const
{
    return myIsBreakable;
}

void Door::SetBreakable(bool aIsBreakable)
{
    myIsBreakable = aIsBreakable;
}
