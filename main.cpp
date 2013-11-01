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

#import "Game.hpp"

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
    Gosu::Font font = {graphics(), Gosu::defaultFontName(), 20};
    optional<Gosu::Image> img;
    Graph graph;
    unmanaged_ptr<Node> grabbedNode, connectingNode;
    NodePtr shortestDistSource;
    std::vector<Path> pathsToDraw;
    Gosu::Image nodeImage = {graphics(), L"node.png", true};
    Gosu::Image nodeFillImage = {graphics(), L"node_fill.png", true};
    Betweenness betweenness;
    TurnbasedGame game = TurnbasedGame({{"Player 1", Gosu::Color::YELLOW}, {"Player 2", Gosu::Color::CYAN}});
    std::map<NodePtr, unmanaged_ptr<Player>> nodeOwner;
public:
    GameWindow()
    :Window(640, 480, false)
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
    
    void onGraphChanged()
    {
        betweenness.run(graph);
        for (auto player : game.getPlayers()) {
            player->setScore(0);
        }
        for (auto node : graph.getNodes()) {
            auto it = nodeOwner.find(node);
            if (it == nodeOwner.end()) continue;
            it->second->addScore(betweenness.getValue(node));
        }
    }

    void draw() noexcept override
    {
        for (auto& node:graph.getNodes()) {
            double val = betweenness.getValue(node);
            auto col = Gosu::interpolate(Gosu::Color::BLUE, Gosu::Color::RED, val);
            nodeImage.draw(node->x-10, node->y-10, zNodes, 1, 1, col);
            auto owner = nodeOwner.find(node);
            if (owner != nodeOwner.end()) {
                nodeFillImage.draw(node->x-10, node->y-10, zNodes, 1, 1, owner->second->getColor());
            }
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
        if (!connectingNode) {
            auto nearest = graph.getNearestNode(mousePos);
            graphics().drawLine(
                    mousePos.x, mousePos.y, Gosu::Color::YELLOW,
                    nearest->x, nearest->y, Gosu::Color::YELLOW,
                    zUI
                );
        }

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
            wss << Gosu::fps() << L" fps";
            font.draw(wss.str().c_str(), 0, 0, zUI);
        }
        double y = 20;
        auto currentPlayer = game.getCurrentPlayer();
        for (auto player : game.getPlayers()) {
            auto name = player->getName();
            auto wname = std::wstring(std::begin(name), std::end(name));
            if (player == currentPlayer) {
                font.draw(wname, 0, y-2, zUI, 1.4, 1.4, player->getColor());
            } else {
                font.draw(wname, 0, y, zUI, 1, 1, player->getColor());
            }
            y += 20;
        }
        
        double x = graphics().width() - 50;
        for (auto player : game.getPlayers()) {
            auto col = player->getColor();
            double score = player->getScore() * 30;
            graphics().drawQuad(
                x, 0, col,
                x+40, 0, col,
                x+40, score, col,
                x, score, col,
                zUI
            );
            std::wstringstream wss;
            wss << score;
            font.draw(wss.str().c_str(), x, score, zUI);
            x -= 50;
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
                    // dragl
                    grabbedNode = selected;
                }
            } else {
                auto nearest = graph.getNearestNode(mousePosition());
                auto node = graph.createNode(mousePosition());
                nodeOwner[node] = game.getCurrentPlayer();
                node -> connect(nearest);
                onGraphChanged();
                game.endTurn();
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
        } else if (btn == Gosu::msRight) {
            if (connectingNode) {
                auto selected = selectNode();
                if (selected && (selected != connectingNode)) {
                    // check for a connection first
                    if (!selected->getEdge(connectingNode)) {
                        selected->connect(connectingNode);
                        onGraphChanged();
                        game.endTurn();
                    }
                }
                // release edge
                connectingNode.clear();
            }
        } else if (btn == Gosu::kbLeftShift) {
            grabbedNode.clear();
        }
    }
};

int main()
{
    GameWindow window;
    window.show();
}
