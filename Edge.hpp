#import "unmanaged_ptr.hpp"
#import "ForwardDeclarations.hpp"

class Edge
{
private:
    NodePtr source, target;
    Edge(const Edge&) = delete;
    Edge& operator=(const Edge&) = delete;
    Edge(Edge&&) = delete;
    Edge& operator=(Edge&&) = delete;
public:
    explicit Edge(NodePtr source, NodePtr target)
    :source(source)
    ,target(target)
    {}
    
    NodePtr getSource() { return source; }
    const NodePtr getSource() const { return source; }
    NodePtr getTarget() { return target; }
    const NodePtr getTarget() const { return target; }
};
