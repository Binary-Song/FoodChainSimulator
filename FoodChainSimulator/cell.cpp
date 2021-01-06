#include "cell.h"
#include "living_thing.h"
#include <assert.h>
#include "mortal.h"
#include "utils_memory.h"
#include <array>
namespace EcoSim
{
	void Cell::MoveTo(Cell& dest)
	{
		if (!content)
		{
			int t = 0;
		}
		assert(dest.position != position);

		dest.OccupyWith(content);
		content = nullptr;
		matrix->mutex_updatedCellPositions.lock();
		matrix->updatedCellPositions.push_back(position);
		matrix->mutex_updatedCellPositions.unlock();
	}

	void Cell::Damage()
	{
		assert(content);
		auto mortal = sp_dynamic_cast<IAnimal>(content);
		if (!mortal) return;

		mortal->Health() -= mortal->StarvationHealthHarm();
		if (mortal->Health() <= 0)
		{
			OccupyWith(nullptr);
		} 
	}

	void Cell::Heal()
	{
		assert(content);
		auto mortal = sp_dynamic_cast<IAnimal>(content);
		if (!mortal) return;
 
		mortal->Health() += mortal->ConsumptionHealthBenifit();
		if (mortal->Health() > mortal->MaximumHealth())
		{
			mortal->Health() = mortal->MaximumHealth();
		} 
	}

	void Cell::OccupyWith(std::shared_ptr <ILivingThing> new_content)
	{
		if (content) content->Die();
		content = new_content;
		matrix->mutex_updatedCellPositions.lock();
		matrix->updatedCellPositions.push_back(position);
		matrix->mutex_updatedCellPositions.unlock();
	}

	void Cell::FillWith(std::shared_ptr<ILivingThing> new_content)
	{
		assert(!content);
		content = new_content;
		matrix->mutex_updatedCellPositions.lock();
		matrix->updatedCellPositions.push_back(position);
		matrix->mutex_updatedCellPositions.unlock();
	}

	auto CellMatrix::IsValid(Vector2 pos) const -> bool
	{
		return (pos.x >= 0
			&& pos.x < width
			&& pos.y >= 0
			&& pos.y < height);
	}

	auto CellMatrix::LinearPosition(Vector2 pos) const -> size_t
	{
		return pos.x + pos.y * width;
	}

	auto CellMatrix::Access(Vector2 coor) -> Cell&
	{
		assert(IsValid(coor));
		return cells[coor.x + coor.y * width];
	}

	auto CellMatrix::Access(Vector2 coor) const -> const Cell&
	{
		assert(IsValid(coor));
		return cells[coor.x + coor.y * width];
	}

	auto CellMatrix::SurroundingCells(Vector2 pos) const -> std::vector<Vector2>
	{
		std::vector<Vector2> resultList;
		Vector2 offsets[]{
			Vector2(0, 1), Vector2(1, 0), Vector2(0, -1), Vector2(-1, 0),
			Vector2(1, 1), Vector2(-1, -1), Vector2(-1, 1), Vector2(1, -1)
		};
		for (auto& offset : offsets)
		{
			Vector2 v = pos + offset;
			if (IsValid(v))
			{
				resultList.push_back(v);
			}
		}
		return resultList;
	}

	auto CellMatrix::UpdatedCellPositions() -> std::vector<Vector2>
	{
		return updatedCellPositions;
	}

	auto CellMatrix::ClearCellUpdateRecord() -> void
	{
		updatedCellPositions.clear();
	}

	auto CellMatrix::FilterPositions(std::vector<Vector2>positions
		, std::function<bool(const Cell&)> predicate) const
		-> std::vector<Vector2>
	{
		std::vector<Vector2> result;
		for (auto&& position : positions)
		{
			if (predicate(Access(position)))
			{
				result.push_back(position);
			}
		}
		return result;
	}



	CellMatrix::CellMatrix(int width, int height)
		: width(width),
		height(height),
		cells(width* height)
	{
		int x = 0, y = 0;
		for (auto&& cell : cells)
		{
			cell.matrix = this;
			cell.position.x = x;
			cell.position.y = y;
			++x;
			if (x == width)
			{
				x = 0;
				++y;
			}
		}
	}
}
