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
    
    Position operator-(const PositionPtr rhs) const noexcept
    {
        return *this - *rhs;
    }

    Position operator+(const PositionPtr rhs) const noexcept
    {
        return *this + *rhs;
    }
    
    Position operator*(double val) const noexcept
    {
        return Position(x*val, y*val);
    }
    
    Position operator/(double val) const
    {
        return Position(x/val, y/val);
    }
};

inline Position operator*(double val, const Position& pos) noexcept
{
    return pos*val;
}

inline Position operator*(double val, const PositionPtr pos) noexcept
{
    return (*pos)*val;
}

inline Position operator+(const PositionPtr lhs, const PositionPtr rhs) noexcept
{
    return (*lhs)+(*rhs);
}

inline Position operator-(const PositionPtr lhs, const PositionPtr rhs) noexcept
{
    return (*lhs)-(*rhs);
}

inline Position operator+(const PositionPtr lhs, const Position& rhs) noexcept
{
    return (*lhs)+rhs;
}

inline Position operator-(const PositionPtr lhs, const Position& rhs) noexcept
{
    return (*lhs)-rhs;
}

namespace std
{
    inline Position abs(const Position& pos) noexcept
    {
        return Position(std::abs(pos.x), std::abs(pos.y));
    }
}
