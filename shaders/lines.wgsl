#include shaders/render_config.wgsl

@group(0) @binding(0) var<uniform> render_config: RenderConfig;

struct DashingParams {
    dashing_period: f32,
    dashing_weight: f32
}

struct VertexInput {
    @builtin(primitive_index) p_index: u32,
    @location(0) position: vec4<f32>,
    @location(1) color: vec4<f32>,
    @location(2) world_position: vec4<f32>,
    @location(3) dashing: DashingParams
}

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) color: vec4<f32>,
    @location(1) texcoord: f32,
    @location(2) @interpolate(flat) dashing: DashingParams
}

@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
    var out: VertexOutput;
    out.position = render_config.projection * render_config.view * in.position;
    out.color = in.color;
    out.texcoord = in.p_index;
    out.dashing_period = in.dashing_period;
    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {
    var dash = in.texcoord * in.dashing.dashing_period;
    if (modf(dash, 1) >= in.dashing.dashing_weight) {
        discard;
    }

    return in.color;
}