#pragma once
#include "Room.h"
#include "Direction.h"

class Door
{
public:
	Door(Room* a, Room* b, Direction dirA, Direction dirB, bool locked = false, bool breakable = false);

	Room* GetOtherRoom(Room* fromRoom) const;
	Direction GetDirectionFromRoom(const Room* fromRoom) const;

	bool CanPass() const;
	bool IsLocked() const;
	void SetLocked(bool locked);
	bool IsBreakableDoor() const;
	void SetBreakable(bool breakable);

private:
	Room* roomA;
	Room* roomB;
	Direction directionFromA;
	Direction directionFromB;
	bool isLocked;
	bool isBreakable;

};
