#include "Node.hpp"
#include <functional>
#include "make_unique.hpp"
#include <cassert>

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

std::vector<unmanaged_ptr<Edge>> Node::getEdges() noexcept
{
    std::vector<unmanaged_ptr<Edge>> ret;
    for (auto& edge:edges) {
        ret.push_back(edge.get());
    }
    return ret;
}

std::vector<unmanaged_ptr<const Edge>> Node::getEdges() const noexcept
{
    std::vector<unmanaged_ptr<const Edge>> ret;
    for (auto& edge:edges) {
        ret.push_back(edge.get());
    }
    return ret;
}

unmanaged_ptr<Edge> Node::getEdge(unmanaged_ptr<Node> other) noexcept
{
    for (auto& edge:edges) {
        if (edge->getTarget() != other) continue;
        return edge.get();
    }
    return nullptr;
}

unmanaged_ptr<Edge> Node::connect(unmanaged_ptr<Node> other)
{
    if (getEdge(other)) throw NodesAlreadyConnectedException("there is already an edge");
    // create edge from here to there
    edges.push_back(std::make_unique<Edge>(this, other));
    // and the other direction
    other->edges.push_back(std::make_unique<Edge>(other, this));
    return edges.back().get();
}

