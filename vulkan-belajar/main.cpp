#include "Application.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    vkb::Application app{};

    if (!app.initialize())
    {
        return -1;
    }

    app.run();

    app.shutdown();

    return 0;
}
