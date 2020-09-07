#include "game.h"
#include "sheep.h"
#include <algorithm>
namespace FoodChain
{
  
	auto GenerateMap(CellMatrix& map, float density) -> void;
	Game::Game(int width, int height, float density)
		: map(width, height),

		eventCaller_mapUpdated(),
		eventCaller_raiseMessage(),
		eventCaller_raiseProblem(),

		event_mapUpdated(eventCaller_mapUpdated),
		event_raiseMessage(eventCaller_raiseMessage),
		event_raiseProblem(eventCaller_raiseProblem)
	{
		GenerateMap(map, density);
	}

	auto GenerateMap(CellMatrix& map, float density) -> void
	{
		for (auto&& cell : map)
		{
			bool shouldAddLivingThing = rand() % 10000 < density * 10000;
			if (shouldAddLivingThing)
			{
				switch (rand() % 2) // TODO: CHANGE THIS TO 4
				{
				case 0:
					cell.content = std::shared_ptr<Grass>(new Grass());
					break;
				case 1:
					cell.content = std::shared_ptr<Sheep>(new Sheep());
				default:
					break;
				}
			}
		}
	}

	auto Game::NextStep() -> void
	{
		MovePhase();
		ReproducePhase();
		DeathPhase();
	}

	auto Game::ClearDisableStates() -> void
	{
		for (auto&& cell : map)
			cell.disabled = false;
	}

	// 移动阶段
	auto Game::MovePhase() -> void
	{
		std::vector<Vector2> updatedPositions;
		// 将禁用状态全部设为否
		ClearDisableStates();
		for (auto&& cell : map)
		{
			// 如果为空格子或禁用，就跳过
			if (cell.content != nullptr && !cell.disabled)
			{
				// 取得目的地
				Vector2 dest = cell.content->DecideDestination(map, cell.position);
				Cell& destCell = map.Access(dest);
				bool didConsumptionHappen;
				
				// 移动生物
				Cell::MoveContent(cell, destCell, didConsumptionHappen);

				auto mortal = dynamic_cast<IMortal*>(destCell.content.get());
				if (mortal)
				{
					// 吃东西加血,不吃扣血
					if (didConsumptionHappen)
					{  
						mortal->Health() += consumptionEffect;
						if (mortal->Health() > maxHealth)
						{
							mortal->Health() = maxHealth;
						}
					}
					else
					{
						mortal->Health() -= starvationEffect;
					}
				}

				// 禁用已经处理的生物所在格
				destCell.disabled = true;

				// 将起点和终点格子添加到vector中
				updatedPositions.push_back(cell.position);
				if (destCell.position != cell.position) 
					updatedPositions.push_back(destCell.position);

			}  
		}
		eventCaller_mapUpdated.Dispatch(map, updatedPositions); 
	}

	// 繁殖阶段
	auto Game::ReproducePhase() -> void
	{
		std::vector<Vector2> updatedPositions;
		// 将禁用状态全部设为否
		ClearDisableStates();
		for (auto&& cell : map)
		{
			// 如果为空格子或禁用，就跳过
			if (cell.content != nullptr && !cell.disabled)
			{
				// 取得子代位置信息
				auto childrenPositions = cell.content->DecideChildrenLocation(map, cell.position);
				for (auto&& childPosition : childrenPositions)
				{
					updatedPositions.push_back(childPosition);
					// 如果格子已被占，则不能在此繁殖


					// TODO : 可以在此繁殖



					if (map.Access(childPosition).content != nullptr)
					{
						throw "Cannot Reproduce here";
					}
					else
					{
						// 取得子代，放置于指定格子
						Cell& childCell = map.Access(childPosition);
						childCell.content= cell.content->Reproduce();
						// 禁用子代生物所在格
						childCell.disabled = true;
					}
				}
			}
		}
		eventCaller_mapUpdated.Dispatch(map, updatedPositions);
	}

	// 死亡阶段
	auto Game::DeathPhase() -> void
	{
		std::vector<Vector2> updatedPositions;
		for (auto&& cell : map)
		{
			IMortal* mortalBeing = dynamic_cast<IMortal*>(cell.content.get());
			// 如果格子中生物会死
			if (cell.content != nullptr 
				&& mortalBeing != nullptr)
			{
				// 生命为0
				if (mortalBeing->Health() == 0)
				{
					mortalBeing->Die();
					cell.content = nullptr;
					updatedPositions.push_back(cell.position);
				}
			}
		}
		eventCaller_mapUpdated.Dispatch(map, updatedPositions);
	}  
} // namespace FoodChain