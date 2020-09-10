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
					cell.SetContent(std::shared_ptr<Grass>(new Grass()));
					break;
				case 1:
					cell.SetContent(std::shared_ptr<Sheep>(new Sheep()));
				default:
					break;
				}
			}
		}
	}

	auto Game::NextStep() -> void
	{
		map.ClearCellUpdateRecord();
		MovePhase();
		ReproducePhase();
		eventCaller_mapUpdated.Dispatch(map, map.UpdatedCellPositions());
	}

	auto Game::ClearDisableStates() -> void
	{
		for (auto&& cell : map)
			cell.disabled = false;
	}

	static void HandleConsumption(bool didEat, Cell& cell);

	// 移动阶段
	auto Game::MovePhase() -> void
	{
		// 将禁用状态全部设为否
		ClearDisableStates();
		for (auto&& cell : map)
		{
			// 如果为空格子或禁用，就跳过
			if (cell.Content() != nullptr && !cell.disabled)
			{
				// 取得目的地
				Vector2 dest = cell.Content()->DecideDestination(map, cell.position);

				if (dest == cell.position) // 没动
				{
					HandleConsumption(false, cell);

					// 禁用此格
					cell.disabled = true;
				}
				else // 动了
				{
					Cell& destCell = map.Access(dest);

					if (destCell.Content() != nullptr) // 说明目的处生物被吃了
					{
						HandleConsumption(true, cell);
					}
					else
					{
						HandleConsumption(false, cell);
					}

					// 禁用两格 
					destCell.disabled = true;
					cell.disabled = true;

					// 移动
					destCell.SetContent(cell.Content());
					cell.SetContent(nullptr);
				}
			}
		}
	}

	static void HandleDeath(Cell& cell);
	/// <summary>
	/// 处理进食行为。
	/// </summary>
	/// <param name="didEat">是否发生进食</param>
	/// <param name="cell">生物所在格子</param>
	/// <returns></returns>
	static void HandleConsumption(bool didEat, Cell& cell)
	{
		auto mortal = dynamic_cast<IMortal*>(cell.Content().get());
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
				HandleDeath(cell);
			}
		}
	}

	static void HandleDeath(Cell& cell)
	{
		cell.SetContent(nullptr);
	}

	// 繁殖阶段
	auto Game::ReproducePhase() -> void
	{
		// 将禁用状态全部设为否
		ClearDisableStates();
		for (auto&& cell : map)
		{
			// 如果为空格子或禁用，就跳过
			if (cell.Content() != nullptr && !cell.disabled)
			{
				auto parent_mortal = std::dynamic_pointer_cast<IMortal>(cell.Content());

				if (parent_mortal)// 如果亲代有生命值
				{
					if (parent_mortal->Health() >= parent_mortal->MinimumReproduceHealth())
					{
						Cell& parent_cell = cell; // 给cell取个别名
						// 取得子代位置信息
						auto childrenPositions = parent_cell.Content()->DecideChildrenLocation(map, parent_cell.position);
						for (auto&& childPos : childrenPositions)
						{
							Cell& child_cell = map.Access(childPos);

							child_cell.SetContent(parent_cell.Content()->Reproduce());
							// 禁用子代生物所在格
							child_cell.disabled = true;
						}
					}
				}
				else// 如果亲代无生命值
				{
					Cell& parent_cell = cell; // 给cell取个别名
					// 取得子代位置信息
					auto childrenPositions = parent_cell.Content()->DecideChildrenLocation(map, parent_cell.position);
					for (auto&& childPos : childrenPositions)
					{
						Cell& child_cell = map.Access(childPos);

						child_cell.SetContent(parent_cell.Content()->Reproduce());
						// 禁用子代生物所在格
						child_cell.disabled = true;
					}
				}
			}
		}
	}


	Game* Game::activeGame = nullptr;
} // namespace EcoSim