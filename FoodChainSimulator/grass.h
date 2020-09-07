 
#ifndef _GRASS_H_
#define _GRASS_H_

#include "common_includes.h"
#include "cell.h"
namespace FoodChain
{
    /// <summary>
    /// 草
    /// </summary>
    class Grass final : public ILivingThing
    { 
    public: 

        /// <summary>
        /// 目标后代个数，实际个数受空间限制，不会超过此目标。
        /// </summary>
        static int targetOffspringCount;

        auto DecideDestination(const CellMatrix &map, Vector2 pos) -> Vector2 override { return pos; }

        auto DecideChildrenLocation(const CellMatrix &map, Vector2 pos) -> std::vector<Vector2> override;

        auto DisplayColor() const -> int override {
            using namespace FoodChain::Utilities;
            return ColorCode::Green;
        }
        
        auto Reproduce() -> std::shared_ptr<ILivingThing> override { return std::shared_ptr<ILivingThing>(new Grass()); }
    };
} // namespace FoodChain
#endif // _GRASS_H_