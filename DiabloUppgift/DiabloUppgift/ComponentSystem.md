
| Component				| Used By			| Purpose                       |  Status |
|---------------------- | ----------------- | ------------------------------|---------|
| DamagableComponent	| Player, Enemy		| Health, defense, damage logic |   [x]   |
| InventoryComponent	| Player			| Item storage, capacity        |   [ ]   |
| LockableComponent		| Door, Chest		| Lock / unlock logic           |   [ ]   |
| LootableComponent		| Enemy, Chest		| Loot drops                    |   [ ]   |
| InteractableComponent	| Chest, Door, etc. | Open, break, unlock, etc.     |   [ ]   |
| UsableComponent		| Potion, Key		| Use / effect logic            |   [ ]   |
| EquippableComponent	| Weapon, Armor		| Equip / unequip logic			|   [ ]   |