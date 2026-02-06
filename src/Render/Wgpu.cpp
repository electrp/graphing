//
// Created by willb on 2/2/2026.
//

#include <stdio.h>
#include <stdlib.h>
#include "Wgpu.h"

#include "GLFW/glfw3.h"


static WGPUInstance             wgpu_instance = nullptr;
static WGPUDevice               wgpu_device = nullptr;
static WGPUSurface              wgpu_surface = nullptr;
static WGPUQueue                wgpu_queue = nullptr;
static WGPUSurfaceConfiguration wgpu_surface_configuration = {};
static int                      wgpu_surface_width = 1280;
static int                      wgpu_surface_height = 800;

static float                    main_scale = -1;
static GLFWwindow*              window = nullptr;
static WGPUSurfaceTexture       surface_texture {};
static WGPUTextureView          texture_view;

WGPUSurface CreateWGPUSurface(const WGPUInstance& instance, GLFWwindow* window);

#if defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
static WGPUAdapter RequestAdapter(wgpu::Instance& instance);
static WGPUDevice RequestDevice(wgpu::Instance& instance, wgpu::Adapter& adapter);
#else
static WGPUAdapter RequestAdapter(WGPUInstance& instance);
static WGPUDevice RequestDevice(WGPUAdapter& adapter);
#endif


static void glfw_error_callback(int error, const char* description)
{
    printf("GLFW Error %d: %s\n", error, description);
}

static void ResizeSurface(int width, int height)
{
    wgpu_surface_configuration.width = wgpu_surface_width = width;
    wgpu_surface_configuration.height = wgpu_surface_height = height;
    wgpuSurfaceConfigure(wgpu_surface, &wgpu_surface_configuration);
}


static bool InitWGPU(GLFWwindow* window)
{
    WGPUTextureFormat preferred_fmt = WGPUTextureFormat_Undefined;  // acquired from SurfaceCapabilities

    // Google DAWN backend: Adapter and Device acquisition, Surface creation
#if defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
    wgpu::InstanceDescriptor instance_desc = {};
    static constexpr wgpu::InstanceFeatureName timedWaitAny = wgpu::InstanceFeatureName::TimedWaitAny;
    instance_desc.requiredFeatureCount = 1;
    instance_desc.requiredFeatures = &timedWaitAny;
    wgpu::Instance instance = wgpu::CreateInstance(&instance_desc);

    wgpu::Adapter adapter = RequestAdapter(instance);
    ImGui_ImplWGPU_DebugPrintAdapterInfo(adapter.Get());

    wgpu_device = RequestDevice(instance, adapter);

    // Create the surface.
#ifdef __EMSCRIPTEN__
    wgpu::EmscriptenSurfaceSourceCanvasHTMLSelector canvas_desc = {};
    canvas_desc.selector = "#canvas";

    wgpu::SurfaceDescriptor surface_desc = {};
    surface_desc.nextInChain = &canvas_desc;
    wgpu_surface = instance.CreateSurface(&surface_desc).MoveToCHandle();
#else
    wgpu_surface = CreateWGPUSurface(instance.Get(), window);
#endif
    if (!wgpu_surface)
        return false;

    // Moving Dawn objects into WGPU handles
    wgpu_instance = instance.MoveToCHandle();

    WGPUSurfaceCapabilities surface_capabilities = {};
    wgpuSurfaceGetCapabilities(wgpu_surface, adapter.Get(), &surface_capabilities);

    preferred_fmt = surface_capabilities.formats[0];

    // WGPU backend: Adapter and Device acquisition, Surface creation
#elif defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU)
    wgpu_instance = wgpuCreateInstance(nullptr);

#ifdef __EMSCRIPTEN__
    getAdapterAndDeviceViaJS();

    wgpu_device = emscripten_webgpu_get_device();
    IM_ASSERT(wgpu_device != nullptr && "Error creating the Device");

    WGPUSurfaceDescriptorFromCanvasHTMLSelector html_surface_desc = {};
    html_surface_desc.chain.sType = WGPUSType_SurfaceDescriptorFromCanvasHTMLSelector;
    html_surface_desc.selector = "#canvas";

    WGPUSurfaceDescriptor surface_desc = {};
    surface_desc.nextInChain = &html_surface_desc.chain;

    // Create the surface.
    wgpu_surface = wgpuInstanceCreateSurface(wgpu_instance, &surface_desc);
    preferred_fmt = wgpuSurfaceGetPreferredFormat(wgpu_surface, {} /* adapter */);
#else // __EMSCRIPTEN__
    wgpuSetLogCallback(
        [](WGPULogLevel level, WGPUStringView msg, void* userdata) { fprintf(stderr, "%s: %.*s\n", ImGui_ImplWGPU_GetLogLevelName(level), (int)msg.length, msg.data); }, nullptr
    );
    wgpuSetLogLevel(WGPULogLevel_Warn);

    WGPUAdapter adapter = RequestAdapter(wgpu_instance);
    ImGui_ImplWGPU_DebugPrintAdapterInfo(adapter);

    wgpu_device = RequestDevice(adapter);

    // Create the surface.
    wgpu_surface = CreateWGPUSurface(wgpu_instance, window);
    if (!wgpu_surface)
        return false;

    WGPUSurfaceCapabilities surface_capabilities = {};
    wgpuSurfaceGetCapabilities(wgpu_surface, adapter, &surface_capabilities);

    preferred_fmt = surface_capabilities.formats[0];
#endif // __EMSCRIPTEN__
#endif // IMGUI_IMPL_WEBGPU_BACKEND_WGPU

    wgpu_surface_configuration.presentMode = WGPUPresentMode_Fifo;
    wgpu_surface_configuration.alphaMode = WGPUCompositeAlphaMode_Auto;
    wgpu_surface_configuration.usage = WGPUTextureUsage_RenderAttachment;
    wgpu_surface_configuration.width = wgpu_surface_width;
    wgpu_surface_configuration.height = wgpu_surface_height;
    wgpu_surface_configuration.device = wgpu_device;
    wgpu_surface_configuration.format = preferred_fmt;

    wgpuSurfaceConfigure(wgpu_surface, &wgpu_surface_configuration);
    wgpu_queue = wgpuDeviceGetQueue(wgpu_device);

    return true;
}

