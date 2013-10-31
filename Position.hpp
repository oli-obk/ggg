#import <cmath>

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

    double distanceSquared(const Position& rhs) noexcept
    {
        auto diff = *this - rhs;
        return diff.x*diff.x + diff.y*diff.y;
    }

    Position operator-(const Position& rhs) noexcept
    {
        return Position(x - rhs.x, y - rhs.y);
    }

    Position operator+(const Position& rhs) noexcept
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
