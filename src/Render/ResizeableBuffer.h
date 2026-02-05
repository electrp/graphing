//
// Created by willb on 9/6/2025.
//

#ifndef ENGINE_PROJ_RESIZEABLEBUFFER_H
#define ENGINE_PROJ_RESIZEABLEBUFFER_H
#include "webgpu/webgpu.h"


struct ResizeableBuffer {
    WGPUBuffer buffer;
    WGPUBufferDescriptor descriptor;

    ResizeableBuffer(ResizeableBuffer const& other) = delete;
    ResizeableBuffer(ResizeableBuffer && other) noexcept;
    ResizeableBuffer(WGPUDevice device, WGPUBufferDescriptor _descriptor);
    ResizeableBuffer& operator=(ResizeableBuffer && other) noexcept;
    ~ResizeableBuffer();
    void ensure_size(WGPUDevice device, WGPUCommandEncoder encoder, uint64_t size);
};


#endif //ENGINE_PROJ_RESIZEABLEBUFFER_H