#ifndef GGG_NODE_HPP
#define GGG_NODE_HPP

#include "Edge.hpp"
#include "Position.hpp"
#include <array>
#include "optional.hpp"
#include <vector>

class NodesAlreadyConnectedException : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

class TooManyEdgesException : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

class Node : public Position
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
    
    size_t getEdgeCount() const noexcept { return num_edges; }
    
    std::vector<Edge*> getEdges() noexcept;
    
    std::vector<const Edge*> getEdges() const noexcept;
    
    optional<Edge&> getEdge(Node& other) noexcept;
    
    Edge& connect(Node& other);
    
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
