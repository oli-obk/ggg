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

enum class ZOrder
{
    Background,
    Stars,
    Player,
    UI
};

class GameWindow : public Gosu::Window
{
    Gosu::Font font;

public:
    GameWindow()
    :Window(640, 480, false)
    ,font(graphics(), Gosu::defaultFontName(), 20)
    {
        setCaption(L"GraphGame");
    }

    void update()
    {
    }

    void draw()
    {
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
