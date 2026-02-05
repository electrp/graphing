//
// Created by willb on 9/5/2025.
//
// Contains some utils for webgpu, and all required incldues

#ifndef ENGINE_PROJ_WGPU_UTIL_H
#define ENGINE_PROJ_WGPU_UTIL_H
#include <webgpu/webgpu.h>

#include <string_view>

std::string_view wgpuUFromView(WGPUStringView str);
WGPUStringView wgpuUToView(std::string_view str);

#endif //ENGINE_PROJ_WGPU_UTIL_H
