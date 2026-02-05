//
// Created by willb on 9/6/2025.
//

#include "ResizeableBuffer.h"

ResizeableBuffer::ResizeableBuffer(ResizeableBuffer &&other) noexcept {
    buffer = other.buffer;
    descriptor = other.descriptor;
}

ResizeableBuffer::ResizeableBuffer(WGPUDevice device, WGPUBufferDescriptor _descriptor) : descriptor(_descriptor) {
    buffer = wgpuDeviceCreateBuffer(device, &descriptor);
}

ResizeableBuffer & ResizeableBuffer::operator=(ResizeableBuffer &&other) noexcept {
    buffer = other.buffer;
    descriptor = other.descriptor;
    other.buffer = nullptr;
    return *this;
}

ResizeableBuffer::~ResizeableBuffer() {
    if (buffer != nullptr)
        wgpuBufferRelease(buffer);
}

void ResizeableBuffer::ensure_size(WGPUDevice device, WGPUCommandEncoder encoder, uint64_t size) {
    if (descriptor.size < size) {
        uint64_t old_size = descriptor.size;
        // Resize buffer to double the difference in size
        descriptor.size += (size - descriptor.size) * 2;

        WGPUBuffer repl = wgpuDeviceCreateBuffer(device, &descriptor);
        wgpuCommandEncoderCopyBufferToBuffer(encoder, buffer, 0, repl, 0, old_size);
        wgpuBufferRelease(buffer);

        buffer = repl;
    }
}
