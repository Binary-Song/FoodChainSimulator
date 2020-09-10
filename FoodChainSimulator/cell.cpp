#include "cell.h"

 
auto EcoSim::Cell::MoveContent(Cell& src, Cell& dest, bool& didConsumptionHappen) -> void
{
	// 动
	if (src.position != dest.position)
	{
		didConsumptionHappen = (dest.content != nullptr);
		dest.content = src.content;
		src.content = nullptr; 
	}
	// 静
	else
	{
		didConsumptionHappen = false;
	}
}


auto EcoSim::CellMatrix::SurroundingCells(Vector2 pos) -> std::vector<std::reference_wrapper<Cell>>
{
	std::vector<std::reference_wrapper<Cell>> resultList;
	Vector2 offsets[]{ Vector2(0, 1), Vector2(1, 0), Vector2(0, -1), Vector2(-1, 0),Vector2(1, 1), Vector2(-1, -1), Vector2(-1, 1), Vector2(1, -1) };
	for (auto& offset : offsets)
	{
		if (IsValid(pos + offset))
		{
			Cell& cell = Access(pos + offset);
			resultList.push_back(cell);
		}
	}
	return resultList;
}

auto EcoSim::CellMatrix::SurroundingCells(Vector2 pos)const -> std::vector<std::reference_wrapper<const Cell>>
{
	std::vector<std::reference_wrapper<const Cell>> resultList;
	Vector2 offsets[]{ Vector2(0, 1), Vector2(1, 0), Vector2(0, -1), Vector2(-1, 0),Vector2(1, 1), Vector2(-1, -1), Vector2(-1, 1), Vector2(1, -1) };
	for (auto& offset : offsets)
	{
		if (IsValid(pos + offset))
		{ 
			resultList.push_back(Access(pos + offset));
		}
	}
	return resultList;
}
 
 
EcoSim::CellMatrix::CellMatrix(int width, int height)
	: width(width),
	height(height),
	cells(width* height)
{
	int x = 0, y = 0;
	for (auto&& cell : cells)
	{
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