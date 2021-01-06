#pragma once
#include <memory>
#include <functional>
#include <vector> 
#include <mutex>
#include "vector2.h"
#include "utils_memory.h"
namespace EcoSim
{
	class Cell;
	class ILivingThing;
	class CellMatrix;

	/// <summary>
	/// 一个地块格子。可以装下一个生物。
	/// </summary>
	class Cell
	{
		friend class CellMatrix;

		/// <summary>
		/// Cell中存放的生物对象指针。null表示Cell为空。
		/// </summary>
		std::shared_ptr<ILivingThing> content = nullptr;

		/// <summary>
		/// 属于哪个地图。
		/// </summary>
		CellMatrix* matrix = nullptr;

	public:

		/// <summary>
		/// Cell的位置。
		/// </summary>
		Vector2 position = Vector2(0, 0);

		/// <summary>
		/// Cell是否被标记为不处理。
		/// </summary>
		bool disabled = false;

		/// <summary>
		/// 构造一个空的Cell。
		/// </summary>
		explicit Cell() {}

		/// <summary>
		/// 构造一个空的Cell，指定位置。
		/// </summary>
		/// <param name="pos"></param>
		explicit Cell(Vector2 pos) : position(pos) {}

		/// <summary>
		/// Cell中存放的生物对象指针。null表示Cell为空。
		/// </summary>
		/// <returns></returns>
		auto Content() const -> std::shared_ptr<ILivingThing> { return content; }

		/// <summary>
		/// 用生物占领此Cell。如果占领前Cell中已有生物，则该生物死亡。
		/// </summary> 
		void OccupyWith(std::shared_ptr<ILivingThing> living_thing);

		/// <summary>
		/// 在Cell中放置生物。当前Cell必须为空。
		/// </summary> 
		void FillWith(std::shared_ptr<ILivingThing> living_thing);

		/// <summary>
		/// 将Cell内的生物移动到另一个Cell中，将导致目的地生物死亡。此Cell不可为空。
		/// </summary> 
		void MoveTo(Cell& dest);

		/// <summary>
		/// 给Cell内的生物造成伤害，可能会杀死此生物。
		/// </summary>  
		void Damage();

		/// <summary>
		/// 给Cell内的生物回复生命。
		/// </summary> 
		void Heal();

		/// <summary>
		/// 不允许给Cell赋值。
		/// </summary> 
		auto operator=(const Cell& cell)->Cell & = delete;

		/// <summary>
		/// 不允许拷贝Cell。
		/// </summary>
		/// <param name="cell"></param>
		Cell(const Cell& cell) = delete;
	};

	/// <summary>
	/// 由Cell组成的矩阵。表示游戏地图。
	/// </summary>
	class CellMatrix
	{
		friend class Cell;
	private:
		/// <summary>
		/// 组成矩阵的Cell。
		/// </summary>
		std::vector<Cell> cells;

		/// <summary>
		/// 每一回合，被更新的Cell的位置都会被存放在这里。
		/// </summary>
		std::vector<Vector2> updatedCellPositions;

		std::mutex mutex_updatedCellPositions;

	public:

		using iterator = std::vector<Cell>::iterator;

		using const_iterator = std::vector<Cell>::const_iterator;

		/// <summary>
		/// 矩阵的宽度。
		/// </summary>
		const int width;

		/// <summary>
		/// 矩阵的高度。
		/// </summary>
		const int height;

		/// <summary>
		/// 构造一个格子矩阵。
		/// </summary>
		CellMatrix(int width, int height);

		/// <summary>
		/// 坐标是否有效。
		/// </summary> 
		auto IsValid(Vector2 coor) const -> bool;

		/// <summary>
		/// 指定位置，返回Cell引用。
		/// </summary> 
		auto Access(Vector2 coor)->Cell&;

		/// <summary>
		/// 指定位置，返回Cell引用。
		/// </summary> 
		auto Access(Vector2 coor) const -> const Cell&;

		/// <summary>
		/// 二维坐标转换为一维坐标
		/// </summary> 
		auto LinearPosition(Vector2 coor) const->size_t;

		/// <summary>
		/// 返回首迭代器。
		/// </summary> 
		auto begin() -> iterator { return cells.begin(); }

		/// <summary>
		/// 返回首迭代器。
		/// </summary> 
		auto begin() const -> const_iterator { return cells.cbegin(); }

		/// <summary>
		/// 返回尾后迭代器。
		/// </summary> 
		auto end() -> iterator { return cells.end(); }

		/// <summary>
		/// 返回尾后迭代器。
		/// </summary> 
		auto end() const -> const_iterator { return cells.cend(); }

		/// <summary>
		/// 返回下一个迭代器。
		/// </summary> 
		auto next(iterator iter) -> iterator { return ++iter; }

		/// <summary>
		/// 返回下一个迭代器。
		/// </summary> 
		auto next(const_iterator iter) const -> const_iterator { return ++iter; }
 
		/// <summary>
		/// 返回位置周围的至多9个格子。越界的不会包括在内。
		/// </summary>
		/// <param name="map">地图</param>
		/// <param name="pos">此格位置</param>
		/// <returns></returns>
		auto SurroundingCells(Vector2 pos) const->std::vector<Vector2>;

		/// <summary>
		/// 返回所有被更新的Cell的位置。
		/// </summary> 
		auto UpdatedCellPositions()->std::vector<Vector2>;

		/// <summary>
		/// 清空Cell更新记录。
		/// </summary> 
		auto ClearCellUpdateRecord() -> void;
		 

		/// <summary>
		/// 取Cell集合对应的坐标集合。指定筛选Cell的条件。
		/// </summary> 
		  auto FilterPositions(std::vector<Vector2>positions
			, std::function<bool(const Cell&)> predicate) const
			-> std::vector<Vector2>;

		/// <summary>
		/// 取Cell集合对应的坐标集合。只有空Cell会被包含。
		/// </summary>
		/// <param name="cells"></param>
		/// <returns></returns>
		  auto FilterEmptyPositions(std::vector<Vector2>positions)   const 
			-> std::vector<Vector2> {
			  return FilterPositions(positions,
				  [](const Cell& cell) {return cell.Content() == nullptr; });
		  }

		/// <summary>
		/// 取Cell集合对应的坐标集合。只有内容是类型是T的会被包含。T必须继承自ILivingThing
		/// </summary> 
		template <typename T>
		  auto FilterPositionsByType(std::vector<Vector2>positions) const
			-> std::vector<Vector2>
		{
			return FilterPositions(positions, [](const Cell& cell)
				{
					return sp_dynamic_cast<T>(cell.Content()) != nullptr;
				});
		}
	};
} // namespace EcoSim
