// Dear ImGui: standalone example application for GLFW + WebGPU
// - Emscripten is supported for publishing on web. See https://emscripten.org.
// - Dawn is used as a WebGPU implementation on desktop.

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_wgpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

#include "Graphing/Curve.h"
#include "Graphing/GraphingWindow.h"
#include "Render/Wgpu.h"

static bool is_running = true;
GraphingWindow* gw;
flecs::world* w;

void main_loop() {
    if (false /* QUITTING */) {
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif
        is_running = false;
    }

    bool focused = is_focused();
    poll_events_control(focused);
    if (!focused)
        return;
    if (should_close()) {
        is_running = false;
    }
    imgui_start_frame();

    // ImGui::ShowDemoWindow();

    bool draw = true;
    gw->Draw(draw);
    w->progress();

    imgui_render();
    auto encoder = make_command_encoder();
    auto pass = make_render_pass(encoder);
    imgui_wgpu_render(pass);
    end_render_pass(pass);
    auto cmd_buffer = make_command_buffer(encoder);
    submit_command_encoder(cmd_buffer);
    present_display();
    render_cleanup(pass, encoder, cmd_buffer);
}

// Main code
int main(int, char**)
{
    if (int val = init_window(); val != 0)
        return val;
    init_imgui();
    w = new flecs::world();
    w->import<flecs::stats>();
    w->set<flecs::Rest>({});

    SetupInputPoint(*w);
    SetupTransform(*w);
    SetupCurve(*w);
    gw = new GraphingWindow(*w);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, true);
#else
    while (is_running) {
        main_loop();
    }
#endif

    shutdown_cleanup();
    delete w;
}