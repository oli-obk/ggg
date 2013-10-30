#include "Node.hpp"
#include "Edge.hpp"

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

class Graph
{
    std::array<optional<Node>, 100> nodes;
    size_t nodeCount;
    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;
    Graph(Graph&&) = delete;
    Graph& operator=(Graph&&) = delete;
public:
    Graph()
    :nodeCount(0)
    {
    }
    
    std::vector<const Node*> getNodes() const noexcept;
    
    optional<Node&> getNearestNode(Position pos) noexcept;

    // throws a TooManyNodesException if there are already 100 nodes
    Node& createNode(Position pos);
    
    const size_t getNodeCount() const noexcept { return nodeCount; }

    // throws an UnknownNodeException if the node is not handled by this graph
    void deleteNode(Node& node);
    
    size_t getEdgeCount() const noexcept;
};
