#include "sheep.h"
#include "grass.h" 
#include "game.h"
namespace EcoSim
{
	Sheep::Sheep() :
		health(initialHealth),
		gender(rand() % 2 ? LivingThingGender::Male : LivingThingGender::Female) {}
	 

	auto Sheep::DecideDestination(const CellMatrix& map, Vector2 pos) -> Vector2
	{
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
		auto surrounds = map.SurroundingCells(pos);
		auto candidatePos = ExtractPositionsOfCells(surrounds, [](const Cell& cell) {
			return dynamic_cast<Grass*>(cell.Content().get()) || cell.Content() == nullptr;
			});
		return RandomSelect(candidatePos, Sheep::targetOffspringCount);
	}

	auto Sheep::DisplayColor() const -> int
	{
		return ColorCode::Red | ColorCode::Blue | ColorCode::Green;
	}

	int Sheep::targetOffspringCount = 1;

	int Sheep::starvationHealthHarm = 1;

	int Sheep::consumptionHealthBenifit = 10;

	int Sheep::maximumHealth = 10;

	int Sheep::initialHealth = 4; 

	int Sheep::minimumReproduceHealth = 5;
}