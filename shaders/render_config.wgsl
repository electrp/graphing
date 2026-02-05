#include shaders/data.wgsl

struct RenderConfig {
    position: Transform,
    view: mat4x4<f32>,
    projection: mat4x4<f32>,
}