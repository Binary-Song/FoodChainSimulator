#include "cell.h"


/// <summary>
/// 取得方阵中的一个格子。
/// </summary>
/// <param name="coor">坐标</param>
/// <returns></returns>

auto EcoSim::CellMatrix::Access(Vector2 coor) const -> const Cell&
{
	assert(IsValid(coor));
	return cells[coor.x + coor.y * width];
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

auto EcoSim::Cell::SetContent(std::shared_ptr<ILivingThing> new_content) -> void
{ 
	content = new_content;
	matrix->updatedCellPositions.push_back(position);
}
