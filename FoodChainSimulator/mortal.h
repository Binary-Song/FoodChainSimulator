
#ifndef _HAS_HEALTH_H_
#define _HAS_HEALTH_H_

#include "common_includes.h"

namespace FoodChain
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
        /// 有生命的东西都会死。
        /// </summary>
        /// <returns></returns>
        auto virtual Die() -> void {}
    };
} // namespace FoodChain

#endif // _HAS_HEALTH_H_