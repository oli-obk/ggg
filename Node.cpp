#include "Node.hpp"
#include <functional>

Node::~Node()
{
    for (auto& edge:_edges) {
        if (!edge) continue;
        bool success = false;
        for (auto& e:edge->node()._edges) {
            if (!e) continue;
            if (e->node() != *this) continue;
            e->node().num_edges--;
            e.clear();
            success = true;
            break;
        }
        assert(success);
        edge.clear();
    }
}

std::vector<Edge*> Node::Edges() noexcept
{
    std::vector<Edge*> ret;
    for (auto& edge:_edges) {
        if (edge) {
            ret.emplace_back(&*edge);
        }
    }
    return ret;
}

std::vector<const Edge*> Node::Edges() const noexcept
{
    std::vector<const Edge*> ret;
    for (auto& edge:_edges) {
        if (edge) {
            ret.emplace_back(&*edge);
        }
    }
    return ret;
}

optional<Edge&> Node::connection(Node& other) noexcept
{
    for (auto& edge:_edges) {
        if (!edge) continue;
        if (edge->node() != other) continue;
        return optional<Edge&>(*edge);
    }
    return optional<Edge&>();
}

bool Node::connect(Node& other) noexcept
{
    if (connection(other)) return false;
    if (num_edges == _edges.size()) {
        return false;
    }
    if (other.num_edges == _edges.size()) {
        return false;
    }
    num_edges++;
    other.num_edges++;
    bool success = false;
    for (auto& edge:_edges) {
        if (edge) continue;
        edge.emplace(std::ref(other));
        success = true;
        break;
    }
    assert(success);
    success = false;
    for (auto& edge:other._edges) {
        if (edge) continue;
        edge.emplace(std::ref(*this));
        success = true;
        break;
    }
    assert(success);
    return true;
}

