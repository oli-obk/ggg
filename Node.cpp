#include "Node.hpp"
#include <functional>

Node::~Node()
{
    for (auto& edge:edges) {
        if (!edge) continue;
        bool success = false;
        for (auto& e:edge->getNode().edges) {
            if (!e) continue;
            if (e->getNode() != *this) continue;
            e->getNode().num_edges--;
            e.clear();
            success = true;
            break;
        }
        assert(success);
        edge.clear();
    }
}

std::vector<Edge*> Node::getEdges() noexcept
{
    std::vector<Edge*> ret;
    for (auto& edge:edges) {
        if (edge) {
            ret.emplace_back(&*edge);
        }
    }
    return ret;
}

std::vector<const Edge*> Node::getEdges() const noexcept
{
    std::vector<const Edge*> ret;
    for (auto& edge:edges) {
        if (edge) {
            ret.emplace_back(&*edge);
        }
    }
    return ret;
}

optional<Edge&> Node::getEdge(Node& other) noexcept
{
    for (auto& edge:edges) {
        if (!edge) continue;
        if (edge->getNode() != other) continue;
        return optional<Edge&>(*edge);
    }
    return optional<Edge&>();
}

Edge& Node::connect(Node& other)
{
    if (getEdge(other)) throw NodesAlreadyConnectedException("there is already an edge");
    if (num_edges == edges.size()) {
        throw TooManyEdgesException("this node has too many edges");
    }
    if (other.num_edges == other.edges.size()) {
        throw TooManyEdgesException("target node has too many edges");
    }
    num_edges++;
    other.num_edges++;
    bool success = false;
    Edge* ret;
    for (auto& edge:edges) {
        if (edge) continue;
        edge.emplace(std::ref(other));
        ret = &*edge;
        success = true;
        break;
    }
    assert(success);
    success = false;
    for (auto& edge:other.edges) {
        if (edge) continue;
        edge.emplace(std::ref(*this));
        success = true;
        break;
    }
    assert(success);
    return *ret;
}

