#include "Player.h"

namespace
{
    int MinInt(int a, int b)
    {
        return (a < b) ? a : b;
    }

    int MaxInt(int a, int b)
    {
        return (a > b) ? a : b;
    }

    std::string FormatSignedValue(int aValue)
    {
        return (aValue >= 0 ? std::string("+") : std::string("")) + std::to_string(aValue);
    }

    std::string BuildStatLine(const std::string& aLabel, int aBase, int anEquipmentBonus, int anEnchantmentBonus)
    {
        const int total = aBase + anEquipmentBonus + anEnchantmentBonus;
        std::string line = aLabel + ": " + std::to_string(total);

        if (anEquipmentBonus != 0)
        {
            line += " (" + FormatSignedValue(anEquipmentBonus) + ")";
        }

        if (anEnchantmentBonus != 0)
        {
            line += " [" + FormatSignedValue(anEnchantmentBonus) + "]";
        }

        line += "\n";
        return line;
    }
}

Player::Player(const std::string& aName, int aStrength, int aDexterity, int aPhysique)
    : myName(aName),
      myStrength(aStrength),
      myDexterity(aDexterity),
      myPhysique(aPhysique),
      myDamagable(aPhysique * 4 + aStrength * 6 + aDexterity * 3, aStrength + aPhysique)
{
    RecalculateDerivedStats();
}

Player::~Player() = default;

const std::string& Player::GetName() const
{
    return myName;
}

int Player::GetStrength() const
{
    return myStrength + GetStrengthBonusFromEquipment() + GetStrengthBonusFromEnchantments();
}

int Player::GetDexterity() const
{
    return myDexterity + GetDexterityBonusFromEquipment() + GetDexterityBonusFromEnchantments();
}

