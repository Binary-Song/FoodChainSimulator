#include "game.h"
#include "sheep.h"
#include <algorithm>
#include <stdlib.h>
#include <time.h>
namespace EcoSim
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
		srand(time(0));
		GenerateMap(map, density);
		activeGame = this;
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
		std::vector<Vector2> updatedPositions;
		MovePhase(updatedPositions);
		ReproducePhase(updatedPositions); 
		eventCaller_mapUpdated.Dispatch(map, updatedPositions);
	}

	auto Game::ClearDisableStates() -> void
	{
		for (auto&& cell : map)
			cell.disabled = false;
	}

	static void HandleConsumption(bool didEat, Cell& cell, std::vector<Vector2>& updatedPositions);

	// 移动阶段
	auto Game::MovePhase(std::vector<Vector2>& updatedPositions) -> void
	{
		// 将禁用状态全部设为否
		ClearDisableStates();
		for (auto&& cell : map)
		{
			// 如果为空格子或禁用，就跳过
			if (cell.content != nullptr && !cell.disabled)
			{ 
				// 取得目的地
				Vector2 dest = cell.content->DecideDestination(map, cell.position);

				if (dest == cell.position) // 没动
				{  
					HandleConsumption(false, cell, updatedPositions);

					// 禁用此格
					cell.disabled = true;
				}
				else // 动了
				{
					Cell& destCell = map.Access(dest);

					if (destCell.content != nullptr) // 说明目的处生物被吃了
					{ 
						HandleConsumption(true, cell, updatedPositions);
					}
					else
					{ 
						HandleConsumption(false, cell, updatedPositions);
					}

					// 禁用两格 
					destCell.disabled = true;
					cell.disabled = true;

					// 添加到更新列表
					updatedPositions.push_back(dest);
					updatedPositions.push_back(cell.position);

					// 移动
					destCell.content = cell.content;
					cell.content = nullptr;
				}
			}
		}
	}

	static void HandleDeath(Cell& cell, std::vector<Vector2>& updatedPositions);
	/// <summary>
	/// 处理进食行为。
	/// </summary>
	/// <param name="didEat">是否发生进食</param>
	/// <param name="cell">生物所在格子</param>
	/// <returns></returns>
	static void HandleConsumption(bool didEat, Cell& cell, std::vector<Vector2>& updatedPositions)
	{
		ILivingThing* livingThing = cell.content.get();
		auto mortal = dynamic_cast<IMortal*>(livingThing);
		if (mortal)
		{
			if (didEat)
			{
				mortal->Health() += mortal->ConsumptionHealthBenifit();
				if (mortal->Health() > mortal->MaximumHealth())
				{
					mortal->Health() = mortal->MaximumHealth();
				} 
			} 
			mortal->Health() -= mortal->StarvationHealthHarm();
			if (mortal->Health() <= 0)
			{
				// 饿死 
				HandleDeath(cell, updatedPositions);
			} 
		} 
	}

	static void HandleDeath(Cell& cell, std::vector<Vector2>& updatedPositions)
	{
		cell.content = nullptr;
		updatedPositions.push_back(cell.position);
	}

	// 繁殖阶段
	auto Game::ReproducePhase(std::vector<Vector2>& updatedPositions) -> void
	{
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

					// 取得子代，放置于指定格子
					Cell& childCell = map.Access(childPosition);

					childCell.content = cell.content->Reproduce();

					auto mortal = dynamic_cast<IMortal*>(cell.content.get());
					if (mortal)
					{
						mortal->Health() = mortal->InitialHealth();
					}

					// 禁用子代生物所在格
					childCell.disabled = true;
				}
			}
		}
	}


	Game* Game::activeGame = nullptr;
} // namespace EcoSim