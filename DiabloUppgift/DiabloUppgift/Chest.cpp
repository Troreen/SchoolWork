#include "Chest.h"

Chest::Chest()
    : myName("Chest"),
      myDescription(""),
      myContents(),
      myOpened(false)
{
}

Chest::Chest(const std::string& aName, const std::string& aDescription)
    : myName(aName),
      myDescription(aDescription),
      myContents(),
      myOpened(false)
{
}

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
