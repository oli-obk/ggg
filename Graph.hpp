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
    std::array<optional<Node>, 100> Nodes;
    size_t NodeCount;
    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;
    Graph(Graph&&) = delete;
    Graph& operator=(Graph&&) = delete;
public:
    Graph()
    :NodeCount(0)
    {
    }
    
    const decltype(Nodes)& GetNodes() const noexcept { return Nodes; }
    
    optional<Node&> GetNearestNode(Position pos) noexcept;

    // throws a TooManyNodesException if there are already 100 nodes
    Node& CreateNode(Position pos);
    
    const size_t GetNodeCount() const noexcept { return NodeCount; }

    // throws an UnknownNodeException if the node is not handled by this graph
    void DeleteNode(Node& node);
};
