cmake_minimum_required(VERSION 3.16)
include(ExternalProject)

project(graphing LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT EMSCRIPTEN)
    add_subdirectory(extern/glfw)

    set(LUAJIT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/LuaJIT)
    add_subdirectory(extern/luajit-cmake)
else()
    file(GLOB LUA_SRCS extern/lua/src/*.c)
    file(GLOB LUA_HEADERS extern/lua/src/*.h)
    add_library(lua ${LUA_SRCS} ${LUA_HEADERS})
endif()

add_executable(graphing
    src/main.cpp
    src/Render/TypedBuffer.h
    src/Platform/Platform.h
    src/Platform/Emscripten.cpp
    src/Shaders/ShaderPreprocess.cpp
    src/Shaders/ShaderPreprocess.h
    extern/flecs/flecs.c
    extern/flecs/flecs.h
    src/Render/ResizeableBuffer.cpp
    src/Render/ResizeableBuffer.h
    src/Render/TypedBuffer.h
    src/Render/WgpuUtil.cpp
    src/Render/WgpuUtil.h
    src/Render/Shader.cpp
    src/Render/Shader.h
    src/Render/Attribute.h
    src/Entity/EntityManipulator.cpp
    src/Entity/EntityManipulator.h
    src/Script/Lua.cpp
    src/Script/Lua.h
    src/Script/Script.cpp
    src/Script/Script.h
        src/Render/Wgpu.cpp
        src/Render/Wgpu.h
        src/Graphing/GraphingWindow.cpp
        src/Graphing/GraphingWindow.h
        src/Entity/Window.cpp
        src/Entity/Window.h
        src/Graphing/InputPoint.cpp
        src/Graphing/InputPoint.h
        src/Entity/Transform.cpp
        src/Entity/Transform.h
)

if(EMSCRIPTEN)
    set_target_properties(graphing PROPERTIES SUFFIX ".html")
endif()

if(WIN32)
    add_library(webgpu STATIC IMPORTED)
    set_target_properties(webgpu PROPERTIES
            IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/extern/wgpu-windows-x86_64-msvc-release/lib/wgpu_native.dll.lib" # Or .a for Linux/macOS
            INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/extern/wgpu-windows-x86_64-msvc-release/include"
    )
else()
    if(EMSCRIPTEN)
        add_library(webgpu INTERFACE)
#        set_target_properties(webgpu PROPERTIES
#                INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/extern/wgpu-windows-x86_64-msvc-release/include"
#        )
    endif()
endif()
target_include_directories(graphing PUBLIC extern/imgui extern/imgui/backends/ extern/flecs src/)

if(NOT EMSCRIPTEN)
    target_include_directories(graphing PUBLIC extern/wgpu-windows-x86_64-msvc-release/include)
endif()

add_subdirectory(extern/imgui)
add_subdirectory(extern/glm)

target_link_libraries(graphing glfw webgpu imgui glm)
if(EMSCRIPTEN)
    target_link_libraries(graphing lua)
    target_include_directories(graphing PRIVATE extern/lua/src)
else()
    target_link_libraries(graphing luajit::lib luajit::header)
endif()

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/shaders/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/shaders/")

if(EMSCRIPTEN)
    # Add necessary Emscripten link options
    target_compile_options(graphing PUBLIC
            "--use-port=emdawnwebgpu"
    )

    target_link_options(graphing PRIVATE
            "--use-port=emdawnwebgpu"
            "-sUSE_GLFW=3"
            "-sASYNCIFY=1"
            "-closure=1"
    )
endif()

if(WIN32)
    add_custom_command(
            TARGET graphing POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy "${PROJECT_SOURCE_DIR}/extern/wgpu-windows-x86_64-msvc-release/lib/wgpu_native.dll" $<TARGET_FILE_DIR:graphing>
            VERBATIM
    )
endif()
