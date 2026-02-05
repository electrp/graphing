//
// Created by willb on 9/6/2025.
//

/*
 *  Expands out macros in a wgsl shader, used to make multi-file shader modules easier
 *  Notes:
 *      - # character must come at the beginning of a line
 *  Commands:
 *      - #include shaders/file.wgsl
 *      - #define X Y
 *      - #ifdef X, #endif
 */

#ifndef ENGINE_PROJ_SHADERPARSE_H
#define ENGINE_PROJ_SHADERPARSE_H

#include <set>
#include <string>
#include <map>

std::string shaderPreprocess(
    std::string filename,
    std::set<std::string>* read_files = nullptr,
    std::map<std::string, std::string>* defines = nullptr);

struct ShaderDescription {
    // std::vector<RenderBuffer> vertex_attributes;
};

#endif //ENGINE_PROJ_SHADERPARSE_H

