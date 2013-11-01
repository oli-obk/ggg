#import <cmath>
#import "unmanaged_ptr.hpp"

class Position;

typedef unmanaged_ptr<Position> PositionPtr;

class Position
{
public:
    double x, y;
    Position(double x, double y) noexcept
    :x(x),y(y)
    {}

    Position() noexcept
    :Position(0, 0)
    {}

    double distanceSquared(const Position& rhs) const noexcept
    {
        auto diff = *this - rhs;
        return diff.x*diff.x + diff.y*diff.y;
    }
    
    double distanceSquared(const PositionPtr& rhs) const noexcept
    {
        return distanceSquared(*rhs);
    }

    double distance(const Position& rhs) const noexcept
    {
        return std::sqrt(distanceSquared(rhs));
    }
    
    double distance(const PositionPtr& rhs) const noexcept
    {
        return std::sqrt(distanceSquared(rhs));
    }

    Position operator-(const Position& rhs) const noexcept
    {
        return Position(x - rhs.x, y - rhs.y);
    }

    Position operator+(const Position& rhs) const noexcept
    {
        return Position(x + rhs.x, y + rhs.y);
    }
};

namespace std
{
    inline Position abs(const Position& pos) noexcept
    {
        return Position(std::abs(pos.x), std::abs(pos.y));
    }
}
