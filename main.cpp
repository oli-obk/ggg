// All of Gosu.
#import <Gosu/Gosu.hpp>
// To safely import std::tr1::shared_ptr
#import <Gosu/TR1.hpp> 
// Makes life easier for Windows users compiling this.
#import <Gosu/AutoLink.hpp>

#import <cmath>
#import <cstdlib>
#import <list>
#import <memory>
#import <sstream> // For int <-> string conversion
#import <vector>

#import <cairomm/cairomm.h>
#import "optional.hpp"
#import "unmanaged_ptr.hpp"

#import "Graph.hpp"
#import "Node.hpp"
#import "Edge.hpp"

#import "FloydWarshall.hpp"
#import "Betweenness.hpp"
#import <iostream>

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
    NodePtr shortestDistSource;
    std::vector<Path> pathsToDraw;
    Gosu::Image nodeImage;
    Betweenness betweenness;
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
        auto node3 = graph.createNode(Position(400, 200));
        auto node4 = graph.createNode(Position(300, 300));
        node->connect(node2);
        node2->connect(node3);
        node3->connect(node4);
    }

    void update() noexcept override
    {
        if (grabbedNode) {
            grabbedNode->x = input().mouseX();
            grabbedNode->y = input().mouseY();
        }
        betweenness.run(graph);
    }

    void draw() noexcept override
    {
        for (auto& node:graph.getNodes()) {
            double val = betweenness.getValue(node);
            auto col = Gosu::interpolate(Gosu::Color::BLUE, Gosu::Color::RED, val);
            nodeImage.draw(node->x-10, node->y-10, zNodes, 1, 1, col);
            for (auto& edge: node->getEdges()) {
                double val2 = betweenness.getValue(edge->getTarget());
                auto col2 = Gosu::interpolate(Gosu::Color::BLUE, Gosu::Color::RED, val2);
                graphics().drawLine(
                    node->x, node->y, col,
                    edge->getTarget()->x, edge->getTarget()->y, col2,
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

        if (shortestDistSource) {
            // draw some sparkling green particles not signify a new edge that is being created
            Position targetPos;
            auto selected = selectNode();
            // snap to cursor && not to self
            if (selected && (selected != shortestDistSource)) {
                targetPos = *selected;
            } else {
                targetPos = mousePos;
            }
            graphics().drawLine(
                targetPos.x, targetPos.y, Gosu::Color::AQUA,
                shortestDistSource->x, shortestDistSource->y, Gosu::Color::AQUA,
                zUI
            );
        }
        
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

        if (!pathsToDraw.empty()) {
            for (auto pathToDraw : pathsToDraw) {
                for (auto it = pathToDraw.begin(); it != pathToDraw.end(); it++) {
                    auto next = it;
                    next++;
                    if (next == pathToDraw.end()) break;
                    graphics().drawLine(
                        (*it)->x, (*it)->y, Gosu::Color::AQUA,
                        (*next)->x, (*next)->y, Gosu::Color::AQUA,
                        zUI
                    );
                }
            }
        }

        {
            std::wstringstream wss;
            wss << Gosu::fps();
            font.draw(wss.str().c_str(), 0, 0, 0);
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
                if (input().down(Gosu::kbLeftShift)) {
                    // compute shortest distance
                    shortestDistSource = selected;
                } else {
                    // dragl
                    grabbedNode = selected;
                }
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
            if (shortestDistSource) {
                auto selected = selectNode();
                if (selected && selected != shortestDistSource) {
                    FloydWarshall fw;
                    fw.run(graph);
                    fw.printDistanceMatrix();
                    pathsToDraw = fw.getPath(shortestDistSource, selected);
                }
                // release edge
                shortestDistSource.clear();
            }
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
        } else if (btn == Gosu::kbLeftShift) {
            shortestDistSource.clear();
        }
    }
};

int main()
{
    GameWindow window;
    window.show();
}
