#ifndef GGG_EDGE_HPP
#define GGG_EDGE_HPP

class Node;

class Edge
{
private:
    Node* _node;
    Edge(const Edge&) = delete;
    Edge& operator=(const Edge&) = delete;
public:
    double weight;
    explicit Edge(Node& node)
    :weight(0)
    ,_node(&node)
    {}

    bool operator==(const Edge& rhs) const { return this == &rhs; }
    bool operator!=(const Edge& rhs) const { return this != &rhs; }
    
    Node& node() { return *_node; }
    const Node& node() const { return *_node; }
};

#endif //GGG_EDGE_HPP
