#pragma once

#include <string>
#include <vector>
#include "InventoryTypes.h"

class Chest
{
public:
    Chest();
    Chest(const std::string& aName, const std::string& aDescription);
    ~Chest();

    const std::string& GetName() const;
    const std::string& GetDescription() const;
    bool IsOpened() const;

    void AddItem(const ItemInstance& anItem);
    std::vector<ItemInstance> Open();

private:
    std::string myName;
    std::string myDescription;
    std::vector<ItemInstance> myContents;
    bool myOpened;
};
