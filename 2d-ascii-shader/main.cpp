// clang-format off
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color_info.hpp>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include <array>
#include <cstdint>
#include <memory.h>
#include <stdio.h>
// clang-format on

const std::array<unsigned char, 31> ASCII_CHAR = {'N', '@', '#', 'W', '$', '9', '8', '7', '6', '5', '4', '3', '2', '1',
                                                  '0', '?', '!', 'a', 'b', 'c', ';', ':', '+', '=', '-', '_', ',', '.'};

struct Context
{
    std::atomic<bool> is_run = true;
    ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();
};
static Context GameContext{};

struct Vertex
{
    int pos_x = 0;
    int pos_y = 0;
    int color = ftxui::Color::White;
};

auto game_loop(Context& game_context) -> void
{
    auto next_frame = std::chrono::steady_clock::now();

    while (game_context.is_run == true)
    {
        next_frame += std::chrono::milliseconds(1);

        game_context.screen.PostEvent(ftxui::Event::Custom);

        std::this_thread::sleep_until(next_frame);
    }
}

// TODO: spawn a new terminal and run the program there
int main()
{
    auto v = Vertex{};
    v.pos_x = 50;
    v.pos_y = 100;

    auto component = ftxui::Renderer(
        [&]
        {
            ftxui::Canvas canvas(500, 500);
            canvas.DrawPoint(10, 10, true);
            canvas.DrawBlockLine(1, 1, 100, 2);

            canvas.DrawText(10, 300, std::to_string(GameContext.screen.dimx()));
            canvas.DrawText(25, 300, std::to_string(GameContext.screen.dimy()));

            auto px = ftxui::Pixel();
            px.foreground_color = ftxui::Color::Red;
            px.character = ASCII_CHAR[0];
            px.bold = true;
            canvas.DrawPixel(v.pos_x, v.pos_y, px);

            return ftxui::canvas(std::move(canvas));
        });

    component = ftxui::CatchEvent(
        component,
        [&](ftxui::Event event)
        {
            if (event == ftxui::Event::Escape || event == ftxui::Event::q)
            {
                GameContext.is_run = false;
                GameContext.screen.Exit();
                // GameContext.screen.ExitLoopClosure();
            }
            // NOTEX: 
            // * each cell in ftxui canvas is a 2x4 braille dot (2 columns & 4 rows).
            // * moving one cell horizontaly, need a total of 2 pixels, that's why we use "+= 2" on the X axis
            // * same goes for Y axis, but we need 4
            else if (event == ftxui::Event::d)
            {
                v.pos_x += 2;
            }
            else if (event == ftxui::Event::a)
            {
                v.pos_x -= 2;
            }
            else if (event == ftxui::Event::w)
            {
                v.pos_y -= 4;
            }
            else if (event == ftxui::Event::s)
            {
                v.pos_y += 4;
            }
            else
            {
                return false;
            }

            return true;
        });
    std::thread refresh([&] { game_loop(GameContext); });

    GameContext.screen.Clear();
    GameContext.screen.Loop(component);

    GameContext.is_run = false;

    if (refresh.joinable())
    {
        refresh.join();
    }

    return EXIT_SUCCESS;
}
