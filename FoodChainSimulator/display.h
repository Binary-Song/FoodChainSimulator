#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common_includes.h"
#include "cell.h"
#include "game.h"
#include <mutex>
#include <memory>
namespace EcoSim
{
	class Display
	{
		static int Width;
		static int Height;
	public:
		 
		static int displayMode;

		/// <summary>
		/// 初始化显示设置
		/// </summary>
		auto static Initialize(Game& game) -> void;

		/// <summary>
		/// 重新绘制整个地图。
		/// </summary>
		/// <param name="map">地图的信息</param>
		/// <returns></returns>
		auto static DrawMap(const CellMatrix& map) -> void;

		/// <summary>
		/// 绘制地图中的一格。
		/// </summary>
		/// <param name="cell"></param>
		/// <returns></returns>
		auto static BufferCell(const Cell& cell) -> void;

	private:
		/// <summary>
		/// 响应游戏的地图更新事件。
		/// </summary>
		/// <param name="updatedPositions"></param>
		/// <returns></returns>
		void static MapUpdatedEventHandler(const CellMatrix& map, std::vector<Vector2> updatedPositions);


		void static DisplayBuffer();
	};
} // namespace EcoSim

#endif // _DISPLAY_H_