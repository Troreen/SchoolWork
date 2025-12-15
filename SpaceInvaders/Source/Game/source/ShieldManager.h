#pragma once
#include <vector>
#include <memory>
#include "ShieldBlock.h"

class ShieldManager {
public:
    ShieldManager() = default;
    ~ShieldManager() = default;

    void Init(const ShieldType& type, int numShields);
    void Reset();
    void Update(float dt);

    const std::vector<std::unique_ptr<ShieldBlock>>& GetBlocks() const;

private:
    const ShieldType* myType = nullptr;
    int myNumShields = 0;
    std::vector<std::unique_ptr<ShieldBlock>> myBlocks;
};
