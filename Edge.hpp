#import "unmanaged_ptr.hpp"

class Node;

class Edge
{
private:
    const unmanaged_ptr<Node> source, target;
    Edge(const Edge&) = delete;
    Edge& operator=(const Edge&) = delete;
    Edge(Edge&&) = delete;
    Edge& operator=(Edge&&) = delete;
public:
    explicit Edge(unmanaged_ptr<Node> source, unmanaged_ptr<Node> target)
    :source(source)
    ,target(target)
    {}
    
    unmanaged_ptr<Node> getSource() { return source; }
    unmanaged_ptr<const Node> getSource() const { return source; }
    unmanaged_ptr<Node> getTarget() { return target; }
    unmanaged_ptr<const Node> getTarget() const { return target; }
};
