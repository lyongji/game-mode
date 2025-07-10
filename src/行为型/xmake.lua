
target("责任链模式")
  set_kind("binary")
  add_files("./责任链模式.cpp")

target("命令模式")
  set_kind("binary")
  add_files("./命令模式.cpp")

target("迭代器模式")
  set_kind("binary")
  add_files("./迭代器模式.cpp")

target("中介者模式")
  set_kind("binary")
  add_files("./中介者模式.cpp")

target("备忘录模式")
  set_kind("binary")
  add_files("./备忘录模式.cpp")

target("状态模式")
  set_kind("binary")
  add_files("./状态模式.cpp")

target("观察者模式")
  set_kind("binary")
  add_files("./观察者模式.cpp")

target("策略模式")
  set_kind("binary")
  add_files("./策略模式.cpp")

target("模板方法模式")
  set_kind("binary")
  add_files("./模板方法模式.cpp")

-- Lua库目标
target("lua库")
    set_kind("static")
    add_files("../../lib/lua-5.4.7/src/*.c|luac.c|lua.c")
    add_includedirs("../../lib/lua-5.4.7/src")
    add_defines("LUA_UCID")

-- 解释器程序目标
target("解释器模式")
    set_kind("binary")
    add_files("解释器模式.cpp")
    add_deps("lua库")
    add_includedirs("../../lib/lua-5.4.7/src")
    add_defines("LUA_UCID") -- 定义宏,支持Unicode标识符
    -- 将脚本文件复制到输出目录
    after_build(function (target)
        os.cp(path.join(os.scriptdir(), "game_script.lua"), target:targetdir())
    end)

