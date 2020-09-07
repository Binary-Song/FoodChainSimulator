#include "grass.h"
#include <algorithm>
namespace FoodChain
{
    auto Grass::DecideChildrenLocation(const CellMatrix &map, Vector2 pos) -> std::vector<Vector2>
    {
        using namespace Utilities;
        auto surrounds = map.SurroundingCells(pos);
        auto empties = ExtractPositionsOfEmptyCells(surrounds);
        return RandomSelect(empties, Grass::targetOffspringCount); 
    }

    int Grass::targetOffspringCount = 3;
} // namespace FoodChain
