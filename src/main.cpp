#include <micro-engine/micro.h>

#include "constants.h"
#include "interface.h"
#include "pipeline.h"
#include "state.h"

using namespace micro;
using namespace micro::context;

auto main() -> int {
    window(Application::Window::Title, Application::Window::Width, Application::Window::Height);
    State state{};

    event.handler = []() {};

    view.showGui   = true;
    view.interface = [&]() { interface(state); };

    view.pipeline = [&]() { pipeline(state); };

    loop();

    quit();

    return EXIT_SUCCESS;
}
