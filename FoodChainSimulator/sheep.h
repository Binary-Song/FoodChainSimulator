 #ifndef _SHEEP_H_
#define _SHEEP_H_

#include "common_includes.h"
#include "cell.h"
#include "mortal.h"
namespace FoodChain
{
    /// <summary>
    /// 羊
    /// </summary>
    class Sheep final : public ILivingThing, public IMortal
    { 
        int health = 2;
    public:

        static int targetOffspringCount;

        auto DecideDestination(const CellMatrix& map, Vector2 pos)->Vector2 override;

        auto DecideChildrenLocation(const CellMatrix& map, Vector2 pos)->std::vector<Vector2> override;
         
        auto DisplayColor() const -> int override;

        auto Reproduce() -> std::shared_ptr<ILivingThing> override{ return std::shared_ptr<ILivingThing>(new Sheep()); } 
 
        auto Health() -> int& override { return health; } 

        virtual ~Sheep() {}
    };
} // namespace FoodChain
#endif // _GRASS_H_