#import "Position.hpp"
#import <vector>
#import <memory>
#import "unmanaged_ptr.hpp"
#import "ForwardDeclarations.hpp"

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
    size_t id;
public:
    Node(Position pos, size_t id) noexcept
    :Position(pos)
    ,id(id)
    {}
    
    Node& operator=(const Position& pos)
    {
        x = pos.x;
        y = pos.y;
        return *this;
    }
    
    size_t getId() const noexcept { return id; }
    
    ~Node();
    
    size_t getEdgeCount() const noexcept { return edges.size(); }
    
    std::vector<EdgePtr> getEdges() noexcept;
    
    const std::vector<EdgePtr> getEdges() const noexcept;
    
    const EdgePtr getEdge(const NodePtr other) const noexcept;
    
    EdgePtr connect(NodePtr other);
};

