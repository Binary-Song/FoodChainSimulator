#include "grass.h"
#include <algorithm>
#include <atomic>
namespace EcoSim
{
    auto Grass::DecideChildrenLocation(const CellMatrix &map, Vector2 pos) -> std::vector<Vector2>
    {
        auto surrounds = map.SurroundingCells(pos);
        auto empties = ExtractPositionsOfEmptyCells(surrounds);
        return RandomSelect(empties, Grass::targetOffspringCount); 
    }
    
    std::atomic<int> Grass::population(0);

    int Grass::targetOffspringCount = 5;  
} // namespace EcoSim
