#include "sheep.h"
#include "grass.h" 
#include "game.h"
namespace EcoSim
{
	Sheep::Sheep() :
		health(initialHealth),
		gender(Rand() % 2 ? LivingThingGender::Male : LivingThingGender::Female) {
		Birth();
	}
	 

	auto Sheep::DecideDestination(const CellMatrix& map, Vector2 pos) -> Vector2
	{
		// 找到周围所有草的位置
		auto surrounds = map.SurroundingCells(pos);
		auto grassPos = map.ExtractCellPositions<Grass>(surrounds);
		// 若有，则随机选一个
		if (grassPos.size() > 0)
			return RandomSelect(grassPos);

		// 若无，则找到周围所有空格子的位置
		auto emptyPos = map.ExtractEmptyCellPositions(surrounds);
		// 若有，则随机选一个
		if (emptyPos.size() > 0)
			return RandomSelect(emptyPos);

		// 若无，则不动。
		return pos;
	}

	auto Sheep::DecideChildrenLocation(const CellMatrix& map, Vector2 pos) -> std::vector<Vector2>
	{ 
		auto surrounds = map.SurroundingCells(pos);
		auto candidatePos = map.ExtractCellPositions(surrounds, [](const Cell& cell) {
			return sp_dynamic_cast<Grass>(cell.Content()) || cell.Content() == nullptr;
			});
		return RandomSelect(candidatePos, Sheep::targetOffspringCount);
	} 

	int Sheep::targetOffspringCount = 1;

	int Sheep::starvationHealthHarm = 1;

	int Sheep::consumptionHealthBenifit = 8;

	int Sheep::maximumHealth = 8;

	int Sheep::initialHealth = 6; 

	int Sheep::minimumReproduceHealth = 7;

	std::atomic<int> Sheep::population(0);
}