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

size_t Graph::getEdgeCount() const noexcept
{
    size_t ret;
    for (auto& node:nodes) {
        if (!node) continue;
        ret += node -> getEdgeCount();
    }
    // divisible by 2? we are getting double edge count
    assert((ret & 1) == 0);
    return ret>>1;
}

std::vector<const Node*> Graph::getNodes() const noexcept
{
    std::vector<const Node*> ret;
    ret.reserve(nodeCount);
    for (auto& node:nodes) {
        if (!node) continue;
        ret.push_back(&*node);
    }
    return ret;
}

