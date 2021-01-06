
#ifndef _GAME_H_
#define _GAME_H_

#include "common_includes.h"
#include "cell.h"
#include "grass.h"
#include "event_system.h"
#include <mutex>
#include "random_gen.h"

namespace EcoSim
{

    /// <summary>
    /// 一个游戏
    /// </summary>
    class Game final
    {
    public:
        
        /// <summary>
        /// 从begin开始到end结束的一块地图。
        /// </summary>
        struct MapSegment
        {
            CellMatrix::iterator begin, end;

            MapSegment(CellMatrix::iterator begin, CellMatrix::iterator end)
                : begin(begin), end(end) {}
        }; 

        /// <summary>
        /// 当前活动的游戏对象。
        /// </summary>
        static Game *activeGame; 

        static int cycleTime;
 
        /// <summary>
        /// 地图更新事件类型
        /// </summary>
        using EventType_MapUpdated 
            = void(const CellMatrix& map, std::vector<Vector2> updatedPositions);
       
    private:
        
        /// <summary>
        /// 游戏地图
        /// </summary>
        CellMatrix map;
        
        /// <summary>
        /// 地图更新事件的调用器。
        /// </summary>
        EventCaller<EventType_MapUpdated> eventCaller_mapUpdated;
        
        /// <summary>
        /// 保存的地图切片方案。
        /// </summary>
        std::vector <MapSegment> mapPieces;
 
    public:
        /// <summary>
        /// 单个线程处理的最少格子数
        /// </summary>
        static int threadMinimumLoad;

        /// <summary>
        /// 地图更新事件
        /// </summary>
        Event<EventType_MapUpdated> event_mapUpdated;
          
        /// <summary>
        /// 用随机地图创建游戏
        /// </summary>
        /// <param name="width">宽</param>
        /// <param name="height">高</param>
        /// <param name="density">生物密度（0-1）</param>
        Game(int width, int height, float density );
    
        /// <summary>
        /// 继续模拟下一个周期
        /// </summary>
        /// <returns></returns>
        auto NextStep() -> void; 

        /// <summary>
        /// 返回当前地图。
        /// </summary>
        /// <returns></returns>
        auto Map() -> const CellMatrix & { return map; }
         
    private:
        /// <summary>
        /// 将所有格子都设置为可用
        /// </summary>
        /// <returns></returns>
        auto ClearDisableStates() -> void; 
         
        /// <summary>
        /// 模拟移动阶段
        /// </summary>
        /// <returns></returns>
        auto MovePhase(CellMatrix::iterator begin, CellMatrix::iterator end) -> void;

        /// <summary>
        /// 模拟繁殖阶段
        /// </summary>
        /// <returns></returns>
        auto ReproducePhase(CellMatrix::iterator begin, CellMatrix::iterator end) -> void;
        
        /// <summary>
        /// 将地图切分成小块，以便分配给多个线程处理。
        /// </summary>
        /// <returns></returns>
        auto SplitMap() -> std::vector<MapSegment>;
        void HandleBirth(CellMatrix& map, Cell& parent_cell);
        void HandleConsumption(bool didEat, Cell& cell); 
        void DispatchThreadsForPhase(bool odd
            , std::function<void(CellMatrix::iterator, CellMatrix::iterator)> stdfunc);

 };
} // namespace EcoSim

#endif // _GAME_H_