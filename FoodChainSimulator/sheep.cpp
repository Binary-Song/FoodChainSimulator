#include "sheep.h"
#include "grass.h" 

namespace FoodChain
{
	auto Sheep::DecideDestination(const CellMatrix& map, Vector2 pos) -> Vector2
	{
		using namespace Utilities;
		// 找到周围所有草的位置
		auto surrounds = map.SurroundingCells(pos);
		auto grassPos = ExtractPositionsOfCellsByContentType<Grass>(surrounds); 
		// 若有，则随机选一个
		if (grassPos.size() > 0) 
			return RandomSelect(grassPos);
		 
		// 若无，则找到周围所有空格子的位置
		auto emptyPos = ExtractPositionsOfEmptyCells(surrounds);
		// 若有，则随机选一个
		if (emptyPos.size() > 0) 
			return RandomSelect(emptyPos);
		 
		// 若无，则不动。
		return pos;
	}

	auto Sheep::DecideChildrenLocation(const CellMatrix& map, Vector2 pos) -> std::vector<Vector2>
	{
		using namespace Utilities;
		if (health < 5) return std::vector<Vector2>();

		auto surrounds = map.SurroundingCells(pos);
		auto emptyPos = ExtractPositionsOfEmptyCells(surrounds);
		return RandomSelect(emptyPos, Sheep::targetOffspringCount);
	}

	auto Sheep::DisplayColor() const -> int
	{
		using namespace Utilities;
		return ColorCode::Red | ColorCode::Blue | ColorCode::Green;
	}

	int Sheep::targetOffspringCount = 1;
}