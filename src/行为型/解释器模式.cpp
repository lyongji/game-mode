#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <lua.hpp>
#include <string>

// 游戏脚本解释器类
class 游戏脚本解释器 {
public:
  游戏脚本解释器() {
    lua状态 = luaL_newstate();
    luaL_openlibs(lua状态);
  }

  ~游戏脚本解释器() { 清理(); }

  // 执行Lua脚本
  bool 执行脚本(const std::string &脚本内容) {
    if (luaL_dostring(lua状态, 脚本内容.c_str())) {
      std::cerr << "Lua错误: " << lua_tostring(lua状态, -1) << std::endl;
      lua_pop(lua状态, 1);
      return false;
    }
    return true;
  }

  // 从文件执行Lua脚本
  bool 执行脚本文件(const std::string &文件路径) {
    namespace fs = std::filesystem;
    if (!fs::exists(文件路径)) {
      std::cerr << "Lua脚本文件不存在: " << 文件路径 << std::endl;
      return false;
    }

    if (luaL_dofile(lua状态, 文件路径.c_str())) {
      std::cerr << "Lua错误: " << lua_tostring(lua状态, -1) << std::endl;
      lua_pop(lua状态, 1);
      return false;
    }
    return true;
  }

  // 注册C++函数到Lua
  void 注册函数(const std::string &函数名, lua_CFunction 函数指针) {
    lua_register(lua状态, 函数名.c_str(), 函数指针);
  }

  // 清理Lua状态
  void 清理() {
    if (lua状态) {
      lua_close(lua状态);
      lua状态 = nullptr;
    }
  }

private:
  lua_State *lua状态;
};

// 计算玩家伤害的C++函数，将被绑定到Lua
static int 计算伤害(lua_State *L) {
  int 攻击力 = lua_tointeger(L, 1);
  int 暴击率 = lua_tointeger(L, 2);
  int 暴击伤害 = lua_tointeger(L, 3);

  // 简单伤害计算逻辑：基础攻击力 + 暴击判断
  int 伤害 = 攻击力;
  if (rand() % 100 < 暴击率) {
    伤害 = 攻击力 * (100 + 暴击伤害) / 100;
    lua_pushstring(L, "暴击! 造成");
  } else {
    lua_pushstring(L, "造成");
  }

  lua_pushinteger(L, 伤害);
  return 2; // 返回两个值：伤害描述和伤害值
}

int main() {
  // 基本控制台设置
  srand(time(nullptr)); // 初始化随机数种子

  游戏脚本解释器 解释器;

  // 注册C++函数到Lua
  解释器.注册函数("计算伤害", 计算伤害);

  // 从文件执行游戏相关的Lua代码
  std::string 脚本路径 = "game_script.lua"; // 使用相对路径
  if (!解释器.执行脚本文件(脚本路径)) {
    std::cerr << "执行Lua脚本失败" << std::endl;
    return 1;
  }

  // 显式清理Lua状态
  解释器.清理();

  // 使用printf确保基本输出
  printf("程序正常结束\n");
  return 0;
}
