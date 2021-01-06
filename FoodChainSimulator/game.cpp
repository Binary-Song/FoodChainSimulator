#include <algorithm>
#include <stdlib.h>
#include <utility> 
#include <thread>
#include <time.h>
#include <functional>
#include "game.h"
#include "sheep.h"
#include "wolf.h"
#include "random_gen.h"
namespace EcoSim
{

	auto GenerateMap(CellMatrix& map, float density) -> void;

	Game::Game(int width, int height, float density)
		: map(width, height),
		event_mapUpdated(eventCaller_mapUpdated)
	{
		GenerateMap(map, density);
		activeGame = this;
		mapPieces = SplitMap();
	}

	auto GenerateMap(CellMatrix& map, float density) -> void
	{
		for (auto&& cell : map)
		{
			bool shouldAddLivingThing = Rand() % 10000 < density * 10000;
			if (shouldAddLivingThing)
			{
				int r = Rand() % 100;
				if (r < 90)
				{
					cell.FillWith(std::shared_ptr<Grass>(new Grass()));
				}
				else if (r < 99)
				{
					cell.FillWith(std::shared_ptr<Sheep>(new Sheep()));
				}
				else
				{
					cell.FillWith(std::shared_ptr<Wolf>(new Wolf()));
				}
			}
		}
	}

	auto Game::SplitMap() -> std::vector<MapSegment>
	{
		std::vector<MapSegment> result;
		if (threadMinimumLoad < 0)
		{
			result = { MapSegment(map.begin(), map.end()) };
			return result;
		}
		int seg_cell_count = std::max((threadMinimumLoad + map.width - 1) / map.width, 6)
			* map.width;

		auto begin = map.begin();
		while (begin <= map.end() - seg_cell_count)
		{
			result.push_back(MapSegment(begin, begin + seg_cell_count));
			begin += seg_cell_count;
		}
		if (begin != map.end())
		{
			result.push_back(MapSegment(begin, map.end()));
		}

		return result;
	}

	void Game::DispatchThreadsForPhase(bool odd
		, std::function<void(CellMatrix::iterator, CellMatrix::iterator)> stdfunc)
	{
		if (mapPieces.size() == 1)
		{
			if (odd)  stdfunc(mapPieces[0].begin, mapPieces[0].end);
		}
		else
		{
			std::vector<std::thread> threads;
			for (auto& seg : mapPieces)
			{
				if (odd)
				{
					threads.push_back(std::thread(stdfunc, seg.begin, seg.end));
				}
				odd = !odd;
			}
			for (auto& thread : threads) thread.join(); 
		}
 	}

	auto Game::NextStep() -> void
	{
		map.ClearCellUpdateRecord();  
		DispatchThreadsForPhase(true, std::bind(&Game::MovePhase, this, std::placeholders::_1, std::placeholders::_2));
		DispatchThreadsForPhase(false, std::bind(&Game::MovePhase, this, std::placeholders::_1, std::placeholders::_2));
		DispatchThreadsForPhase(true, std::bind(&Game::ReproducePhase, this, std::placeholders::_1, std::placeholders::_2));
		DispatchThreadsForPhase(false, std::bind(&Game::ReproducePhase, this, std::placeholders::_1, std::placeholders::_2));

		eventCaller_mapUpdated.Dispatch(map, map.UpdatedCellPositions());
	}

	auto Game::ClearDisableStates() -> void
	{
		for (auto&& cell : map)
			cell.disabled = false;
	}

	// 移动阶段
	auto Game::MovePhase(CellMatrix::iterator begin, CellMatrix::iterator end) -> void
	{
		// 将禁用状态全部设为否
		ClearDisableStates();
		for (auto loop_iter = begin; loop_iter != end; ++loop_iter)
		{
			Cell& cell = *loop_iter;
			// 如果为空格子或禁用，就跳过
			if (cell.Content() && !cell.disabled)
			{
				// 取得目的地
				Vector2 dest = cell.Content()->DecideDestination(map, cell.position);

				if (dest == cell.position) // 没动
				{ 
					cell.Damage();
					// 禁用此格
					cell.disabled = true;
				}
				else // 动了
				{
					Cell& destCell = map.Access(dest);

					auto anim = sp_dynamic_cast<IAnimal>(cell.Content());
					if (destCell.Content() && anim && destCell.Content()->TypeIdentifier() == anim->Food()) {
						cell.Heal();
					}
					else {
						cell.Damage();
					}

					// 禁用两格 
					destCell.disabled = true;
					cell.disabled = true;

					// 移动
					if(cell.Content()) cell.MoveTo(destCell);
				}
			}
		}
	}

  

	auto Game::HandleBirth(CellMatrix& map, Cell& parent_cell) -> void
	{
		// 取得子代位置信息
		auto childrenPositions = parent_cell.Content()->DecideChildrenLocation(map, parent_cell.position);
		for (auto&& childPos : childrenPositions)
		{
			Cell& child_cell = map.Access(childPos);
			auto child = parent_cell.Content()->Reproduce();
			child_cell.OccupyWith(child);  
			// 禁用子代生物所在格
			child_cell.disabled = true;
		}
	}


	// 繁殖阶段
	auto Game::ReproducePhase(CellMatrix::iterator begin, CellMatrix::iterator end) -> void
	{
		// 将禁用状态全部设为否
		ClearDisableStates();
		for (auto loop_iter = begin; loop_iter != end; ++loop_iter)
		{
			Cell& cell = *loop_iter;
			// 如果为空格子或禁用，就跳过
			if (cell.Content() && !cell.disabled)
			{
				auto parent_mortal = sp_dynamic_cast<IAnimal>(cell.Content());

				// 生命值过低，无法生育。
				if (parent_mortal && parent_mortal->Health() < parent_mortal->MinimumReproduceHealth())
				{
					continue;
				}

				auto parent_gendered = sp_dynamic_cast<IAnimal>(cell.Content());
				if (parent_gendered && parent_gendered->Gender() == LivingThingGender::Female)// 有性别概念，则雌性旁边要有雄性才能生育
				{ 
					auto surroundings =  (map.SurroundingCells(cell.position)); // 找到周围生物

					// 找到周围雄性同种且本回合内未繁殖过的生物 存放在male_positions里
					auto male_positions = map.FilterPositions 
					 
					(surroundings, [&cell](const Cell& neighborCell)
						{
							auto gendered_neighbor = sp_dynamic_cast<IAnimal>(neighborCell.Content());
							return (gendered_neighbor != nullptr												// 有性别
								&& gendered_neighbor->Gender() == LivingThingGender::Male						// 雄性
								&& neighborCell.disabled == false												// 本回合内未繁殖
								&& neighborCell.Content()->TypeIdentifier() == cell.Content()->TypeIdentifier() // 同种
								);
						});

					if (male_positions.size() != 0)
					{
						// 繁殖后的雄性将被禁用，本回合不能再繁殖 
						map.Access(male_positions[Rand() % male_positions.size()]).disabled = true;
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
	int Game::threadMinimumLoad = -1;
	int Game::cycleTime = 0;
}



// namespace EcoSim