int Player::GetPhysique() const
{
    return myPhysique + GetPhysiqueBonusFromEquipment() + GetPhysiqueBonusFromEnchantments();
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

int Player::GetAttackBonusFromEnchantments() const
{
    int bonus = 0;
    for (const ActiveEnchantment& enchantment : myActiveEnchantments)
    {
        bonus += GetItemSpec(enchantment.id).attackBonus;
    }

    return bonus;
}

int Player::GetDefenseBonusFromEnchantments() const
{
    int bonus = 0;
    for (const ActiveEnchantment& enchantment : myActiveEnchantments)
    {
        bonus += GetItemSpec(enchantment.id).defenseBonus;
    }

    return bonus;
}

int Player::GetStrengthBonusFromEquipment() const
{
    int bonus = 0;
    if (myEquipment.hasMainHand && myEquipment.mainHand.equipped)
    {
        bonus += GetItemSpec(myEquipment.mainHand.id).strengthModifier;
    }
    if (myEquipment.hasChest && myEquipment.chest.equipped)
    {
        bonus += GetItemSpec(myEquipment.chest.id).strengthModifier;
    }
    return bonus;
}

int Player::GetStrengthBonusFromEnchantments() const
{
    int bonus = 0;
    for (const ActiveEnchantment& enchantment : myActiveEnchantments)
    {
        bonus += GetItemSpec(enchantment.id).strengthModifier;
    }
    return bonus;
}

int Player::GetDexterityBonusFromEquipment() const
{
    int bonus = 0;
    if (myEquipment.hasMainHand && myEquipment.mainHand.equipped)
    {
        bonus += GetItemSpec(myEquipment.mainHand.id).dexterityModifier;
    }
    if (myEquipment.hasChest && myEquipment.chest.equipped)
    {
        bonus += GetItemSpec(myEquipment.chest.id).dexterityModifier;
    }
    return bonus;
}

int Player::GetDexterityBonusFromEnchantments() const
{
    int bonus = 0;
    for (const ActiveEnchantment& enchantment : myActiveEnchantments)
    {
        bonus += GetItemSpec(enchantment.id).dexterityModifier;
    }
    return bonus;
}

int Player::GetPhysiqueBonusFromEquipment() const
{
    int bonus = 0;
    if (myEquipment.hasMainHand && myEquipment.mainHand.equipped)
    {
        bonus += GetItemSpec(myEquipment.mainHand.id).physiqueModifier;
    }
    if (myEquipment.hasChest && myEquipment.chest.equipped)
    {
        bonus += GetItemSpec(myEquipment.chest.id).physiqueModifier;
    }
    return bonus;
}

int Player::GetPhysiqueBonusFromEnchantments() const
{
    int bonus = 0;
    for (const ActiveEnchantment& enchantment : myActiveEnchantments)
    {
        bonus += GetItemSpec(enchantment.id).physiqueModifier;
    }
    return bonus;
}

bool Player::HasActiveEnchantments() const
{
    return !myActiveEnchantments.empty();
}

std::vector<ActiveEnchantment>& Player::ActiveEnchantments()
{
    return myActiveEnchantments;
}

const std::vector<ActiveEnchantment>& Player::ActiveEnchantments() const
{
    return myActiveEnchantments;
}

void Player::AddActiveEnchantment(const ActiveEnchantment& anEnchantment)
{
    const ItemSpec& spec = GetItemSpec(anEnchantment.id);
    int duration = anEnchantment.durationTurns > 0 ? anEnchantment.durationTurns : spec.durationTurns;
    if (duration <= 0)
    {
        duration = spec.durationTurns;
    }

    for (auto& active : myActiveEnchantments)
    {
        if (active.id == anEnchantment.id)
        {
            active.durationTurns = MaxInt(active.durationTurns, duration);
            RecalculateDerivedStats();
            return;
        }
    }

    myActiveEnchantments.push_back({ anEnchantment.id, duration });
    RecalculateDerivedStats();
}

std::string Player::GetActiveEnchantmentsSummary() const
{
    if (myActiveEnchantments.empty())
    {
        return {};
    }

    std::string summary;
    for (const ActiveEnchantment& enchantment : myActiveEnchantments)
    {
        const ItemSpec& spec = GetItemSpec(enchantment.id);
        if (!summary.empty())
        {
            summary += ", ";
        }

        summary += spec.name;
        if (enchantment.durationTurns > 0)
        {
            summary += " (" + std::to_string(enchantment.durationTurns) + " turns left)";
        }
    }

    return summary;
}

void Player::AdvanceActiveEnchantments()
{
    bool durationsChanged = false;

    for (auto& enchantment : myActiveEnchantments)
    {
        if (enchantment.durationTurns > 0)
        {
            enchantment.durationTurns -= 1;
            durationsChanged = true;
        }
    }

    size_t writeIndex = 0;
    for (size_t readIndex = 0; readIndex < myActiveEnchantments.size(); ++readIndex)
    {
        const ActiveEnchantment& enchantment = myActiveEnchantments[readIndex];
        bool shouldRemove = false;

        if (enchantment.durationTurns <= 0)
        {
            const ItemSpec& spec = GetItemSpec(enchantment.id);
            if (spec.durationTurns > 0)
            {
                shouldRemove = true;
            }
        }

        if (!shouldRemove)
        {
            if (writeIndex != readIndex)
            {
                myActiveEnchantments[writeIndex] = enchantment;
            }

            ++writeIndex;
        }
    }

    const bool removedAny = writeIndex != myActiveEnchantments.size();
    if (removedAny)
    {
        myActiveEnchantments.resize(writeIndex);
    }

    if (durationsChanged || removedAny)
    {
        RecalculateDerivedStats();
    }
}

void Player::RecalculateDerivedStats()
{
    const int totalStrength = GetStrength();
    const int totalDexterity = GetDexterity();
    const int totalPhysique = GetPhysique();

    int maxHealth = totalPhysique * 4 + totalStrength * 6 + totalDexterity * 3;
    if (maxHealth < 0)
    {
        maxHealth = 0;
    }

    myDamagable.SetMaxHealth(maxHealth);

    const int defenseFromStats = totalStrength + totalPhysique;
    const int defenseBonus = GetDefenseBonusFromEquipment() + GetDefenseBonusFromEnchantments();
    int totalDefense = defenseFromStats + defenseBonus;
    if (totalDefense < 0)
    {
        totalDefense = 0;
    }

    myDamagable.SetDefense(totalDefense);
}

float Player::GetCarryWeight() const
{
    return CalculateCarryWeight();
}

float Player::GetRemainingCarryWeight() const
{
    float remaining = myInventory.maxCarryWeight - CalculateCarryWeight();
    return remaining < 0.0f ? 0.0f : remaining;
}

float Player::GetMaxCarryWeight() const
{
    return myInventory.maxCarryWeight;
}

bool Player::CanCarryAdditionalWeight(float aWeight) const
{
    const float newWeight = CalculateCarryWeight() + aWeight;
    if (newWeight < 0.0f)
    {
        return true;
    }
    return newWeight <= myInventory.maxCarryWeight + 0.0001f;
}

bool Player::CanCarryItem(const ItemInstance& anItem) const
{
    return CanCarryAdditionalWeight(GetItemWeight(anItem));
}

float Player::ComputeItemWeight(const ItemInstance& anItem) const
{
    return GetItemWeight(anItem);
}

float Player::CalculateCarryWeight() const
{
    float totalWeight = 0.0f;

    for (const ItemInstance& item : myInventory.items)
    {
        totalWeight += GetItemWeight(item);
    }

    if (myEquipment.hasMainHand && myEquipment.mainHand.equipped)
    {
        totalWeight += GetItemWeight(myEquipment.mainHand);
    }

    if (myEquipment.hasChest && myEquipment.chest.equipped)
    {
        totalWeight += GetItemWeight(myEquipment.chest);
    }

    return totalWeight;
}

float Player::GetItemWeight(const ItemInstance& anItem) const
{
    const ItemSpec& spec = GetItemSpec(anItem.id);
    int count = anItem.count;
    if (count <= 0)
    {
        count = 1;
    }
    return spec.weight * static_cast<float>(count);
}

bool Player::AddItem(const ItemInstance &anItem)
{
    const ItemSpec& spec = GetItemSpec(anItem.id);

    if (!CanCarryItem(anItem))
    {
        return false;
    }

    if (spec.maxStack > 1)
    {
        for (auto& item : myInventory.items)
        {
            if (item.id == anItem.id && item.count < spec.maxStack)
            {
                int space = spec.maxStack - item.count;
                int toMove = MinInt(space, anItem.count);
                item.count += toMove;
                if (toMove == anItem.count)
                {
                    return true;
                } 
                ItemInstance remaining = anItem;
                remaining.count -= toMove;
                return AddItem(remaining);
            }
        }
    }

    myInventory.items.push_back(anItem);
    return true;
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
    if (anAmount < 0)
    {
        anAmount = 0;
    }

    myDamagable.TakeDamage(anAmount);
}

const DamagableComponent& Player::GetDamagable() const
{
    return myDamagable;
}

std::string Player::PrintStats() const
{
    std::string stats = "Name: " + myName + "\n";
    const int strengthEquipment = GetStrengthBonusFromEquipment();
    const int strengthEnchantment = GetStrengthBonusFromEnchantments();
    stats += BuildStatLine("STR", myStrength, strengthEquipment, strengthEnchantment);

    const int dexEquipment = GetDexterityBonusFromEquipment();
    const int dexEnchantment = GetDexterityBonusFromEnchantments();
    stats += BuildStatLine("DEX", myDexterity, dexEquipment, dexEnchantment);

    const int physEquipment = GetPhysiqueBonusFromEquipment();
    const int physEnchantment = GetPhysiqueBonusFromEnchantments();
    stats += BuildStatLine("PHY", myPhysique, physEquipment, physEnchantment);

    const int baseAttack = myStrength * 2;
    const int equipmentAttack = strengthEquipment * 2 + GetAttackBonusFromEquipment();
    const int enchantmentAttack = strengthEnchantment * 2 + GetAttackBonusFromEnchantments();
    stats += BuildStatLine("ATK", baseAttack, equipmentAttack, enchantmentAttack);

    const int baseDefense = myStrength + myPhysique;
    const int equipmentDefense = strengthEquipment + physEquipment + GetDefenseBonusFromEquipment();
    const int enchantmentDefense = strengthEnchantment + physEnchantment + GetDefenseBonusFromEnchantments();
    stats += BuildStatLine("DEF", baseDefense, equipmentDefense, enchantmentDefense);

    stats += "HP: " + std::to_string(GetHealth()) + "/" + std::to_string(GetMaxHealth()) + "\n";
    return stats;
}

