#include "game.h"
#include "sheep.h"
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include "wolf.h"
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
				int r = rand() % 100;
				if (r < 90)
				{ 
					cell.SetContent(std::shared_ptr<Grass>(new Grass()));
				}
				else if (r < 99)
				{ 
					cell.SetContent(std::shared_ptr<Sheep>(new Sheep()));
				}
				else 
				{
					cell.SetContent(std::shared_ptr<Wolf>(new Wolf()));
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

	/// <summary>
	/// 处理死亡。
	/// </summary>
	/// <param name="neighborCell"></param>
	inline void HandleDeath(Cell& cell)
	{
		cell.SetContent(nullptr);
	}

	/// <summary>
	/// 处理进食行为。
	/// </summary>
	/// <param name="didEat">是否发生进食</param>
	/// <param name="neighborCell">生物所在格子</param>
	/// <returns></returns>
	static void HandleConsumption(bool didEat, Cell& cell)
	{
		auto mortal = sp_dynamic_cast<IMortal>(cell.Content());
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


	inline auto HandleBirth(CellMatrix& map, Cell& parent_cell) -> void
	{
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
				auto parent_mortal = sp_dynamic_cast<IMortal>(cell.Content());

				// 有年龄概念且年龄过高：无法生育
				if (parent_mortal && parent_mortal->Health() < parent_mortal->MinimumReproduceHealth())
				{
					continue;
				}

				auto parent_gendered = sp_dynamic_cast<IGendered>(cell.Content());
				if (parent_gendered && parent_gendered->Gender() == LivingThingGender::Female)// 有性别概念，则雌性旁边要有雄性才能生育
				{
					const CellMatrix& constMap = map;
					auto surroundings = constMap.SurroundingCells(cell.position); // 找到周围生物

					// 找到周围雄性同种且本回合内未繁殖过的生物 存放在male_positions里
					auto male_positions = ExtractPositionsOfCells(surroundings, [&cell](const Cell& neighborCell)
						{
							auto gendered_neighbor = sp_dynamic_cast<IGendered>(neighborCell.Content());
							return (gendered_neighbor != nullptr // 有性别
								&& gendered_neighbor->Gender() == LivingThingGender::Male // 雄性
								&& neighborCell.disabled == false // 本回合内未繁殖
								&& neighborCell.Content()->TypeIdentifier() == cell.Content()->TypeIdentifier() // 同种
								);
						});

					if (male_positions.size() != 0)
					{
						// 繁殖后的雄性将被禁用，本回合不能再繁殖 
						map.Access(male_positions[rand() % male_positions.size()]).disabled = true;
						// 以雌性为中点，进行繁殖
						HandleBirth(map, cell);
					}
				}

				if (!parent_gendered) // 没有性别概念，则直接生育
				{
					HandleBirth(map, cell);
				}
			}
		}
	}

	Game* Game::activeGame = nullptr;
}



// namespace EcoSim