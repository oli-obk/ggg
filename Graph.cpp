#include "Graph.hpp"

optional<Node&> Graph::GetNearestNode(Position pos) noexcept
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


Node& Graph::CreateNode(Position pos)
{
    for (auto& node:Nodes) {
        if (node) continue;
        node.emplace(pos);
        NodeCount++;
        return *node;
    }
    throw TooManyNodesException("there are too many nodes already, increase the amount during compilation");
}

void Graph::DeleteNode(Node& node)
{
    for (auto& n:Nodes) {
        if (!n) continue;
        if (*n == node) {
            n.clear();
            NodeCount--;
            return;
        }
    }
    throw UnknownNodeException("tried to delete a node that does not belong to this graph");
}
