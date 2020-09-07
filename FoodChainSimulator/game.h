
#ifndef _GAME_H_
#define _GAME_H_

#include "common_includes.h"
#include "cell.h"
#include "grass.h"
#include "event_system.h"

namespace FoodChain
{
 
    /// <summary>
    /// 一个游戏
    /// </summary>
    class Game final
    {
    public:

        /// <summary>
        /// 最大生命值
        /// </summary>
        int maxHealth = 9;

        /// <summary>
        /// 进食增加的血量
        /// </summary>
        int consumptionEffect = 4;

        /// <summary>
        /// 饥饿减少的血量
        /// </summary>
        int starvationEffect = 1;

        /// <summary>
        /// 出生时的血量
        /// </summary>
        int initialHealth = 2;
         

   
 
        /// <summary>
        /// 地图更新事件 
        /// </summary>
        using EventType_MapUpdated = void(const CellMatrix& map, std::vector<Vector2> updatedPositions);
        
  
        /// <summary>
        /// 报告消息事件
        /// </summary>
        using EventType_RaiseMessage = void(std::string message);
 

        /// <summary>
        /// 报告问题事件
        /// </summary>
        using EventType_RaiseProblem = void(std::string problemDescription);

    private:
        
        /// <summary>
        /// 游戏地图
        /// </summary>
        CellMatrix map;
         
        EventCaller<EventType_MapUpdated> eventCaller_mapUpdated;
        EventCaller<EventType_RaiseMessage> eventCaller_raiseMessage; 
        EventCaller<EventType_RaiseProblem> eventCaller_raiseProblem;
 
    public:
 
        /// <summary>
        /// 地图更新事件
        /// </summary>
        Event<EventType_MapUpdated> event_mapUpdated;
 
        /// <summary>
        /// 报告消息事件
        /// </summary>
        Event<EventType_RaiseMessage> event_raiseMessage;
 
        /// <summary>
        /// 报告问题事件
        /// </summary>
        Event<EventType_RaiseProblem> event_raiseProblem;
         
        /// <summary>
        /// 用随机地图创建游戏
        /// </summary>
        /// <param name="width">宽</param>
        /// <param name="height">高</param>
        /// <param name="density">生物密度（0-1）</param>
        Game(int width, int height, float density);
 
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
        auto MovePhase() -> void;

        /// <summary>
        /// 模拟繁殖阶段
        /// </summary>
        /// <returns></returns>
        auto ReproducePhase() -> void;

        /// <summary>
        /// 模拟死亡阶段
        /// </summary>
        /// <returns></returns>
        auto DeathPhase() -> void;
        };
} // namespace FoodChain

#endif // _GAME_H_