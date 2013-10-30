#ifndef GGG_NODE_HPP
#define GGG_NODE_HPP

#include "Edge.hpp"
#include "Position.hpp"
#include <array>
#include "optional.hpp"
#include <vector>

struct Node : public Position
{
private:
    std::array<optional<Edge>, 10> edges;
    size_t num_edges;
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
    Node(Node&&) = delete;
    Node& operator=(Node&&) = delete;
public:
    Node(Position pos = Position()) noexcept
    :Position(pos){}
    
    ~Node();
    
    std::vector<Edge*> getEdges() noexcept;
    
    std::vector<const Edge*> getEdges() const noexcept;
    
    optional<Edge&> getEdge(Node& other) noexcept;
    
    bool connect(Node& other) noexcept;
    
    bool operator==(const Node& rhs) const noexcept
    {
        return this == &rhs;
    }
    
    bool operator!=(const Node& rhs) const noexcept
    {
        return this != &rhs;
    }
};

#endif //GGG_NODE_HPP
