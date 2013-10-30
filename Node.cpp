#include "Node.hpp"
#include <functional>
#include "make_unique.hpp"

Node::~Node()
{
    for (auto& edge:edges) {
        bool success = false;
        for (auto& e:edge->getNode().edges) {
            if (e->getNode() != *this) continue;
            e = std::move(edge->getNode().edges.back());
            edge->getNode().edges.pop_back();
            success = true;
            break;
        }
        assert(success);
    }
}

std::vector<Edge*> Node::getEdges() noexcept
{
    std::vector<Edge*> ret;
    for (auto& edge:edges) {
        ret.push_back(edge.get());
    }
    return ret;
}

std::vector<const Edge*> Node::getEdges() const noexcept
{
    std::vector<const Edge*> ret;
    for (auto& edge:edges) {
        ret.push_back(edge.get());
    }
    return ret;
}

optional<Edge&> Node::getEdge(Node& other) noexcept
{
    for (auto& edge:edges) {
        if (edge->getNode() != other) continue;
        return optional<Edge&>(*edge);
    }
    return optional<Edge&>();
}

Edge& Node::connect(Node& other)
{
    if (getEdge(other)) throw NodesAlreadyConnectedException("there is already an edge");
    Edge* ret;
    edges.push_back(std::make_unique<Edge>(std::ref(other)));
    other.edges.push_back(std::make_unique<Edge>(std::ref(*this)));
    return *ret;
}

