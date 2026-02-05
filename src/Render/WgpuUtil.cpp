//
// Created by willb on 9/5/2025.
//

#include "WgpuUtil.h"

std::string_view wgpuUFromView(WGPUStringView str) {
    return std::string_view {str.data, str.length};
}

WGPUStringView wgpuUToView(std::string_view str) {
    return WGPUStringView {str.data(), str.length()};
}
