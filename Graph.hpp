#include "Node.hpp"
#include "Edge.hpp"
#include <memory>

class TooManyNodesException : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

class UnknownNodeException : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

class ThereAreNoNodesException : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

class Graph
{
    std::vector<std::unique_ptr<Node>> nodes;
    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;
    Graph(Graph&&) = delete;
    Graph& operator=(Graph&&) = delete;
public:
    Graph() = default;
    
    std::vector<const Node*> getNodes() const noexcept;
    
    Node& getNearestNode(Position pos);

    // throws a TooManyNodesException if there are already 100 nodes
    Node& createNode(Position pos);
    
    size_t getNodeCount() const noexcept { return nodes.size(); }

    // throws an UnknownNodeException if the node is not handled by this graph
    void deleteNode(Node& node);
    
    size_t getEdgeCount() const noexcept;
};
