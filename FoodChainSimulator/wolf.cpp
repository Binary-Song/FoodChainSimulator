#include "wolf.h"
#include "grass.h"
#include "utils.h"
#include "sheep.h"
namespace EcoSim
{  
	Wolf::Wolf() :
		health(initialHealth),
		gender(rand() % 2 ? LivingThingGender::Male : LivingThingGender::Female) {}


	auto Wolf::DecideDestination(const CellMatrix& map, Vector2 pos) -> Vector2
	{
		// 找到周围所有羊的位置
		auto surrounds = map.SurroundingCells(pos);
		auto sheepPos = ExtractPositionsOfCellsByContentType<Sheep>(surrounds);
		// 若有，则随机选一个
		if (sheepPos.size() > 0)
			return RandomSelect(sheepPos);

		// 若无，则找到周围所有 格子的位置
		auto okPos = ExtractPositionsOfCells(surrounds);
		
		// 若有，则随机选一个
		if (okPos.size() > 0)
		{
			auto pos = RandomSelect(okPos);
			if (sp_dynamic_cast<Grass>(map.Access(pos).Content()))
			{
				 health -= consumptionHealthBenifit;
			}
			return pos;
		}
			

		// 若无，则不动。
		return pos;
	}

	auto Wolf::DecideChildrenLocation(const CellMatrix& map, Vector2 pos)->std::vector<Vector2> 
	{
		auto surrounds = map.SurroundingCells(pos);
		auto candidatePos = ExtractPositionsOfCells(surrounds, [](const Cell& cell) {
			return sp_dynamic_cast<Grass>(cell.Content()) || cell.Content() == nullptr;
			});
		return RandomSelect(candidatePos, Wolf::targetOffspringCount);
	} 

	int Wolf::targetOffspringCount = 1;

	int Wolf::starvationHealthHarm = 1;

	int Wolf::consumptionHealthBenifit = 15;

	int Wolf::maximumHealth = 25;

	int Wolf::initialHealth = 20;

	int Wolf::minimumReproduceHealth = 22;
}

