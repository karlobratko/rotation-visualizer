//
// Created by kbratko on 8/10/2023.
//

#ifndef MICRO_CONTEXT_BASE_H
#define MICRO_CONTEXT_BASE_H

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <csignal>

#include "event.h"
#include "view.h"
#include "../utils.h"

namespace micro::context {
    static View  view{};
    static Event event{};

    fn sighandler(int signal) -> void {
        (void)signal;
        event.quit = true;
    }

    fn window(core::cstring title, int width, int height, bool initGui = true) -> void {
        if (glfwInit() == GLFW_FALSE)
            throw std::runtime_error("Couldn't initialize GLFW.");

        signal(SIGINT, &sighandler);

        view.init(title, width, height);
        event.init(&view);

        if (initGui)
            view.initGui();
    }

    fn quit() -> void {
        if (view.isEnabled()) view.quit();

        glfwTerminate();
    }

    fn loop(core::Runnable const &function = []() {}) -> void {
        auto oldTime = utils::time<core::f64>();

        if (view.isEnabled())
            while (!event.quit) {
                auto currentTime = utils::time<core::f64>();
                view.deltaTime   = currentTime - oldTime;
                oldTime          = currentTime;

                event.handle();

                function();

                view.render();

                event.poll();
            }
    }
}

#endif //MICRO_CONTEXT_INIT_H
