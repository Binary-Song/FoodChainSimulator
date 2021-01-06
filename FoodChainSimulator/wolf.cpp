#include "wolf.h"
#include "grass.h"
#include "utils.h"
#include "sheep.h"
#include "random_gen.h"
namespace EcoSim
{   

	Wolf::Wolf() :
		health(initialHealth),
		gender(Rand() % 2 ? LivingThingGender::Male : LivingThingGender::Female),
		randVal(Rand())
	{
		Birth();
	}


	auto Wolf::DecideDestination(const CellMatrix& map, Vector2 pos) -> Vector2
	{
		// 找到周围所有羊的位置
		auto surrounds = map.SurroundingCells(pos);
		auto sheepPos = map.FilterPositionsByType<Sheep>(surrounds);
		// 若有，则随机选一个
		if (sheepPos.size() > 0)
			return RandomSelect(sheepPos);

		// 若无，则找到周围所有草或空格子的位置
		auto okPos = map.FilterPositions(surrounds, [](const Cell& cell) {
			return (cell.Content() == nullptr || sp_dynamic_cast<Grass>(cell.Content()));
			});
		
		// 若有，则随机选一个
		if (okPos.size() > 0)
		{
			auto pos = RandomSelect(okPos);
		 
			return pos;
		}
		// 若无，则不动。
		return pos;
	}

	auto Wolf::DecideChildrenLocation(const CellMatrix& map, Vector2 pos)->std::vector<Vector2> 
	{
		auto surrounds = map.SurroundingCells(pos);
		auto candidatePos = map.FilterPositions(surrounds, [](const Cell& cell) {
			return sp_dynamic_cast<Grass>(cell.Content()) || cell.Content() == nullptr;
			});
		return RandomSelect(candidatePos, Wolf::targetOffspringCount);
	} 

	int Wolf::targetOffspringCount = 1;

	int Wolf::starvationHealthHarm = 1;

	int Wolf::consumptionHealthBenifit = 10;

	int Wolf::maximumHealth = 28;

	int Wolf::initialHealth = 15;

	int Wolf::minimumReproduceHealth = 16;

	std::atomic<int> Wolf::population(0);
} 