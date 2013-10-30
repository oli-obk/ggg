#ifndef GGG_POSITION_HPP
#define GGG_POSITION_HPP

#include <cmath>

class Position
{
public:
    double x, y;
    Position(double x, double y):x(x),y(y) {}
    Position():Position(0, 0){}
    double distanceSquared(const Position& rhs)
    {
        auto diff = *this - rhs;
        return diff.x*diff.x + diff.y*diff.y;
    }
    Position operator-(const Position& rhs)
    {
        return Position(x - rhs.x, y - rhs.y);
    }
    Position operator+(const Position& rhs)
    {
        return Position(x + rhs.x, y + rhs.y);
    }
};

namespace std
{
    inline Position abs(const Position& pos)
    {
        return Position(std::abs(pos.x), std::abs(pos.y));
    }
}

#endif //GGG_POSITION_HPP
