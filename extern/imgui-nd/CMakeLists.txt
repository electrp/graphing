# Define an ImGui target that fits our use case


add_library(imgui STATIC
        # Among the different backends available, we are interested in connecting
        # the GUI to GLFW andWebGPU:
        backends/imgui_impl_wgpu.h
        backends/imgui_impl_wgpu.cpp
        backends/imgui_impl_glfw.cpp
        backends/imgui_impl_glfw.cpp

        # Bonus to add some C++ specific features (the core ImGUi is a C library)
        misc/cpp/imgui_stdlib.h
        misc/cpp/imgui_stdlib.cpp

        # The core ImGui files
        imconfig.h
        imgui.h
        imgui.cpp
        imgui_draw.cpp
        imgui_internal.h
        imgui_tables.cpp
        imgui_widgets.cpp
        imgui_demo.cpp
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
)

if(NOT EMSCRIPTEN)
    target_compile_definitions(imgui PUBLIC IMGUI_IMPL_WEBGPU_BACKEND_WGPU)
else()
    target_compile_definitions(imgui PUBLIC IMGUI_IMPL_WEBGPU_BACKEND_DAWN)
    target_compile_options(imgui PUBLIC
            "--use-port=emdawnwebgpu"
    )
endif()


target_include_directories(imgui PUBLIC .)
target_link_libraries(imgui PUBLIC webgpu glfw glm)