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

#include "Entity/EntityManipulator.h"
#include "Graphing/Curve.h"
#include "Graphing/GraphingWindow.h"
#include "Render/Wgpu.h"

static bool is_running = true;
EntityManipulator* em;
GraphingWindow* gw;
flecs::world* w;

const char* imgui_ini = R"(
[Window][Debug##Default]
Pos=60,60
Size=400,400
Collapsed=0

[Window][Dear ImGui Demo]
Pos=900,0
Size=550,680
Collapsed=0

[Window][Hello, world!]
Pos=60,60
Size=339,180
Collapsed=0

[Window][Graphing]
Pos=428,0
Size=1402,965
Collapsed=0
DockId=0x00000002,0

[Window][WindowOverViewport_11111111]
Pos=0,0
Size=1830,965
Collapsed=0

[Window][Entities]
Pos=0,0
Size=426,965
Collapsed=0
DockId=0x00000001,0

[Docking][Data]
DockSpace   ID=0x08BD597D Window=0x1BBC0F80 Pos=0,0 Size=1830,965 Split=X
  DockNode  ID=0x00000001 Parent=0x08BD597D SizeRef=426,800 Selected=0x66870828
  DockNode  ID=0x00000002 Parent=0x08BD597D SizeRef=1402,800 CentralNode=1 Selected=0x2390A690
)";

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

    ImGui::DockSpaceOverViewport();

    // ImGui::ShowDemoWindow();

    bool draw_graphingWindow = true;
    bool draw_entityManipulator = true;

    gw->Draw(draw_graphingWindow);
    em->draw(&draw_entityManipulator, *w);
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
    ImGui::LoadIniSettingsFromMemory(imgui_ini);

    w = new flecs::world();

#ifndef __EMSCRIPTEN__
    w->import<flecs::stats>();
    w->set<flecs::Rest>({});
#endif

    SetupInputPoint(*w);
    SetupTransform(*w);
    SetupCurve(*w);
    gw = new GraphingWindow(*w);
    em = new EntityManipulator(
        w->query_builder<>()
            .with<GraphingWindow::GraphingRelation>(gw->m_host)
            .build()
    );

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