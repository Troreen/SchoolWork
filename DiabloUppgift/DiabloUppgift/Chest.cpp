#include "Chest.h"

Chest::Chest()
    : myName("Chest")
    , myDescription("")
    , myContents()
    , myOpened(false)
    , myLocked(false)
    , myCapacity(0)
{
}

Chest::Chest(const std::string& aName, const std::string& aDescription)
    : myName(aName)
    , myDescription(aDescription)
    , myContents()
    , myOpened(false)
    , myLocked(false)
    , myCapacity(0)
{
}

Chest::~Chest() = default;

const std::string& Chest::GetName() const
{
    return myName;
}

const std::string& Chest::GetDescription() const
{
    return myDescription;
}

bool Chest::IsOpened() const
{
    return myOpened;
}

bool Chest::IsLocked() const
{
    return myLocked;
}

void Chest::SetLocked(bool aLocked)
{
    myLocked = aLocked;
}

int Chest::GetCapacity() const
{
    return myCapacity;
}

void Chest::SetCapacity(int aCapacity)
{
    if (aCapacity < 0)
    {
        aCapacity = 0;
    }
    myCapacity = aCapacity;
    
}

void Chest::AddItem(const ItemInstance& anItem)
{
    myContents.push_back(anItem);
}

std::vector<ItemInstance> Chest::Open()
{
    if (myOpened)
    {
        return {};
    }

    myOpened = true;
    std::vector<ItemInstance> contents = myContents;
    myContents.clear();
    return contents;
}
