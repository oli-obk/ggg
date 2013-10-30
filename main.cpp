// All of Gosu.
#include <Gosu/Gosu.hpp>
// To safely include std::tr1::shared_ptr
#include <Gosu/TR1.hpp> 
// Makes life easier for Windows users compiling this.
#include <Gosu/AutoLink.hpp>

#include <cmath>
#include <cstdlib>
#include <list>
#include <memory>
#include <sstream> // For int <-> string conversion
#include <vector>

#include <cairomm/cairomm.h>
#include "optional.hpp"

enum ZOrder
{
    zBackground,
    zEdges,
    zNodes,
    zUI
};

class Position
{
public:
    double x, y;
    Position(double x, double y):x(x),y(y) {}
    Position():Position(0, 0){}
    double distanceSquared(const Position& rhs)
    {
        auto diff = *this - rhs;
        return diff.x*diff.x + diff.y*diff.y;
    }
    Position operator-(const Position& rhs)
    {
        return Position(x - rhs.x, y - rhs.y);
    }
    Position operator+(const Position& rhs)
    {
        return Position(x + rhs.x, y + rhs.y);
    }
};

namespace std
{
    Position abs(const Position& pos)
    {
        return Position(std::abs(pos.x), std::abs(pos.y));
    }
}

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

    bool operator==(const Edge& rhs) const
    {
        return this == &rhs;
    }
    
    bool operator!=(const Edge& rhs) const
    {
        return this != &rhs;
    }
    
    Node& node() { return *_node; }
    const Node& node() const { return *_node; }
};

namespace std {
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

struct Node : public Position
{
private:
    std::array<optional<Edge>, 10> _edges;
    size_t num_edges;
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
    Node(Node&&) = delete;
    Node& operator=(Node&&) = delete;
public:
    Node(Position pos = Position()):Position(pos){}
    ~Node()
    {
        for (auto& edge:_edges) {
            if (!edge) continue;
            bool success = false;
            for (auto& e:edge->node()._edges) {
                if (!e) continue;
                if (e->node() != *this) continue;
                e->node().num_edges--;
                e.clear();
                success = true;
                break;
            }
            assert(success);
            edge.clear();
        }
    }
    
    std::vector<Edge*> Edges()
    {
        std::vector<Edge*> ret;
        for (auto& edge:_edges) {
            if (edge) {
                ret.emplace_back(&*edge);
            }
        }
        return ret;
    }
    
    std::vector<const Edge*> Edges() const
    {
        std::vector<const Edge*> ret;
        for (auto& edge:_edges) {
            if (edge) {
                ret.emplace_back(&*edge);
            }
        }
        return ret;
    }
    
    optional<Edge&> connection(Node& other)
    {
        for (auto& edge:_edges) {
            if (!edge) continue;
            if (edge->node() != other) continue;
            return optional<Edge&>(*edge);
        }
        return optional<Edge&>();
    }
    
    bool connect(Node& other)
    {
        if (connection(other)) return false;
        if (num_edges == _edges.size()) {
            return false;
        }
        if (other.num_edges == _edges.size()) {
            return false;
        }
        num_edges++;
        other.num_edges++;
        bool success = false;
        for (auto& edge:_edges) {
            if (edge) continue;
            edge.emplace(std::ref(other));
            success = true;
            break;
        }
        assert(success);
        success = false;
        for (auto& edge:other._edges) {
            if (edge) continue;
            edge.emplace(std::ref(*this));
            success = true;
            break;
        }
        assert(success);
        return true;
    }
    
    bool operator==(const Node& rhs) const
    {
        return this == &rhs;
    }
    
    bool operator!=(const Node& rhs) const
    {
        return this != &rhs;
    }
};

// finish recursion
void drawLine(Gosu::Graphics&, Gosu::Color, double, Position){}

template<typename T, typename... Args>
void drawLine(Gosu::Graphics& g, Gosu::Color col, double z, T pos, T pos2, Args... args)
{
    static_assert(std::is_same<T, Position>::value, "drawLine can only be used with Position elements");
    g.drawLine(pos.x, pos.y, col, pos2.x, pos2.y, col, z);
    drawLine(g, col, z, pos2, args...);
}

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
    
