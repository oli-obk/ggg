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
    zGraph,
    zUI
};

class Node;

class Edge
{
private:
    Node* _node;
    Edge* _other;
public:
    double weight;
    Edge(Node& node)
    :weight(0)
    ,_node(&node)
    {}
    
    Edge(Node& node, Edge& edge)
    :Edge(node)
    {
        _other = &edge;
        edge._other = this;
    }
    Node& node() { return *_node; }
    Edge& other() { return *_other; }
};

namespace std {
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

struct Node
{
private:
    std::vector<std::unique_ptr<Edge>> _edges;
public:
    double x, y;
    Node(double x, double y)
    :x(x)
    ,y(y)
    {
    }
    
    const std::vector<std::unique_ptr<Edge>>& Edges() const { return _edges; }
    
    optional<Edge&> connection(Node& other) const
    {
        for (auto& edge:_edges) {
            if (&edge->node() == &other) {
                return optional<Edge&>(*edge);
            }
        }
        return optional<Edge&>();
    }
    
    bool connect(Node& other)
    {
        if (connection(other)) return false;
        auto edge = std::make_unique<Edge>(*this);
        auto edge2 = std::make_unique<Edge>(*this, *edge);
        _edges.push_back(std::move(edge));
        other._edges.push_back(std::move(edge2));
        return true;
    }
    
};

class Graph
{
    static const size_t max_nodes = 100;
    std::array<optional<Node>, max_nodes> Nodes;
    size_t NodeCount;
public:
    Graph()
    :NodeCount(0)
    {
    }
    
    optional<Node&> GetNearestNode(double x, double y)
    {
        optional<Node&> found;
        double dist;
        for (auto& node:Nodes) {
            if (!node) continue;
            auto _dist = (x - node->x)*(x - node->x) + (y - node->y)*(y - node->y);
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

    Node& CreateNode(double x, double y)
    {
        for (auto& node:Nodes) {
            if (node) continue;
            node.emplace(x, y);
            NodeCount++;
            return *node;
        }
        throw std::runtime_error("too many nodes");
    }

    void DeleteNode(Node& node)
    {
        for (auto& n:Nodes) {
            if (&(*n) == &node) {
                n.clear();
                NodeCount--;
                return;
            }
        }
        throw std::runtime_error("unknown node");
    }
    
    void draw(Gosu::Graphics& g)
    {
        for (auto& node:Nodes) {
            if (!node) continue;
            double wdt = 10;
            double hgt = 10;
            g.drawQuad(
                node->x - wdt, node->y - wdt, Gosu::Color::RED,
                node->x + wdt, node->y - wdt, Gosu::Color::RED,
                node->x + wdt, node->y + wdt, Gosu::Color::RED,
                node->x - wdt, node->y + wdt, Gosu::Color::RED,
                zUI
            );
            for (auto& edge: node->Edges()) {
                g.drawLine(
                    node->x, node->y, Gosu::Color::BLUE,
                    edge->node().x, edge->node().y, Gosu::Color::BLUE,
                    zUI
                );
            }
        }
    }
};

class GameWindow : public Gosu::Window
{
    Gosu::Font font;
    optional<Gosu::Image> img;
    Graph graph;
public:
    GameWindow()
    :Window(640, 480, false)
    ,font(graphics(), Gosu::defaultFontName(), 20)
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
        
        auto& node = graph.CreateNode(100, 100);
        auto& node2 = graph.CreateNode(150, 200);
        node.connect(node2);
    }

    void update()
    {
    }

    void draw()
    {
        graph.draw(graphics());
        graphics().drawTriangle(
            input().mouseX(), input().mouseY(), Gosu::Color::WHITE,
            input().mouseX()+20, input().mouseY()+5, Gosu::Color::WHITE,
            input().mouseX()+5, input().mouseY()+20, Gosu::Color::WHITE,
            zUI
        );
        auto nearest = graph.GetNearestNode(input().mouseX(), input().mouseY());
        if (nearest) {
            graphics().drawLine(
                input().mouseX(), input().mouseY(), Gosu::Color::GREEN,
                nearest->x, nearest->y, Gosu::Color::GREEN,
                zUI
            );
        }
    }

    void buttonDown(Gosu::Button btn)
    {
        if (btn == Gosu::kbEscape) {
           close();
        }
    }
};

int main()
{
    GameWindow window;
    window.show();
}
