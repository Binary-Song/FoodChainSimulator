#ifndef _CELL_H_
#define _CELL_H_

#include "common_includes.h"
#include <type_traits>

namespace FoodChain
{
	class Cell;
	class ILivingThing;
	class CellMatrix;

	/// <summary>
	/// 游戏地图的一个格子。
	/// </summary>
	class Cell final
	{
	public:

		/// <summary>
		/// 指向格子中存储的内容，nullptr表示空格子。
		/// </summary>
		std::shared_ptr<ILivingThing> content = nullptr;

		/// <summary>
		/// 格子的位置 
		/// </summary>
		Vector2 position = Vector2(0, 0);

		/// <summary>
		/// 禁用的格子不会被处理。
		/// </summary>
		bool disabled = false;

		/// <summary>
		/// 创建一个空格子。
		/// </summary>
		explicit Cell() {}

		/// <summary>
		/// 创建一个空格子。
		/// </summary>
		/// <param name="pos">格子位置</param>
		explicit Cell(Vector2 pos) : position(pos) {}

		/// <summary>
		/// 将格子的内容移动到另一个格子中去。源格子的内容将变为空，目标格子的内容将被覆盖。
		/// </summary>
		/// <param name="src">源</param>
		/// <param name="dest">目标</param>
		/// <returns></returns>
		auto static MoveContent(Cell& src, Cell& dest, bool& didConsumptionHappen) -> void;

		auto operator=(Cell cell)->Cell & = delete;

		Cell(const Cell& cell) = delete;
	};

	/// <summary>
   /// Cell 中包含的内容，一个生物。
   /// </summary>
	class ILivingThing
	{
	public:
		virtual ~ILivingThing() {}

		/// <summary>
		/// 移动阶段，决定移动的目的地。移动到非空格子内表示捕食。
		/// </summary>
		/// <param name="map">地图</param>
		/// <param name="pos">此格位置</param>
		/// <returns></returns>
		auto virtual DecideDestination(const CellMatrix& map, Vector2 pos)->Vector2 = 0;

		/// <summary>
		/// 繁殖阶段，决定在哪个位置生成子代。 
		/// </summary>
		/// <param name="map">地图</param>
		/// <param name="pos">此格位置</param>
		/// <returns></returns>
		auto virtual DecideChildrenLocation(const CellMatrix& map, Vector2 pos)->std::vector<Vector2> = 0;

		/// <summary>
		/// 繁殖阶段，构造子代对象。
		/// </summary>
		/// <param name="map">地图</param>
		/// <param name="pos">此格位置</param>
		/// <returns></returns>
		auto virtual Reproduce()->std::shared_ptr<ILivingThing> = 0;


		/// <summary>
		/// 显示颜色。
		/// </summary>
		/// <returns></returns>
		auto virtual DisplayColor() const-> int = 0; 
	};

	/// <summary>
	/// 一个Cell组成的方阵，用于表示地图。
	/// </summary>
	class CellMatrix
	{
	public:
		/// <summary>
		/// 宽度。
		/// </summary>
		const int width;

		/// <summary>
		/// 高度。
		/// </summary>
		const int height;

		/// <summary>
		///  指定高和宽，构造新的方阵。
		/// </summary>
		/// <param name="width">宽</param>
		/// <param name="height">高</param>
		CellMatrix(int width, int height);

		/// <summary>
		/// 判定一个坐标是否合法。
		/// </summary>
		/// <param name="coor">坐标</param>
		/// <returns></returns>
		auto IsValid(Vector2 coor) const -> bool
		{
			return (coor.x >= 0 && coor.x < width&& coor.y >= 0 && coor.y < height);
		}

		/// <summary>
		/// 取得方阵中的一个格子。
		/// </summary>
		/// <param name="coor">坐标</param>
		/// <returns></returns>
		auto Access(Vector2 coor) -> Cell&
		{
			assert(IsValid(coor));
			return cells[coor.x + coor.y * width];
		}

		/// <summary>
		/// 取得方阵中的一个格子。
		/// </summary>
		/// <param name="coor">坐标</param>
		/// <returns></returns>
		auto Access(Vector2 coor) const -> const Cell&
		{
			assert(IsValid(coor));
			return cells[coor.x + coor.y * width];
		}

		auto begin() -> std::vector<FoodChain::Cell>::iterator { return cells.begin(); }

		auto begin() const -> std::vector<FoodChain::Cell>::const_iterator { return cells.cbegin(); }

		auto end() -> std::vector<FoodChain::Cell>::iterator { return cells.end(); }

		auto end() const -> std::vector<FoodChain::Cell>::const_iterator { return cells.cend(); }

		auto next(std::vector<FoodChain::Cell>::iterator iter) -> std::vector<FoodChain::Cell>::iterator { return ++iter; }

		auto next(std::vector<FoodChain::Cell>::const_iterator iter) const -> std::vector<FoodChain::Cell>::const_iterator { return ++iter; }


		/// <summary>
		/// 返回位置周围的至多9个格子。越界的不会包括在内。
		/// </summary>
		/// <param name="map">地图</param>
		/// <param name="pos">此格位置</param>
		/// <returns></returns>
		auto SurroundingCells(Vector2 pos)->std::vector<std::reference_wrapper<Cell>>;

		/// <summary>
		/// 返回位置周围的至多9个格子。越界的不会包括在内。
		/// </summary>
		/// <param name="map">地图</param>
		/// <param name="pos">此格位置</param>
		/// <returns></returns>
		auto SurroundingCells(Vector2 pos) const->std::vector<std::reference_wrapper<const Cell>>;

	private:
		std::vector<Cell> cells;
	};

	namespace Utilities {
		/// <summary>
		/// 返回满足条件的格子的位置。
		/// </summary> 
		inline auto ExtractPositionsOfCells(const std::vector<std::reference_wrapper<const Cell>>& cells, bool predicate(const Cell& cell)) -> std::vector<Vector2>
		{
			std::vector<Vector2> result;
			for (auto&& cell : cells)
			{
				if (predicate(cell.get()))
				{
					result.push_back(cell.get().position);
				}
			}
			return result;
		}

		inline auto ExtractPositionsOfEmptyCells(const std::vector<std::reference_wrapper<const Cell>>& cells) -> std::vector<Vector2>
		{
			return ExtractPositionsOfCells(cells, [](const Cell& cell) {return cell.content == nullptr; });
		}

		template<typename T>
		auto ExtractPositionsOfCellsByContentType(const std::vector<std::reference_wrapper<const Cell>>& cells) -> std::vector<Vector2>
		{
			static_assert(std::is_base_of<ILivingThing, T>::value, "T must inherit from ILivingThing.");
			return ExtractPositionsOfCells(cells, [](const Cell& cell)
				{
					return dynamic_cast<T*>(cell.content.get()) != nullptr;
				});
		}
	}
} // namespace FoodChain
#endif // _CELL_H_