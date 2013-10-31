#import <memory>
#import <vector>
#import "unmanaged_ptr.hpp"
#import "Position.hpp"
#import "ForwardDeclarations.hpp"

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
    
    const std::vector<NodePtr> getNodes() const noexcept;
    
    NodePtr getNearestNode(Position pos) noexcept;

    // throws a TooManyNodesException if there are already 100 nodes
    NodePtr createNode(Position pos);
    
    size_t getNodeCount() const noexcept { return nodes.size(); }

    // throws an UnknownNodeException if the node is not handled by this graph
    void deleteNode(NodePtr node);
    
    size_t getEdgeCount() const noexcept;
    
    const std::vector<EdgePtr> getUndirectedEdges() const noexcept;
};
