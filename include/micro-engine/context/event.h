//
// Created by kbratko on 2/28/2023.
//

#ifndef MICRO_CONTEXT_EVENT_H
#define MICRO_CONTEXT_EVENT_H

#include <deque>
#include <unordered_map>

#include "view.h"
#include "../core/types.h"
#include "../mathematics/linear.h"

namespace micro::context {
    class Event {
    public:
        bool           quit    = false;
        core::Runnable handler = []() {};

        struct {
            std::unordered_map<int, bool> active{};
            std::deque<int>               pressed{};
        }                                 keyboard{};

        struct {
            bool resized;
            // size of window content area in screen coordinates
            math::vector2<core::i32> size;
        }                            window{};

        struct {
            bool resized;
            // size of framebuffer in pixels (for glViewport)
            math::vector2<core::i32> size;
        }                            framebuffer{};

        struct {
            struct {
                bool                     moved    = false;
                math::vector2<core::f64> position = {0., 0.};
            }                            cursor{};

            struct {
                std::unordered_map<int, bool> active{};
                std::deque<int>               clicked{};
            }                                 button{};

            struct {
                bool scrolled;
                bool xPositive;
                bool xNegative;
                bool yPositive;
                bool yNegative;
            }        scroll{};
        }            mouse{};

        fn init(View *) -> void;

        fn poll() -> void;

        fn handle() -> void;

    private:
        View *view = nullptr;

        static fn windowSizeCallback(GLFWwindow *window, int width, int height) -> void;

        static fn framebufferSizeCallback(GLFWwindow *window, int width, int height) -> void;

        static fn keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) -> void;

        static fn cursorPositionCallback(GLFWwindow *window, double xPos, double yPos) -> void;

        static fn mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) -> void;

        static fn scrollCallback(GLFWwindow *window, double xOffset, double yOffset) -> void;
    };

    fn Event::windowSizeCallback(GLFWwindow *window, int width, int height) -> void {
        void *ptr = glfwGetWindowUserPointer(window);

        if (auto *event = static_cast<Event *>(ptr))
            event->window = {true, {width, height}};
    }

    fn Event::framebufferSizeCallback(GLFWwindow *window, int width, int height) -> void {
        void *ptr = glfwGetWindowUserPointer(window);

        if (auto *event = static_cast<Event *>(ptr))
            event->framebuffer = {true, {width, height}};
    }

    fn Event::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) -> void {
        (void)scancode;
        (void)mods;

        void *ptr = glfwGetWindowUserPointer(window);

        if (auto *event = static_cast<Event *>(ptr))
            switch (action) {
                case GLFW_PRESS:
                    event->keyboard.active[key] = true;
                    break;
                case GLFW_RELEASE:
                    event->keyboard.active[key] = false;
                    event->keyboard.pressed.push_front(key);
                    break;
                default:
                    break;
            }
    }

    fn Event::cursorPositionCallback(GLFWwindow *window, double xPos, double yPos) -> void {
        void *ptr = glfwGetWindowUserPointer(window);

        if (auto *event = static_cast<Event *>(ptr))
            event->mouse.cursor = {true, {xPos, yPos}};
    }

    fn Event::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) -> void {
        (void)mods;

        void *ptr = glfwGetWindowUserPointer(window);

        if (auto *event = static_cast<Event *>(ptr))
            switch (action) {
                case GLFW_PRESS:
                    event->keyboard.active[button] = true;
                    break;
                case GLFW_RELEASE:
                    event->keyboard.active[button] = false;
                    event->keyboard.pressed.push_front(button);
                    break;
                default:
                    break;
            }
    }

    fn Event::scrollCallback(GLFWwindow *window, double xOffset, double yOffset) -> void {
        void *ptr = glfwGetWindowUserPointer(window);

        if (auto *event = static_cast<Event *>(ptr))
            event->mouse.scroll = {
                true,
                xOffset > 0.99,
                xOffset < -0.99,
                yOffset > 0.99,
                yOffset < -0.99
            };
    }

    fn Event::init(View *_view) -> void {
        view = _view;

        glfwGetWindowSize(view->window, &window.size.x, &window.size.y);
        glfwGetFramebufferSize(view->window, &framebuffer.size.x, &framebuffer.size.y);
        glfwGetCursorPos(view->window, &mouse.cursor.position.x, &mouse.cursor.position.y);

        glfwSetWindowUserPointer(view->window, this);
        glfwSetWindowSizeCallback(view->window, Event::windowSizeCallback);
        glfwSetFramebufferSizeCallback(view->window, Event::framebufferSizeCallback);
        glfwSetKeyCallback(view->window, Event::keyCallback);
        glfwSetCursorPosCallback(view->window, Event::cursorPositionCallback);
        glfwSetMouseButtonCallback(view->window, Event::mouseButtonCallback);
        glfwSetScrollCallback(view->window, Event::scrollCallback);
    }

    fn Event::poll() -> void {
        quit = glfwWindowShouldClose(view->window);
        glfwPollEvents();
    }

    fn Event::handle() -> void {
        if (window.resized)
            view->size = window.size;

        if (framebuffer.resized)
            view->viewport = framebuffer.size;

        handler();

        if (!keyboard.pressed.empty()) {
            if (keyboard.pressed.back() == GLFW_KEY_ESCAPE)
                glfwSetWindowShouldClose(view->window, 1);

            if (keyboard.pressed.back() == GLFW_KEY_F11)
                view->toggleFullscreen(!view->isFullscreen());

            keyboard.pressed.pop_back();
        }

        if (!mouse.button.clicked.empty()) mouse.button.clicked.pop_back();

        if (window.resized && framebuffer.resized) view->viewportChanged();

        window.resized      = false;
        framebuffer.resized = false;
        mouse.cursor.moved  = false;
        mouse.scroll        = {};
    }
}

#endif //MICRO_CONTEXT_EVENT_H
