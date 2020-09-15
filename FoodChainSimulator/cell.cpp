#include "cell.h"
#include "living_thing.h"
#include <assert.h>
#include "mortal.h"
#include "utils_memory.h"
namespace EcoSim
{
	void Cell::MoveTo(Cell& dest)
	{
		assert(content);
		OccupyWith(content);
		content = nullptr;
	}

	void Cell::Damage()
	{
		assert(content);
		auto mortal = sp_dynamic_cast<IAnimal>( content );
		if (mortal)
		{
			mortal->Health() -= mortal->StarvationHealthHarm();
			if (mortal->Health() <= 0)
			{
				OccupyWith(nullptr);
			}
		}
	}

	void Cell::Heal()
	{
		assert(content);
		auto mortal = sp_dynamic_cast<IAnimal>(content);
		if (mortal)
		{
			mortal->Health() += mortal->ConsumptionHealthBenifit();
			if (mortal->Health() > mortal->MaximumHealth())
			{
				mortal->Health() = mortal->MaximumHealth();
			}
		}
	}

	void Cell::OccupyWith(std::shared_ptr <ILivingThing> new_content)
	{
		if(content) content->Die(); 
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

	auto CellMatrix::SurroundingCells(Vector2 pos) -> std::vector<Cell*>
	{
		std::vector<Cell*> resultList;
		Vector2 offsets[]{
			Vector2(0, 1), Vector2(1, 0), Vector2(0, -1), Vector2(-1, 0),
			Vector2(1, 1), Vector2(-1, -1), Vector2(-1, 1), Vector2(1, -1)
		};
		for (auto& offset : offsets)
		{
			if (IsValid(pos + offset))
			{
				Cell& cell = Access(pos + offset);
				resultList.push_back(&cell);
			}
		}
		return resultList;
	}

	auto CellMatrix::SurroundingCells(Vector2 pos) const-> const std::vector<Cell*>
	{
		return SurroundingCells(pos);
	}

	auto CellMatrix::UpdatedCellPositions() -> std::vector<Vector2>
	{
		return updatedCellPositions;
	}

	auto CellMatrix::ClearCellUpdateRecord() -> void
	{
		updatedCellPositions.clear();
	}

	auto CellMatrix::ExtractCellPositions(const std::vector<  Cell*> &cells) -> std::vector<Vector2>
	{
		std::vector<Vector2> result;
		for (auto&& cell : cells)
		{
			result.push_back(cell->position);
		}
		return result;
	}

	auto CellMatrix::ExtractCellPositions(const std::vector<   Cell*>& cells
		, std::function<bool(const Cell&)> predicate)
		->std::vector<Vector2>
	{
		std::vector<Vector2> result;
		for (auto&& cell : cells)
		{
			if (predicate(*cell))
			{
				result.push_back(cell->position);
			}
		}
		return result;
	}

	auto CellMatrix::ExtractEmptyCellPositions(const std::vector<  Cell*> &cells)
		->std::vector<Vector2>
	{
		return ExtractCellPositions(cells,
			[](const Cell& cell) {return cell.Content() == nullptr; });
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

	auto ExtractPositionsOfCells(const std::vector<  Cell*>& cells) -> std::vector<Vector2>
	{
		std::vector<Vector2> result;
		for (auto&& cell : cells)
		{
			result.push_back(cell->position);
		}
		return result;
	}
}
