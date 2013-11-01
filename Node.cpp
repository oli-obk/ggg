#import "Node.hpp"
#import "Edge.hpp"
#import <functional>
#import "make_unique.hpp"
#import <cassert>

Node::~Node()
{
    for (auto& edge:edges) {
        bool success = false;
        auto& other_edges = edge->getTarget()->edges;
        for (auto& e:other_edges) {
            if (e->getTarget() != this) continue;
            e = std::move(other_edges.back());
            other_edges.pop_back();
            success = true;
            break;
        }
        assert(success);
    }
}

std::vector<EdgePtr> Node::getEdges() noexcept
{
    std::vector<EdgePtr> ret;
    for (auto& edge:edges) {
        ret.push_back(edge.get());
    }
    return ret;
}

const std::vector<EdgePtr> Node::getEdges() const noexcept
{
    std::vector<EdgePtr> ret;
    for (auto& edge:edges) {
        ret.push_back(edge.get());
    }
    return ret;
}

const EdgePtr Node::getEdge(const NodePtr other) const noexcept
{
    for (auto& edge:edges) {
        if (edge->getTarget() != other) continue;
        return edge.get();
    }
    return nullptr;
}

EdgePtr Node::connect(NodePtr other)
{
    if (getEdge(other)) throw NodesAlreadyConnectedException("there is already an edge");
    if (other == this) throw NodesAlreadyConnectedException("cannot connect to self");
    // create edge from here to there
    edges.push_back(std::make_unique<Edge>(this, other));
    // and the other direction
    other->edges.push_back(std::make_unique<Edge>(other, this));
    return edges.back().get();
}