int init_window()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Make sure GLFW does not initialize any graphics context.
    // This needs to be done explicitly later.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Create window
    main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
    wgpu_surface_width *= main_scale;
    wgpu_surface_height *= main_scale;
    window = glfwCreateWindow(wgpu_surface_width, wgpu_surface_height, "Graphing", nullptr, nullptr);
    if (window == nullptr)
        return 1;

    // Initialize the WebGPU environment
    if (!InitWGPU(window))
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    glfwShowWindow(window);

    return 0;
}

void init_imgui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOther(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplWGPU_InitInfo init_info;
    init_info.Device = wgpu_device;
    init_info.NumFramesInFlight = 3;
    init_info.RenderTargetFormat = wgpu_surface_configuration.format;
    init_info.DepthStencilFormat = WGPUTextureFormat_Undefined;
    ImGui_ImplWGPU_Init(&init_info);

}

bool is_focused() {
    return glfwGetWindowAttrib(window, GLFW_ICONIFIED) == 0;
}

void poll_events_control(bool& focused) {
    glfwPollEvents();
    if (!focused)
    {
        ImGui_ImplGlfw_Sleep(10);
        return;
    }

    // React to changes in screen size
    int width, height;
    glfwGetFramebufferSize((GLFWwindow*)window, &width, &height);
    if (width == 0 || height == 0) {
        focused = false;
        return;
    }
    if (width != wgpu_surface_width || height != wgpu_surface_height)
        ResizeSurface(width, height);

    // Check surface status for error. If texture is not optimal, try to reconfigure the surface.
    wgpuSurfaceGetCurrentTexture(wgpu_surface, &surface_texture);
    if (ImGui_ImplWGPU_IsSurfaceStatusError(surface_texture.status))
    {
        fprintf(stderr, "Unrecoverable Surface Texture status=%#.8x\n", surface_texture.status);
        abort();
    }
    if (ImGui_ImplWGPU_IsSurfaceStatusSubOptimal(surface_texture.status))
    {
        if (surface_texture.texture)
            wgpuTextureRelease(surface_texture.texture);
        if (width > 0 && height > 0)
            ResizeSurface(width, height);
        return;
    }
}

