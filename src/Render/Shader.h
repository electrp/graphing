//
// Created by willb on 1/27/2026.
//

#ifndef GRAPHING_SHADER_H
#define GRAPHING_SHADER_H
#include <span>
#include <vector>
#include <webgpu/webgpu.h>

struct AttributeBinding {

};

class Shader {
public:
    Shader();

    std::span<const WGPUVertexAttribute> get_vertex_attributes();

private:
    std::vector<WGPUVertexAttribute> vertex_attributes;

};


#endif //GRAPHING_SHADER_H