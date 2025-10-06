#include "Chest.h"
#include "ChestTypes.h"

Chest::Chest()
    : myType(nullptr)
    , myContents()
    , myOpened(false)
    , myLocked(false)
    , myCapacity(0)
    , myCustomName("Chest")
    , myCustomDescription()
{
}

Chest::Chest(const std::string& aName, const std::string& aDescription)
    : myType(nullptr)
    , myContents()
    , myOpened(false)
    , myLocked(false)
    , myCapacity(0)
    , myCustomName(aName)
    , myCustomDescription(aDescription)
{
}

Chest::Chest(const ChestSpec& aType)
    : myType(&aType)
    , myContents()
    , myOpened(false)
    , myLocked(aType.locked)
    , myCapacity(0)
    , myCustomName()
    , myCustomDescription()
{
    SetCapacity(aType.capacity);
}

Chest::~Chest() = default;

std::string Chest::GetName() const
{
    if (myType && myType->name)
    {
        return myType->name;
    }

    return myCustomName.empty() ? std::string("Chest") : myCustomName;
}

std::string Chest::GetDescription() const
{
    if (myType && myType->description)
    {
        return myType->description;
    }

    return myCustomDescription;
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

const ChestSpec& Chest::GetType() const
{
    if (myType)
    {
        return *myType;
    }

    return GetChestSpec(ChestId::WoodenChest);
}

ChestId Chest::GetTypeId() const
{
    return GetType().id;
}

bool Chest::HasType() const
{
    return myType != nullptr;
}
