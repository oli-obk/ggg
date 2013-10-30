#include "Graph.hpp"
#include "make_unique.hpp"
#include <limits>

Node& Graph::getNearestNode(Position pos)
{
    if (nodes.empty()) {
        throw ThereAreNoNodesException("can't get nearest node without any nodes");
    }
    Node* found;
    double dist = std::numeric_limits<double>::max();
    for (auto& node:nodes) {
        auto _dist = pos.distanceSquared(*node);
        if (_dist < dist) {
            found = node.get();
            dist = _dist;
        }
    }
    assert(found);
    return *found;
}


Node& Graph::createNode(Position pos)
{
    nodes.push_back(std::make_unique<Node>(pos));
    return *nodes.back();
}

void Graph::deleteNode(Node& node)
{
    for (auto& n:nodes) {
        if (*n == node) {
            n = std::move(nodes.back());
            nodes.pop_back();
            return;
        }
    }
    throw UnknownNodeException("tried to delete a node that does not belong to this graph");
}

size_t Graph::getEdgeCount() const noexcept
{
    size_t ret;
    for (auto& node:nodes) {
        ret += node -> getEdgeCount();
    }
    // divisible by 2? we are getting double edge count
    assert((ret & 1) == 0);
    return ret>>1;
}

std::vector<const Node*> Graph::getNodes() const noexcept
{
    std::vector<const Node*> ret;
    ret.reserve(nodes.size());
    for (auto& node:nodes) {
        ret.push_back(node.get());
    }
    return ret;
}

