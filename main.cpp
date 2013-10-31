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
#include "unmanaged_ptr.hpp"

#include "Graph.hpp"

enum ZOrder
{
    zBackground,
    zEdges,
    zNodes,
    zUI
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

class GameWindow : public Gosu::Window
{
    Gosu::Font font;
    optional<Gosu::Image> img;
    Graph graph;
    unmanaged_ptr<Node> grabbedNode, connectingNode;
    Gosu::Image nodeImage;
public:
    GameWindow()
    :Window(640, 480, false)
    ,font(graphics(), Gosu::defaultFontName(), 20)
    ,nodeImage(graphics(), L"node.png", true)
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
        
        auto node = graph.createNode(Position(100, 100));
        auto node2 = graph.createNode(Position(150, 200));
        node->connect(node2);
    }

    void update() noexcept override
    {
        if (grabbedNode) {
            grabbedNode->x = input().mouseX();
            grabbedNode->y = input().mouseY();
        }
    }

    void draw() noexcept override
    {
        for (auto& node:graph.getNodes()) {
            nodeImage.draw(node->x-10, node->y-10, zNodes, 1, 1, Gosu::Color::RED);
            for (auto& edge: node->getEdges()) {
                graphics().drawLine(
                    node->x, node->y, Gosu::Color::BLUE,
                    edge->getTarget()->x, edge->getTarget()->y, Gosu::Color::BLUE,
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
            if (selected && (selected != connectingNode) && !selected->getEdge(connectingNode)) {
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
        {
            std::wstringstream wss;
            wss << graph.getEdgeCount();
            font.draw(wss.str().c_str(), 0, 0, 0);
        }
        double y = 20;
        for (auto edge:graph.getEdges()) {
            std::wstringstream wss;
            wss << edge->getSource().get() << " -> " << edge->getTarget().get();
            font.draw(wss.str().c_str(), 0, y, 0);
            y += 20;
        }
    }
    
    Position mousePosition() const noexcept
    {
        return Position(input().mouseX(), input().mouseY());
    }
    
    unmanaged_ptr<Node> selectNode() noexcept
    {
        if (graph.getNodeCount() == 0) return nullptr;
        auto mousePos = mousePosition();
        auto nearest = graph.getNearestNode(mousePos);
        auto diff = std::abs(*nearest - mousePos);
        if (diff.x < 10 && diff.y < 10) {
            return nearest;
        }
        return nullptr;
    }

    void buttonDown(Gosu::Button btn) noexcept override
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
    
    void buttonUp(Gosu::Button btn) noexcept override
    {
        if (btn == Gosu::msLeft) {
            // release any currently dragged node
            grabbedNode.clear();
        } else if (btn == Gosu::kbSpace) {
            auto selected = selectNode();
            if (!selected) {
                graph.createNode(mousePosition());
            } else {
                graph.deleteNode(selected);
            }
        } else if (btn == Gosu::msRight) {
            if (connectingNode) {
                auto selected = selectNode();
                if (selected) {
                    // check for a connection first
                    if (!selected->getEdge(connectingNode)) {
                        selected->connect(connectingNode);
                    }
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
