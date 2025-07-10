
-- Lua 5.4.7 xmake 构建配置

-- 定义 "lua" 目标，用于构建 Lua 解释器
target("lua")
    -- 设置目标类型为二进制文件
    set_kind("binary")
    -- 添加 "src" 目录下的所有 C 源文件
    add_files("src/*.c")
    -- 移除 "src/luac.c" 文件，因为它是单独构建的
    remove_files("src/luac.c")
    -- 添加 "src" 目录到包含目录列表
    add_includedirs("src")
    -- 添加 "LUA_UCID" 定义,开启Unicode标识符支持
    add_defines("LUA_UCID")


-- 定义 "luac" 目标，用于构建 Lua 编译器
target("luac")
    -- 设置目标类型为二进制文件
    set_kind("binary")
    -- 添加 "src/luac.c" 文件
    add_files("src/luac.c")
    -- 添加 "src" 目录下的所有 C 源文件
    add_files("src/*.c")
    -- 移除 "src/lua.c" 文件，因为它不应该是 Lua 编译器的一部分
    remove_files("src/lua.c")
    -- 添加 "src" 目录到包含目录列表
    add_includedirs("src")
    -- 添加 "LUA_UCID" 定义,开启Unicode标识符支持
    add_defines("LUA_UCID")
