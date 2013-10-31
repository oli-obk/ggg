#import "Graph.hpp"
#import "Node.hpp"
#import "Edge.hpp"
#import "make_unique.hpp"
#import <limits>
#import <set>

NodePtr Graph::getNearestNode(Position pos) noexcept
{
    if (nodes.empty()) {
        throw ThereAreNoNodesException("can't get nearest node without any nodes");
    }
    NodePtr found;
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


NodePtr Graph::createNode(Position pos)
{
    nodes.push_back(std::make_unique<Node>(pos));
    return nodes.back().get();
}

void Graph::deleteNode(NodePtr node)
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
    size_t ret = 0;
    for (auto& node:nodes) {
        ret += node -> getEdgeCount();
    }
    // divisible by 2? we are getting double edge count
    assert((ret & 1) == 0);
    return ret>>1;
}

const std::vector<NodePtr> Graph::getNodes() const noexcept
{
    std::vector<NodePtr> ret;
    ret.reserve(nodes.size());
    for (auto& node:nodes) {
        ret.push_back(node.get());
    }
    return ret;
}


const std::vector<EdgePtr> Graph::getUndirectedEdges() const noexcept
{
    std::vector<EdgePtr> ret;
    std::set<NodePtr> processedNodes;
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


