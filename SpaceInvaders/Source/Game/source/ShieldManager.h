#pragma once
#include <vector>
#include <memory>
#include "ShieldBlock.h"

class ShieldManager {
public:
    void Init(const ShieldType& type, int numShields);
    void Reset();
    void Update(float dt);

    const std::vector<std::unique_ptr<ShieldBlock>>& GetBlocks() const;
};