bool should_close() {
    return glfwWindowShouldClose(window);
}

void imgui_start_frame() {
    ImGui_ImplWGPU_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void imgui_render() {
    ImGui::Render();
}

WGPUCommandEncoder make_command_encoder() {
    WGPUCommandEncoderDescriptor enc_desc = {};
    return wgpuDeviceCreateCommandEncoder(wgpu_device, &enc_desc);
}

WGPURenderPassEncoder make_render_pass(WGPUCommandEncoder encoder) {
    WGPUTextureViewDescriptor view_desc = {};
    view_desc.format = wgpu_surface_configuration.format;
    view_desc.dimension = WGPUTextureViewDimension_2D ;
    view_desc.mipLevelCount = WGPU_MIP_LEVEL_COUNT_UNDEFINED;
    view_desc.arrayLayerCount = WGPU_ARRAY_LAYER_COUNT_UNDEFINED;
    view_desc.aspect = WGPUTextureAspect_All;

    texture_view = wgpuTextureCreateView(surface_texture.texture, &view_desc);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    WGPURenderPassColorAttachment color_attachments = {};
    color_attachments.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
    color_attachments.loadOp = WGPULoadOp_Clear;
    color_attachments.storeOp = WGPUStoreOp_Store;
    color_attachments.clearValue = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    color_attachments.view = texture_view;

    WGPURenderPassDescriptor render_pass_desc = {};
    render_pass_desc.colorAttachmentCount = 1;
    render_pass_desc.colorAttachments = &color_attachments;
    render_pass_desc.depthStencilAttachment = nullptr;

    return wgpuCommandEncoderBeginRenderPass(encoder, &render_pass_desc);
}

void imgui_wgpu_render(WGPURenderPassEncoder pass) {
    ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), pass);
}

void end_render_pass(WGPURenderPassEncoder pass) {
    wgpuRenderPassEncoderEnd(pass);
}

WGPUCommandBuffer make_command_buffer(WGPUCommandEncoder encoder) {
    WGPUCommandBufferDescriptor cmd_buffer_desc = {};
    return wgpuCommandEncoderFinish(encoder, &cmd_buffer_desc);
}

void submit_command_encoder(WGPUCommandBuffer cmd_buffer) {
    wgpuQueueSubmit(wgpu_queue, 1, &cmd_buffer);
}

void present_display() {
#ifndef __EMSCRIPTEN__
    wgpuSurfacePresent(wgpu_surface);
    // Tick needs to be called in Dawn to display validation errors
#if defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
    wgpuDeviceTick(wgpu_device);
#endif
#endif
}

void render_cleanup(WGPURenderPassEncoder pass, WGPUCommandEncoder encoder, WGPUCommandBuffer cmd_buffer) {
    wgpuTextureViewRelease(texture_view);
    wgpuRenderPassEncoderRelease(pass);
    wgpuCommandEncoderRelease(encoder);
    wgpuCommandBufferRelease(cmd_buffer);
}

void shutdown_cleanup() {
    ImGui_ImplWGPU_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    wgpuSurfaceUnconfigure(wgpu_surface);
    wgpuSurfaceRelease(wgpu_surface);
    wgpuQueueRelease(wgpu_queue);
    wgpuDeviceRelease(wgpu_device);
    wgpuInstanceRelease(wgpu_instance);

    glfwDestroyWindow(window);
    glfwTerminate();

    return;
}

#if defined(IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
static WGPUAdapter RequestAdapter(wgpu::Instance& instance)
{
    wgpu::Adapter acquired_adapter;
    wgpu::RequestAdapterOptions adapter_options;
    auto onRequestAdapter = [&](wgpu::RequestAdapterStatus status, wgpu::Adapter adapter, wgpu::StringView message)
    {
        if (status != wgpu::RequestAdapterStatus::Success)
        {
            printf("Failed to get an adapter: %s\n", message.data);
            return;
        }
        acquired_adapter = std::move(adapter);
    };

    // Synchronously (wait until) acquire Adapter
    wgpu::Future waitAdapterFunc { instance.RequestAdapter(&adapter_options, wgpu::CallbackMode::WaitAnyOnly, onRequestAdapter) };
    wgpu::WaitStatus waitStatusAdapter = instance.WaitAny(waitAdapterFunc, UINT64_MAX);
    IM_ASSERT(acquired_adapter != nullptr && waitStatusAdapter == wgpu::WaitStatus::Success && "Error on Adapter request");
    return acquired_adapter.MoveToCHandle();
}

