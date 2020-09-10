#include "grass.h"
#include <algorithm>
namespace EcoSim
{
    auto Grass::DecideChildrenLocation(const CellMatrix &map, Vector2 pos) -> std::vector<Vector2>
    {
        auto surrounds = map.SurroundingCells(pos);
        auto empties = ExtractPositionsOfEmptyCells(surrounds);
        return RandomSelect(empties, Grass::targetOffspringCount); 
    }

    int Grass::targetOffspringCount = 3;
} // namespace EcoSim
