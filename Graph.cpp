#import "Graph.hpp"
#import "make_unique.hpp"
#import <limits>
#import <set>

unmanaged_ptr<Node> Graph::getNearestNode(Position pos) noexcept
{
    if (nodes.empty()) {
        throw ThereAreNoNodesException("can't get nearest node without any nodes");
    }
    unmanaged_ptr<Node> found;
    double dist = std::numeric_limits<double>::max();
    for (auto& node:nodes) {
        auto _dist = pos.distanceSquared(*node);
        if (_dist < dist) {
            found = node;
            dist = _dist;
        }
    }
    assert(found);
    return found;
}


unmanaged_ptr<Node> Graph::createNode(Position pos)
{
    nodes.push_back(std::make_unique<Node>(pos));
    return nodes.back().get();
}

void Graph::deleteNode(unmanaged_ptr<Node> node)
{
    for (auto& n:nodes) {
        if (n == node) {
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

std::vector<unmanaged_ptr<const Node>> Graph::getNodes() const noexcept
{
    std::vector<unmanaged_ptr<const Node>> ret;
    ret.reserve(nodes.size());
    for (auto& node:nodes) {
        ret.push_back(node.get());
    }
    return ret;
}


std::vector<unmanaged_ptr<const Edge>> Graph::getUndirectedEdges() const noexcept
{
    std::vector<unmanaged_ptr<const Edge>> ret;
    std::set<unmanaged_ptr<const Node>> processedNodes;
    for (auto& node:nodes) {
        for (auto edge:node->getEdges()) {
            if (processedNodes.find(edge->getTarget()) != processedNodes.end()) {
                // already have this edge
                continue;
            }
            ret.push_back(edge);
        }
        processedNodes.insert(node.get());
    }
    return ret;
}