    optional<Node&> GetNearestNode(Position pos)
    {
        optional<Node&> found;
        double dist;
        for (auto& node:Nodes) {
            if (!node) continue;
            auto _dist = pos.distanceSquared(*node);
            if (!found) {
                found.emplace(*node);
                dist = _dist;
                continue;
            }
            if (_dist < dist) {
                found.emplace(*node);
                dist = _dist;
            }
        }
        return found;
    }

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

class GameWindow : public Gosu::Window
{
    Gosu::Font font;
    optional<Gosu::Image> img;
    Graph graph;
    optional<Node&> grabbedNode, connectingNode;
    Gosu::Image NodeImage;
public:
    GameWindow()
    :Window(640, 480, false)
    ,font(graphics(), Gosu::defaultFontName(), 20)
    ,NodeImage(graphics(), L"node.png", true)
    {
        setCaption(L"GraphGame");
        auto surface = Cairo::ImageSurface::create(
                        Cairo::FORMAT_ARGB32,
                        240,
                        80
                        );
        auto ctx = Cairo::Context::create(surface);
        
        ctx->save();
        {
            ctx->select_font_face("serif", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
            ctx->set_font_size(32.0);
            ctx->set_source_rgb(0.0, 0.0, 1.0);
            ctx->move_to(10.0, 50.0);
            ctx->show_text("Hello, world");
        }
        ctx->restore();
        
        surface->flush();
        auto data = surface->get_data();
        data = surface->get_data();
        Gosu::Bitmap bmp(surface->get_width(), surface->get_height());
        Gosu::Color* it = bmp.data();
        for (size_t i = 0; i < surface->get_width()*surface->get_height(); i++) {
            it->setAlpha(data[3]);
            it->setRed(data[2]);
            it->setGreen(data[1]);
            it->setBlue(data[0]);
            it++;
            data += 4;
        }
        img.emplace(std::ref(graphics()), bmp);
        
        auto& node = graph.CreateNode(Position(100, 100));
        auto& node2 = graph.CreateNode(Position(150, 200));
        node.connect(node2);
    }

    void update()
    {
        if (grabbedNode) {
            grabbedNode->x = input().mouseX();
            grabbedNode->y = input().mouseY();
        }
    }

    void draw()
    {
        for (auto& node:graph.GetNodes()) {
            if (!node) continue;
            double wdt = 10;
            double hgt = 10;
            NodeImage.draw(node->x-10, node->y-10, zNodes, 1, 1, Gosu::Color::RED);
            for (auto& edge: node->Edges()) {
                graphics().drawLine(
                    node->x, node->y, Gosu::Color::BLUE,
                    edge->node().x, edge->node().y, Gosu::Color::BLUE,
                    zEdges
                );
            }
        }
        auto mousePos = mousePosition();
        drawLine(graphics(), Gosu::Color::WHITE, zUI,
            mousePos,
            mousePos + Position(20, 5),
            mousePos + Position(5, 20),
            mousePos
        );

        if (connectingNode) {
            // draw some sparkling green particles not signify a new edge that is being created
            Position targetPos;
            auto selected = selectNode();
            // snap to cursor && not to self && not to already connected nodes
            if (selected && (*selected != *connectingNode) && !selected->connection(*connectingNode)) {
                targetPos = *selected;
            } else {
                targetPos = mousePos;
            }
            graphics().drawLine(
                targetPos.x, targetPos.y, Gosu::Color::GREEN,
                connectingNode->x, connectingNode->y, Gosu::Color::GREEN,
                zUI
            );
        }
    }
    
    Position mousePosition() const
    {
        return Position(input().mouseX(), input().mouseY());
    }
    
    optional<Node&> selectNode()
    {
        auto mousePos = mousePosition();
        auto nearest = graph.GetNearestNode(mousePos);
        if (nearest) {
            auto diff = std::abs(*nearest - mousePos);
            if (diff.x < 10 && diff.y < 10) {
                return nearest;
            }
        }
        return optional<Node&>();
    }

    void buttonDown(Gosu::Button btn)
    {
        if (btn == Gosu::kbEscape) {
           close();
        } else if (btn == Gosu::msLeft) {
            auto selected = selectNode();
            if (selected) {
                grabbedNode = selected;
            }
        } else if (btn == Gosu::msRight) {
            auto selected = selectNode();
            if (selected) {
                connectingNode = selected;
            }
        }
    }
    
    void buttonUp(Gosu::Button btn)
    {
        if (btn == Gosu::msLeft) {
            // release any currently dragged node
            grabbedNode.clear();
        } else if (btn == Gosu::kbSpace) {
            auto selected = selectNode();
            if (!selected) {
                graph.CreateNode(mousePosition());
            } else {
                graph.DeleteNode(*selected);
            }
        } else if (btn == Gosu::msRight) {
            if (connectingNode) {
                auto selected = selectNode();
                if (selected) {
                    selected->connect(*connectingNode);
                }
                // release edge
                connectingNode.clear();
            }
        }
    }
};

int main()
{
    GameWindow window;
    window.show();
}
