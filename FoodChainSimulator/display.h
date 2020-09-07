#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common_includes.h"
#include "cell.h"
#include "game.h"
namespace FoodChain
{
    class Display
    {
    public:
        
        /// <summary>
        /// 初始化显示设置
        /// </summary>
        auto static Initialize(Game& game) -> void;

        /// <summary>
        /// 重新绘制整个地图。
        /// </summary>
        /// <param name="map">地图的信息</param>
        /// <returns></returns>
        auto static DrawMap(const CellMatrix &map) -> void;
        
        /// <summary>
        /// 绘制地图中的一格。
        /// </summary>
        /// <param name="cell"></param>
        /// <returns></returns>
        auto static DrawCell(const Cell& cell) -> void;

        /// <summary>
        /// 响应游戏的地图更新事件。
        /// </summary>
        /// <param name="updatedPositions"></param>
        /// <returns></returns>
        auto static MapUpdatedEventHandler(const CellMatrix& map, std::vector<Vector2> updatedPositions) -> void;
         
    };
} // namespace FoodChain

#endif // _DISPLAY_H_