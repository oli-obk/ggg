#include "Node.hpp"
#include "Edge.hpp"

class Graph
{
    std::array<optional<Node>, 100> Nodes;
    size_t NodeCount;
    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;
public:
    Graph()
    :NodeCount(0)
    {
    }
    
    const decltype(Nodes)& GetNodes() const { return Nodes; }
    
    optional<Node&> GetNearestNode(Position pos);

    Node& CreateNode(Position pos)
    {
        for (auto& node:Nodes) {
            if (node) continue;
            node.emplace(pos);
            NodeCount++;
            return *node;
        }
        throw std::runtime_error("too many nodes");
    }

    void DeleteNode(Node& node)
    {
        for (auto& n:Nodes) {
            if (!n) continue;
            if (*n == node) {
                n.clear();
                NodeCount--;
                return;
            }
        }
        throw std::runtime_error("unknown node");
    }
};
