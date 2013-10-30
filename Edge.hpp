#ifndef GGG_EDGE_HPP
#define GGG_EDGE_HPP

class Node;

class Edge
{
private:
    Node* node;
    Edge(const Edge&) = delete;
    Edge& operator=(const Edge&) = delete;
public:
    explicit Edge(Node& node)
    :node(&node)
    {}

    bool operator==(const Edge& rhs) const { return this == &rhs; }
    bool operator!=(const Edge& rhs) const { return this != &rhs; }
    
    Node& getNode() { return *node; }
    const Node& getNode() const { return *node; }
};

#endif //GGG_EDGE_HPP
