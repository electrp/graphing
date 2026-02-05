//
// Created by willb on 2/2/2026.
//

#ifndef GRAPHING_WGPU_H
#define GRAPHING_WGPU_H

#include "imgui_impl_glfw.h"
#include "imgui_impl_wgpu.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#if defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU)
#include <emscripten/html5_webgpu.h>
#endif
#endif

#include <webgpu/webgpu.h>
#if defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
#include <webgpu/webgpu_cpp.h>
#endif

int init_window();
void init_imgui();
bool is_focused();
void poll_events_control(bool& focused);
bool should_close();
void imgui_start_frame();
void imgui_render();
WGPUCommandEncoder make_command_encoder();
WGPURenderPassEncoder make_render_pass(WGPUCommandEncoder encoder);
void imgui_wgpu_render(WGPURenderPassEncoder pass);
void end_render_pass(WGPURenderPassEncoder pass);
WGPUCommandBuffer make_command_buffer(WGPUCommandEncoder encoder);
void submit_command_encoder(WGPUCommandBuffer cmd_buffer);
void present_display();
void render_cleanup(WGPURenderPassEncoder pass, WGPUCommandEncoder encoder, WGPUCommandBuffer cmd_buffer);
void shutdown_cleanup();

#endif //GRAPHING_WGPU_H