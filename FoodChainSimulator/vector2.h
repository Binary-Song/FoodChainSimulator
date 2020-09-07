
#ifndef _VECTOR2_H_
#define _VECTOR2_H_
namespace FoodChain
{
    /// <summary>
    /// 两个整数组成的向量。
    /// </summary>
    struct Vector2 final
    {
    public: 

        int x = 0;
         
        int y = 0;
          
        explicit Vector2(int x = 0, int y = 0)
            : x(x), y(y) {}
         
        auto operator+(Vector2 v2) const -> Vector2
        {
            return Vector2(x + v2.x, y + v2.y);
        }
         
        auto operator==(Vector2 v2) const -> bool
        {
            return x == v2.x && y == v2.y;
        }
          
        auto operator!=(Vector2 v2) const -> bool
        {
            return !((*this) == v2);
        } 
    };
 
} // namespace FoodChain
#endif // _VECTOR2_H_