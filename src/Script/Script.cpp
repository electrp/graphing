//
// Created by willb on 1/28/2026.
//

#include "Script.h"

#include "Lua.h"

Script::Script()
{
}

Script::~Script()
{
    // if (state)
        // lua_close(state);
}

void Script::load(const char* file)
{
    // if (state)
    //     lua_close(state);
    // state = lua_open();
    // luaL_dofile(state, file);
}
