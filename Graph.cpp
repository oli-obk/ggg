#include "Graph.hpp"

optional<Node&> Graph::getNearestNode(Position pos) noexcept
{
    optional<Node&> found;
    double dist;
    for (auto& node:nodes) {
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


Node& Graph::createNode(Position pos)
{
    for (auto& node:nodes) {
        if (node) continue;
        node.emplace(pos);
        nodeCount++;
        return *node;
    }
    throw TooManyNodesException("there are too many nodes already, increase the amount during compilation");
}

void Graph::deleteNode(Node& node)
{
    for (auto& n:nodes) {
        if (!n) continue;
        if (*n == node) {
            n.clear();
            nodeCount--;
            return;
        }
    }
    throw UnknownNodeException("tried to delete a node that does not belong to this graph");
}
