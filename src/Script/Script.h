//
// Created by willb on 1/28/2026.
//

#ifndef GRAPHING_SCRIPT_H
#define GRAPHING_SCRIPT_H
#include <string>
#include <vector>

#include "lua.hpp"

class Script {
    Script();
    ~Script();

    std::string file;
    lua_State* state = nullptr;

    void load(const char *file);
};


#endif //GRAPHING_SCRIPT_H