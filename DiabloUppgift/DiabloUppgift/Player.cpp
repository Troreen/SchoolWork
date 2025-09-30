#include "Player.h"

Player::Player(const std::string& aName, int aStrength, int aDexterity, int aPhysique)
    : myName(aName),
      myStrength(aStrength),
      myDexterity(aDexterity),
      myPhysique(aPhysique),
      myDamagable(aPhysique * 4 + aStrength * 6 + aDexterity * 3, aStrength + aPhysique)
{
}

const std::string& Player::GetName() const
{
    return myName;
}

int Player::GetStrength() const
{
    return myStrength;
}

int Player::GetDexterity() const
{
    return myDexterity;
}

int Player::GetPhysique() const
{
    return myPhysique;
}

int Player::GetHealth() const
{
    return myDamagable.GetHealth();
}

int Player::GetMaxHealth() const
{
    return myDamagable.GetMaxHealth();
}

int Player::GetDefense() const
{
    return myDamagable.GetDefense();
}

InventoryState &Player::Inventory()
{
    return myInventory;
}

const InventoryState &Player::Inventory() const
{
    return myInventory;
}

EquipmentState &Player::Equipment()
{
    return myEquipment;
}

const EquipmentState &Player::Equipment() const
{
    return myEquipment;
}

int Player::GetAttackBonusFromEquipment() const
{
    int bonus = 0;
    if (myEquipment.hasMainHand && myEquipment.mainHand.equipped)
    {
        bonus += GetItemSpec(myEquipment.mainHand.id).attackBonus;
    }
    if (myEquipment.hasChest && myEquipment.chest.equipped)
    {
        bonus += GetItemSpec(myEquipment.chest.id).attackBonus;
    }

    return bonus;
}

int Player::GetDefenseBonusFromEquipment() const
{
    int bonus = 0;
    if (myEquipment.hasMainHand && myEquipment.mainHand.equipped)
    {
        bonus += GetItemSpec(myEquipment.mainHand.id).defenseBonus;
    }
    if (myEquipment.hasChest && myEquipment.chest.equipped)
    {
        bonus += GetItemSpec(myEquipment.chest.id).defenseBonus;
    }

    return bonus;
}

bool Player::AddItem(const ItemInstance &anItem)
{
    const ItemSpec& spec = GetItemSpec(anItem.id);

    if (spec.maxStack > 1)
    {
        for (auto& item : myInventory.items)
        {
            if (item.id == anItem.id && item.count < spec.maxStack)
            {
                int space = spec.maxStack - item.count;
                int toMove = std::min(space, anItem.count);
                item.count += toMove;
                if (toMove == anItem.count)
                {
                    return true;
                } 
                
                break;
            }
        }
    }

    if ((int)myInventory.items.size() < myInventory.maxSlots)
    {
        myInventory.items.push_back(anItem);
        return true;
    }

    return false;
}

bool Player::UseHealthPotion()
{
    for (size_t i = 0; i < myInventory.items.size(); i++)
    {
        ItemInstance& instance = myInventory.items[i];
        if (instance.id == ItemId::HealthPotion && instance.count > 0)
        {
            const ItemSpec& spec = GetItemSpec(instance.id);
            myDamagable.Heal(spec.healAmount);
            instance.count -= 1;
            if (instance.count <= 0)
            {
                myInventory.items.erase(myInventory.items.begin() + static_cast<long long>(i)); 
            }

            return true;
        }
    }
    
    return false;
}

void Player::TakeDamage(int anAmount)
{
    int equipmentDefense = GetDefenseBonusFromEquipment();
    int adjustedAmount = anAmount - equipmentDefense;
    
    if (adjustedAmount < 0)
    {
        adjustedAmount = 0;
    }

    myDamagable.TakeDamage(adjustedAmount);
}

const DamagableComponent& Player::GetDamagable() const
{
    return myDamagable;
}

std::string Player::PrintStats() const
{
    std::string stats = "Name: " + myName + "\n";
    stats += "STR: " + std::to_string(myStrength) + "\n";
    stats += "DXT: " + std::to_string(myDexterity) + "\n";
    stats += "PHY: " + std::to_string(myPhysique) + "\n";
    stats += "DEF: " + std::to_string(GetDefense()) + "\n";
    stats += "HP: " + std::to_string(GetHealth()) + "/" + std::to_string(GetMaxHealth()) + "\n";
    return stats;
}