static WGPUDevice RequestDevice(wgpu::Instance& instance, wgpu::Adapter& adapter)
{
    // Set device callback functions
    wgpu::DeviceDescriptor device_desc;
    device_desc.SetDeviceLostCallback(wgpu::CallbackMode::AllowSpontaneous,
        [](const wgpu::Device&, wgpu::DeviceLostReason type, wgpu::StringView msg) { fprintf(stderr, "%s error: %s\n", ImGui_ImplWGPU_GetDeviceLostReasonName((WGPUDeviceLostReason)type), msg.data); }
    );
    device_desc.SetUncapturedErrorCallback(
        [](const wgpu::Device&, wgpu::ErrorType type, wgpu::StringView msg) { fprintf(stderr, "%s error: %s\n", ImGui_ImplWGPU_GetErrorTypeName((WGPUErrorType)type), msg.data); }
    );

    wgpu::Device acquired_device;
    auto onRequestDevice = [&](wgpu::RequestDeviceStatus status, wgpu::Device local_device, wgpu::StringView message)
    {
        if (status != wgpu::RequestDeviceStatus::Success)
        {
            printf("Failed to get an device: %s\n", message.data);
            return;
        }
        acquired_device = std::move(local_device);
    };

    // Synchronously (wait until) get Device
    wgpu::Future waitDeviceFunc { adapter.RequestDevice(&device_desc, wgpu::CallbackMode::WaitAnyOnly, onRequestDevice) };
    wgpu::WaitStatus waitStatusDevice = instance.WaitAny(waitDeviceFunc, UINT64_MAX);
    IM_ASSERT(acquired_device != nullptr && waitStatusDevice == wgpu::WaitStatus::Success && "Error on Device request");
    return acquired_device.MoveToCHandle();
}
#elif defined(IMGUI_IMPL_WEBGPU_BACKEND_WGPU)
#ifdef __EMSCRIPTEN__
// Adapter and device initialization via JS
EM_ASYNC_JS( void, getAdapterAndDeviceViaJS, (),
{
    if (!navigator.gpu)
        throw Error("WebGPU not supported.");
    const adapter = await navigator.gpu.requestAdapter();
    const device = await adapter.requestDevice();
    Module.preinitializedWebGPUDevice = device;
} );
#else // __EMSCRIPTEN__
static void handle_request_adapter(WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message, void* userdata1, void* userdata2)
{
    if (status == WGPURequestAdapterStatus_Success)
    {
        WGPUAdapter* extAdapter = (WGPUAdapter*)userdata1;
        *extAdapter = adapter;
    }
    else
    {
        printf("Request_adapter status=%#.8x message=%.*s\n", status, (int) message.length, message.data);
    }
}

static void handle_request_device(WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView message, void* userdata1, void* userdata2)
{
    if (status == WGPURequestDeviceStatus_Success)
    {
        WGPUDevice* extDevice = (WGPUDevice*)userdata1;
        *extDevice = device;
    }
    else
    {
        printf("Request_device status=%#.8x message=%.*s\n", status, (int) message.length, message.data);
    }
}

static WGPUAdapter RequestAdapter(WGPUInstance& instance)
{
    WGPURequestAdapterOptions adapter_options = {};

    WGPUAdapter local_adapter;
    WGPURequestAdapterCallbackInfo adapterCallbackInfo = {};
    adapterCallbackInfo.callback = handle_request_adapter;
    adapterCallbackInfo.userdata1 = &local_adapter;

    wgpuInstanceRequestAdapter(instance, &adapter_options, adapterCallbackInfo);
    IM_ASSERT(local_adapter && "Error on Adapter request");
    return local_adapter;
}

