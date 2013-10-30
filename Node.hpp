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
    std::array<optional<Edge>, 10> _edges;
    size_t num_edges;
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
    Node(Node&&) = delete;
    Node& operator=(Node&&) = delete;
public:
    Node(Position pos = Position()):Position(pos){}
    ~Node();
    
    std::vector<Edge*> Edges();
    
    std::vector<const Edge*> Edges() const;
    
    optional<Edge&> connection(Node& other);
    
    bool connect(Node& other);
    
    bool operator==(const Node& rhs) const
    {
        return this == &rhs;
    }
    
    bool operator!=(const Node& rhs) const
    {
        return this != &rhs;
    }
};

#endif //GGG_NODE_HPP
