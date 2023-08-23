//
// Created by kbratko on 2/28/2023.
//

#ifndef MICRO_CONTEXT_VIEW_H
#define MICRO_CONTEXT_VIEW_H

#include <glad/glad.h>
#include "glfw/glfw3.h"

#include <array>
#include <iostream>

#include "imgui.h"
#include "../core/types.h"
#include "../mathematics/linear.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace micro::context {
    class View {
    public:
        GLFWwindow *window = nullptr;
        ImGuiIO *   gui    = nullptr;

        bool  showGui   = false;
        bool  vsync     = true;
        bool  ccw       = true;
        int   antialias = 16;
        float lineWidth = 1.f;
        float pointSize = 1.f;

        math::vector2<core::i32> position{};
        math::vector2<core::i32> size{};
        math::vector2<core::i32> viewport{};

        core::f64 deltaTime = 0.;

        core::Runnable pipeline = []() {};

        core::Runnable interface = []() {};

        fn viewportChanged() -> void;

        [[nodiscard]] fn isFullscreen() const -> bool;

        fn toggleFullscreen(bool fullscreen) -> void;

        [[nodiscard]] fn isEnabled() const -> bool;

        fn init(core::cstring title, int width, int height) -> void;

        fn initGui() -> void;

        fn quit() -> void;

        fn render() const -> void;

        fn drawGui() const -> void;

        fn target(math::vector4<core::f32> const &clearColor, bool clearColorBit = true,
                  bool                            clearDepthBit                  = true) -> void;

        fn target(math::vector3<core::f32> const &clearColor, bool clearColorBit = true,
                  bool                            clearDepthBit                  = true) -> void;

    private:
        GLFWmonitor *monitor = nullptr;

        bool updateViewport = true;
        bool fullscreen     = false;
        bool enabled        = false;
        bool guiEnabled     = false;
    };

    fn View::viewportChanged() -> void { updateViewport = true; }

    fn View::isFullscreen() const -> bool { return fullscreen; }

    fn View::init(core::cstring _title, int _width, int _height) -> void {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        if (antialias)
            glfwWindowHint(GLFW_SAMPLES, antialias);

        window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);
        if (window == nullptr)
            throw std::runtime_error("Couldn't create window.");

        monitor = glfwGetPrimaryMonitor();
        glfwGetWindowSize(window, &size.x, &size.y);
        glfwGetWindowPos(window, &position.x, &position.y);
        glfwGetFramebufferSize(window, &viewport.x, &viewport.y);
        updateViewport = true;

        glfwSwapInterval(vsync ? 1 : 0);
        glfwMakeContextCurrent(window);

        gladLoadGL(glfwGetProcAddress);

        if (antialias)
            glEnable(GL_MULTISAMPLE);

        glEnable(GL_DEPTH_TEST);
        //glDepthFunc(GL_NOTEQUAL);

        glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glFrontFace(ccw ? GL_CCW : GL_CW);

        glLineWidth(lineWidth);
        glPointSize(pointSize);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        enabled = true;
    }

    auto View::initGui() -> void {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        gui = &ImGui::GetIO();
        (void)gui;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");

        guiEnabled = true;
    }

    fn View::quit() -> void {
        if (guiEnabled && gui != nullptr) {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            gui = nullptr;
        }

        glfwDestroyWindow(window);
        window = nullptr;
    }

    fn View::toggleFullscreen(bool _fullscreen) -> void {
        if (fullscreen == _fullscreen) return;

        if (_fullscreen) {
            glfwGetWindowSize(window, &size.x, &size.y);
            glfwGetWindowPos(window, &position.x, &position.y);

            GLFWvidmode const *mode = glfwGetVideoMode(monitor);

            glfwSetWindowMonitor(window,
                                 monitor,
                                 0,
                                 0,
                                 mode->width,
                                 mode->height,
                                 mode->refreshRate);
        }
        else
            glfwSetWindowMonitor(window,
                                 nullptr,
                                 position.x,
                                 position.y,
                                 size.x,
                                 size.y,
                                 GLFW_DONT_CARE);

        fullscreen     = _fullscreen;
        updateViewport = true;
        glfwGetFramebufferSize(window, &viewport.x, &viewport.y);
    }

    fn View::isEnabled() const -> bool { return enabled; }

    fn View::render() const -> void {
        pipeline();

        if (guiEnabled && showGui)
            drawGui();

        glfwSwapBuffers(window);
    }

    fn View::drawGui() const -> void {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        interface();

        ImGui::Render();
        glViewport(0, 0, static_cast<GLsizei>(gui->DisplaySize.x), static_cast<GLsizei>(gui->DisplaySize.y));
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    fn View::target(math::vector4<core::f32> const &_clearColor, bool _clearColorBit, bool _clearDepthBit) -> void {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (updateViewport) {
            glViewport(0, 0, viewport.x, viewport.y);
            updateViewport = false;
        }

        glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
        if (_clearColorBit)
            glClear(GL_COLOR_BUFFER_BIT);
        if (_clearDepthBit)
            glClear(GL_DEPTH_BUFFER_BIT);
    }

    fn View::target(math::vector3<core::f32> const &_clearColor, bool _clearColorBit, bool _clearDepthBit) -> void { target(math::vector4<core::f32>{_clearColor, 1.f}, _clearColorBit, _clearDepthBit); }
}

#endif //MICRO_CONTEXT_VIEW_H
