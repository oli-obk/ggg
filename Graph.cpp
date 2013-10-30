#include "Graph.hpp"

optional<Node&> Graph::GetNearestNode(Position pos)
{
    optional<Node&> found;
    double dist;
    for (auto& node:Nodes) {
        if (!node) continue;
        auto _dist = pos.distanceSquared(*node);
        if (!found) {
            found.emplace(*node);
            dist = _dist;
            continue;
        }
        if (_dist < dist) {
            found.emplace(*node);
            dist = _dist;
        }
    }
    return found;
}
