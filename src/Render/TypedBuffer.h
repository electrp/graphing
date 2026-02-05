//
// Created by willb on 1/26/2026.
//

#ifndef GRAPHING_SDL3WEBGPU_H
#define GRAPHING_SDL3WEBGPU_H

#include <webgpu/webgpu.h>
#include <span>
#include <vector>

#include "ResizeableBuffer.h"
#include "WgpuUtil.h"

template<typename T>
class TypedBuffer {
public:
    explicit TypedBuffer(WGPUDevice device);
    ~TypedBuffer();

    void push(T && data);
    void push(std::span<T> data);
    std::span<T> get_list();
    std::span<const T> get_list() const;
    void clear();

private:
    std::vector<T> cpu;
    ResizeableBuffer* buffer;
};

template<typename T>
TypedBuffer<T>::TypedBuffer(WGPUDevice device)
{
    WGPUBufferDescriptor buffer_descriptor;
    buffer_descriptor.label = wgpuUToView("Debug Renderer Vertex Buffer");
    buffer_descriptor.nextInChain = nullptr;
    buffer_descriptor.size = 4096 * sizeof(T);
    buffer_descriptor.mappedAtCreation = false;
    buffer_descriptor.usage =
        WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex | WGPUBufferUsage_CopySrc | WGPUBufferUsage_Storage;
    buffer = new ResizeableBuffer(device, buffer_descriptor);
}

template<typename T>
TypedBuffer<T>::~TypedBuffer()
{
    delete buffer;
}

template<typename T>
void TypedBuffer<T>::push(T && data)
{
    cpu.emplace_back(std::forward<T>(data));
}

template<typename T>
void TypedBuffer<T>::push(std::span<T> data)
{
    cpu.emplace(cpu.back(), data.begin(), data.end());
}

template<typename T>
std::span<T> TypedBuffer<T>::get_list()
{
    return cpu;
}

template<typename T>
std::span<const T> TypedBuffer<T>::get_list() const
{
    return cpu;
}

template<typename T>
void TypedBuffer<T>::clear()
{
    cpu.clear();
}


#endif //GRAPHING_SDL3WEBGPU_H
