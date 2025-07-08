#include <ctime>
#include <format>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

// 前置声明
class 角色存档;

// 原发器：游戏角色
class 游戏角色 {
public:
  游戏角色(std::string 名称, int 生命值, std::string 位置)
      : 角色名称(std::move(名称)), 生命值(生命值), 位置(std::move(位置)) {}

  // 创建存档
  std::unique_ptr<角色存档> 创建存档() const;

  // 加载存档
  void 加载存档(const 角色存档 &存档);

  // 角色行为
  void 战斗() {
    std::cout << 角色名称 << " 参与战斗...\n";
    生命值 -= 20;
    装备耐久度衰减();
  }

  void 探索新区域(std::string 新位置) {
    位置 = std::move(新位置);
    std::cout << "移动到新区域: " << 位置 << "\n";
  }

  void 获得装备(std::string 装备) {
    装备列表.push_back(std::move(装备));
    std::cout << "获得新装备: " << 装备列表.back() << "\n";
  }

  void 显示状态() const {
    std::cout << std::format("=== {} 状态 ===\n", 角色名称);
    std::cout << "生命值: " << 生命值 << "\n";
    std::cout << "位置: " << 位置 << "\n";
    std::cout << "装备: ";
    for (const auto &装备 : 装备列表) {
      std::cout << 装备 << " ";
    }
    std::cout << "\n\n";
  }

  // 提供公共访问器解决访问权限问题
  int 获取生命值() const { return 生命值; }
  const std::string &获取位置() const { return 位置; }
  const std::vector<std::string> &获取装备列表() const { return 装备列表; }

private:
  void 装备耐久度衰减() {
    if (!装备列表.empty()) {
      auto &装备 = 装备列表.back();
      装备 += "(磨损)";
    }
  }

  std::string 角色名称;
  int 生命值;
  std::string 位置;
  std::vector<std::string> 装备列表;
};

// 备忘录：角色存档
class 角色存档 {
public:
  // 使用游戏角色的公共接口访问状态
  角色存档(const 游戏角色 &角色)
      : 生命值(角色.获取生命值()), 位置(角色.获取位置()),
        装备列表(角色.获取装备列表()) {}

  // 访问接口
  int 获取生命值() const { return 生命值; }
  const std::string &获取位置() const { return 位置; }
  const std::vector<std::string> &获取装备列表() const { return 装备列表; }

private:
  const int 生命值;
  const std::string 位置;
  const std::vector<std::string> 装备列表;
};

// 实现原发器的方法
std::unique_ptr<角色存档> 游戏角色::创建存档() const {
  return std::make_unique<角色存档>(*this);
}

void 游戏角色::加载存档(const 角色存档 &存档) {
  生命值 = 存档.获取生命值();
  位置 = 存档.获取位置();
  装备列表 = 存档.获取装备列表();
  std::cout << "【系统】" << 角色名称 << " 加载存档成功\n";
}

// 管理者：存档管理器
class 存档管理器 {
public:
  void 保存存档(int 槽位, const 游戏角色 &角色) {
    存档映射表[槽位] = 角色.创建存档();
    std::cout << "【系统】存档已保存至槽位 " << 槽位 << "\n";
  }

  void 加载存档(int 槽位, 游戏角色 &角色) const {
    if (auto 搜索 = 存档映射表.find(槽位); 搜索 != 存档映射表.end()) {
      角色.加载存档(*搜索->second);
    } else {
      std::cout << "【错误】槽位 " << 槽位 << " 无存档\n";
    }
  }

  void 显示存档列表() const {
    if (存档映射表.empty()) {
      std::cout << "【系统】无存档记录\n";
      return;
    }

    std::cout << "=== 存档列表 ===\n";
    for (const auto &[槽位, 存档] : 存档映射表) {
      std::cout << std::format("槽位 {}: 生命值={}, 位置={}, 装备数={}\n", 槽位,
                               存档->获取生命值(), 存档->获取位置(),
                               存档->获取装备列表().size());
    }
  }

private:
  std::map<int, std::unique_ptr<角色存档>> 存档映射表;
};

int main() {
  // 创建游戏角色
  游戏角色 角色("冒险者", 100, "起始村庄");
  角色.获得装备("木剑");
  角色.显示状态();

  // 创建存档管理器
  存档管理器 存档管理;

  // 保存初始状态
  存档管理.保存存档(1, 角色);

  // 游戏进程
  角色.战斗();
  角色.探索新区域("黑暗森林");
  角色.获得装备("皮甲");
  角色.显示状态();

  // 保存进度
  存档管理.保存存档(2, 角色);

  // 继续游戏
  角色.战斗();
  角色.战斗();
  角色.探索新区域("恶魔城堡");
  角色.显示状态();

  // 加载存档1
  std::cout << "\n=== 加载存档1 ===\n";
  存档管理.加载存档(1, 角色);
  角色.显示状态();

  // 加载存档2
  std::cout << "\n=== 加载存档2 ===\n";
  存档管理.加载存档(2, 角色);
  角色.显示状态();

  // 查看存档列表
  std::cout << "\n=== 存档列表 ===\n";
  存档管理.显示存档列表();

  return 0;
}
