
#ifndef _HAS_HEALTH_H_
#define _HAS_HEALTH_H_

#include "common_includes.h"

namespace EcoSim
{
    /// <summary>
    /// 有生命值的东西。
    /// </summary>
    class IMortal
    {
    public:
        virtual ~IMortal() = default;

        /// <summary>
        /// 返回一个生命值引用。通过它可以读写生命值。
        /// </summary>
        /// <returns></returns>
        auto virtual Health() -> int& = 0; 

        /// <summary>
        /// 饥饿减少的生命值。
        /// </summary>
        /// <returns></returns>
        auto virtual StarvationHealthHarm() ->  int = 0;

        /// <summary>
        /// 进食增加的生命值。
        /// </summary>
        /// <returns></returns>
        auto virtual ConsumptionHealthBenifit() -> int = 0;

        /// <summary>
        /// 最大生命值。
        /// </summary>
        /// <returns></returns>
        auto virtual MaximumHealth() -> int = 0;

        /// <summary>
        /// 最小繁殖生命值。
        /// </summary>
        /// <returns></returns>
        auto virtual MinimumReproduceHealth() -> int = 0;

        /// <summary>
        /// 出生时的生命值。
        /// </summary>
        /// <returns></returns>
        auto virtual InitialHealth() -> int = 0;
    };
} // namespace EcoSim

#endif // _HAS_HEALTH_H_