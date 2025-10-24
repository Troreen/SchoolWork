#pragma once

#include <string>
#include <vector>
#include "InventoryTypes.h"

struct ChestSpec;
enum class ChestId;

class Chest
{
public:
    Chest();
    Chest(const std::string& aName, const std::string& aDescription);
    explicit Chest(const ChestSpec& aType);
    ~Chest();

    std::string GetName() const;
    std::string GetDescription() const;
    bool IsOpened() const;

    bool IsLocked() const;
    void SetLocked(bool aLocked);

    int GetCapacity() const;
    void SetCapacity(int aCapacity);

    void AddItem(const ItemInstance& anItem);
    std::vector<ItemInstance> Open();

    const ChestSpec& GetType() const;
    ChestId GetTypeId() const;
    bool HasType() const;

private:
    const ChestSpec* myType;
    std::vector<ItemInstance> myContents;
    bool myOpened;
    bool myLocked;
    int myCapacity;

    std::string myCustomName;
    std::string myCustomDescription;
};