static WGPUDevice RequestDevice(WGPUAdapter& adapter)
{
    WGPUDevice local_device;
    WGPURequestDeviceCallbackInfo deviceCallbackInfo = {};
    deviceCallbackInfo.callback = handle_request_device;
    deviceCallbackInfo.userdata1 = &local_device;
    wgpuAdapterRequestDevice(adapter, nullptr, deviceCallbackInfo);
    IM_ASSERT(local_device && "Error on Device request");
    return local_device;
}
#endif // __EMSCRIPTEN__
#endif // IMGUI_IMPL_WEBGPU_BACKEND_WGPU

// GLFW helper to create a WebGPU surface, used only in WGPU-Native. DAWN-Native already has a built-in function
// As of today (2025/10) there is no "official" support in GLFW to create a surface for WebGPU backend
// This stub uses "low level" GLFW calls to acquire information from a specific Window Manager.
// Currently supported platforms: Windows / Linux (X11 and Wayland) / MacOS. Not necessary nor available with EMSCRIPTEN.
#ifndef __EMSCRIPTEN__

#if defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
#define GLFW_HAS_X11_OR_WAYLAND     1
#else
#define GLFW_HAS_X11_OR_WAYLAND     0
#endif
#ifdef _WIN32
#undef APIENTRY
#ifndef GLFW_EXPOSE_NATIVE_WIN32    // for glfwGetWin32Window()
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#elif defined(__APPLE__)
#ifndef GLFW_EXPOSE_NATIVE_COCOA    // for glfwGetCocoaWindow()
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#elif GLFW_HAS_X11_OR_WAYLAND
#ifndef GLFW_EXPOSE_NATIVE_X11      // for glfwGetX11Display(), glfwGetX11Window() on Freedesktop (Linux, BSD, etc.)
#define GLFW_EXPOSE_NATIVE_X11
#endif
#ifndef GLFW_EXPOSE_NATIVE_WAYLAND
#if defined(__has_include) && __has_include(<wayland-client.h>)
#define GLFW_EXPOSE_NATIVE_WAYLAND
#endif
#endif
#endif
#include <GLFW/glfw3native.h>
#undef Status                       // X11 headers are leaking this and also 'Success', 'Always', 'None', all used in DAWN api. Add #undef if necessary.

WGPUSurface CreateWGPUSurface(const WGPUInstance& instance, GLFWwindow* window)
{
    ImGui_ImplWGPU_CreateSurfaceInfo create_info = {};
    create_info.Instance = instance;
#if defined(GLFW_EXPOSE_NATIVE_COCOA)
    {
        create_info.System = "cocoa";
        create_info.RawWindow = (void*)glfwGetCocoaWindow(window);
        return ImGui_ImplWGPU_CreateWGPUSurfaceHelper(&create_info);
    }
#elif defined(GLFW_EXPOSE_NATIVE_WAYLAND)
    if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND)
    {
        create_info.System = "wayland";
        create_info.RawDisplay = (void*)glfwGetWaylandDisplay();
        create_info.RawSurface = (void*)glfwGetWaylandWindow(window);
        return ImGui_ImplWGPU_CreateWGPUSurfaceHelper(&create_info);
    }
#elif defined(GLFW_EXPOSE_NATIVE_X11)
    if (glfwGetPlatform() == GLFW_PLATFORM_X11)
    {
        create_info.System = "x11";
        create_info.RawWindow = (void*)glfwGetX11Window(window);
        create_info.RawDisplay = (void*)glfwGetX11Display();
        return ImGui_ImplWGPU_CreateWGPUSurfaceHelper(&create_info);
    }
#elif defined(GLFW_EXPOSE_NATIVE_WIN32)
    {
        create_info.System = "win32";
        create_info.RawWindow = (void*)glfwGetWin32Window(window);
        create_info.RawInstance = (void*)::GetModuleHandle(NULL);
        return ImGui_ImplWGPU_CreateWGPUSurfaceHelper(&create_info);
    }
#else
#error "Unsupported WebGPU native platform!"
#endif
    return nullptr;
}
#endif // #ifndef __EMSCRIPTEN__