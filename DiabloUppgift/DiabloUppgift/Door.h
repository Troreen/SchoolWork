#pragma once
#include "Direction.h"
#include "Player.h"

class Room;
class Door
{
public:
    Door(Room* aRoomA,
         Room* aRoomB,
         Direction aDirectionFromA,
         Direction aDirectionFromB,
         bool aIsLocked = false,
         int aDexterityToUnlock = 0,
         int aStrengthToBreak = 0
        );
    Door() = delete;
    ~Door();

    Room* GetOtherRoom(Room* aFromRoom) const;
    Direction GetDirectionFromRoom(const Room* aFromRoom) const;
    
    int GetDexterityToUnlock() const;
    int GetStrengthToBreak() const;

    bool CanPass() const;
    bool IsLocked() const;
    void SetLocked(bool aIsLocked);

    const bool TryUnlock(const Player &aPlayer);
    const bool TryBreak(const Player &aPlayer);

private:
    Room* myRoomA;
    Room* myRoomB;
    Direction myDirectionFromA;
    Direction myDirectionFromB;
    bool myIsLocked;
    int myDexterityToUnlock;
    int myStrengthToBreak; 
};
