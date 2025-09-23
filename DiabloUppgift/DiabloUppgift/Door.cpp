#include "Door.h"
#include "Room.h"	
#include <cassert>

Door::Door(Room* a, Room* b, Direction dirA, Direction dirB, bool locked, bool breakable)
	: roomA(a), roomB(b), directionFromA(dirA), directionFromB(dirB), isLocked(locked), isBreakable(breakable)
{
}

Room* Door::GetOtherRoom(Room* fromRoom) const
{
	if (fromRoom == roomA) return roomB;
	if (fromRoom == roomB) return roomA;
	assert(false && "Invalid room provided to GetOtherRoom");
	return nullptr;
}

Direction Door::GetDirectionFromRoom(const Room* fromRoom) const
{
	if (fromRoom == roomA) return directionFromA;
	if (fromRoom == roomB) return directionFromB;
	assert(false && "Invalid room provided to GetDirectionFromRoom"); 
	return directionFromA; 
}

bool Door::CanPass() const
{
	return !isLocked;
}

bool Door::IsLocked() const
{
	return isLocked;
}

void Door::SetLocked(bool locked)
{
	isLocked = locked;
}

bool Door::IsBreakableDoor() const
{
	return isBreakable;
}

void Door::SetBreakable(bool breakable)
{
	isBreakable = breakable;
}