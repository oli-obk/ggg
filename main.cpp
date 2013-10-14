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
    optional<Gosu::Image> img;
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
        
        ctx->select_font_face("serif", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
        ctx->set_font_size(32.0);
        ctx->set_source_rgb(0.0, 0.0, 1.0);
        ctx->move_to(10.0, 50.0);
        ctx->show_text("Hello, world");
        
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
    }

    void update()
    {
    }

    void draw()
    {
        if(img) {
            img->draw(0, 0, 0);
            font.draw(L"bla", 0, 0, 1);
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
