#ifndef GGG_NODE_HPP
#define GGG_NODE_HPP

#include "Edge.hpp"
#include "Position.hpp"
#include <vector>
#include <memory>
#include "unmanaged_ptr.hpp"

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
    std::vector<std::unique_ptr<Edge>> edges;
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
    Node(Node&&) = delete;
    Node& operator=(Node&&) = delete;
public:
    Node(Position pos = Position()) noexcept
    :Position(pos){}
    
    ~Node();
    
    size_t getEdgeCount() const noexcept { return edges.size(); }
    
    std::vector<unmanaged_ptr<Edge>> getEdges() noexcept;
    
    std::vector<unmanaged_ptr<const Edge>> getEdges() const noexcept;
    
    unmanaged_ptr<Edge> getEdge(unmanaged_ptr<Node> other) noexcept;
    
    unmanaged_ptr<Edge> connect(unmanaged_ptr<Node> other);
};

#endif //GGG_NODE_HPP
