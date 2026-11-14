#include "Application.h"

int main(int argc, char* argv[])
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
