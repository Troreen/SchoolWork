#pragma once
#include "Direction.h"

class Room;
class Door
{
public:
    Door(Room* aRoomA,
         Room* aRoomB,
         Direction aDirectionFromA,
         Direction aDirectionFromB,
         bool aIsLocked = false,
         bool aIsBreakable = false);

    Room* GetOtherRoom(Room* aFromRoom) const;
    Direction GetDirectionFromRoom(const Room* aFromRoom) const;

    bool CanPass() const;
    bool IsLocked() const;
    void SetLocked(bool aIsLocked);
    bool IsBreakableDoor() const;
    void SetBreakable(bool aIsBreakable);

private:
    Room* myRoomA;
    Room* myRoomB;
    Direction myDirectionFromA;
    Direction myDirectionFromB;
    bool myIsLocked;
    bool myIsBreakable;
};
