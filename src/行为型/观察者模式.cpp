#include <format>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

// 前置声明
class 观察者;

// 游戏事件类型
enum class 事件类型 {
  玩家移动,
  玩家升级,
  获得物品,
  击败敌人,
  生命值变化,
  游戏保存
};

// 游戏事件数据结构
struct 游戏事件 {
  事件类型 类型;
  std::string 来源; // 事件来源（如玩家名称）
  std::string 目标; // 事件目标（如敌人名称）
  int 数值;         // 相关数值（如伤害值、等级）
  std::string 位置; // 发生位置
};

// 主题接口
class 主题 {
public:
  virtual ~主题() = default;
  virtual void 注册观察者(观察者 *观察者) = 0;
  virtual void 注销观察者(观察者 *观察者) = 0;
  virtual void 通知观察者(const 游戏事件 &事件) = 0;
};
// 观察者接口
class 观察者 {
public:
  virtual ~观察者() = default;
  virtual void 更新(const 游戏事件 &事件) = 0;
};
// 具体主题：游戏事件系统
class 游戏事件系统 : public 主题 {
public:
  void 注册观察者(观察者 *观察者) override { 观察者列表.push_back(观察者); }

  void 注销观察者(观察者 *观察者) override {
    auto 位置 = std::find(观察者列表.begin(), 观察者列表.end(), 观察者);
    if (位置 != 观察者列表.end()) {
      观察者列表.erase(位置);
    }
  }

  void 通知观察者(const 游戏事件 &事件) override {
    for (auto *观察者 : 观察者列表) {
      观察者->更新(事件);
    }
  }

  void 发布事件(事件类型 类型, const std::string &来源,
                const std::string &目标 = "", int 数值 = 0,
                const std::string &位置 = "") {
    游戏事件 事件{类型, 来源, 目标, 数值, 位置};
    std::cout << std::format("【事件发布】{}: {} => {}\n",
                             获取事件类型名称(类型), 来源, 目标);
    通知观察者(事件);
  }

private:
  std::string 获取事件类型名称(事件类型 类型) const {
    static const std::unordered_map<事件类型, std::string> 类型名称 = {
        {事件类型::玩家移动, "玩家移动"},     {事件类型::玩家升级, "玩家升级"},
        {事件类型::获得物品, "获得物品"},     {事件类型::击败敌人, "击败敌人"},
        {事件类型::生命值变化, "生命值变化"}, {事件类型::游戏保存, "游戏保存"}};
    return 类型名称.at(类型);
  }

  std::vector<观察者 *> 观察者列表;
};

// 具体观察者：成就系统
class 成就系统 : public 观察者 {
public:
  void 更新(const 游戏事件 &事件) override {
    switch (事件.类型) {
    case 事件类型::玩家升级:
      if (事件.数值 >= 10 && !解锁的成就.count("新手毕业")) {
        解锁成就("新手毕业", "达到10级");
      }
      if (事件.数值 >= 50 && !解锁的成就.count("传奇英雄")) {
        解锁成就("传奇英雄", "达到50级");
      }
      break;

    case 事件类型::击败敌人:
      if (事件.目标 == "巨龙" && !解锁的成就.count("屠龙者")) {
        解锁成就("屠龙者", "击败巨龙");
      }
      击败敌人计数[事件.目标]++;
      if (击败敌人计数[事件.目标] >= 100 && !解锁的成就.count("百人斩")) {
        解锁成就("百人斩", std::format("击败100个{}", 事件.目标));
      }
      break;

    case 事件类型::获得物品:
      if (事件.目标 == "王者之剑" && !解锁的成就.count("传说武器")) {
        解锁成就("传说武器", "获得王者之剑");
      }
      break;

    default:
      // 忽略其他事件
      break;
    }
  }

private:
  void 解锁成就(const std::string &成就名称, const std::string &描述) {
    解锁的成就.insert(成就名称);
    std::cout << std::format("【成就解锁】{} - {}\n", 成就名称, 描述);
  }

  std::set<std::string> 解锁的成就;
  std::unordered_map<std::string, int> 击败敌人计数;
};

// 具体观察者：用户界面系统
class 用户界面 : public 观察者 {
public:
  void 更新(const 游戏事件 &事件) override {
    switch (事件.类型) {
    case 事件类型::玩家移动:
      更新小地图(事件.位置);
      break;

    case 事件类型::生命值变化:
      更新血条(事件.来源, 事件.数值);
      break;

    case 事件类型::玩家升级:
      显示升级特效(事件.来源, 事件.数值);
      break;

    case 事件类型::击败敌人:
      显示击败特效(事件.目标);
      break;

    default:
      // 忽略其他事件
      break;
    }
  }

private:
  void 更新小地图(const std::string &位置) {
    std::cout << std::format("【UI】小地图更新: 当前位置 - {}\n", 位置);
  }

  void 更新血条(const std::string &玩家, int 生命值) {
    std::cout << std::format("【UI】{} 血条更新: {}/100\n", 玩家, 生命值);
  }

  void 显示升级特效(const std::string &玩家, int 等级) {
    std::cout << std::format("【UI】{} 升级特效: 达到 {} 级!\n", 玩家, 等级);
  }

  void 显示击败特效(const std::string &敌人) {
    std::cout << std::format("【UI】击败特效: {} 被消灭!\n", 敌人);
  }
};

// 具体观察者：自动存档系统
class 自动存档系统 : public 观察者 {
public:
  void 更新(const 游戏事件 &事件) override {
    switch (事件.类型) {
    case 事件类型::玩家升级:
    case 事件类型::获得物品:
    case 事件类型::击败敌人:
    case 事件类型::游戏保存:
      执行存档(事件.来源);
      break;

    default:
      // 忽略其他事件
      break;
    }
  }

private:
  void 执行存档(const std::string &玩家) {
    std::cout << std::format("【存档】{} 的游戏进度已自动保存\n", 玩家);
  }
};

// 具体观察者：游戏日志系统
class 游戏日志系统 : public 观察者 {
public:
  void 更新(const 游戏事件 &事件) override {
    std::string 日志条目 = 创建日志条目(事件);
    日志记录.push_back(日志条目);
    std::cout << std::format("【日志】{}\n", 日志条目);
  }

  void 显示日志() const {
    std::cout << "\n=== 游戏日志 ===\n";
    for (const auto &日志 : 日志记录) {
      std::cout << 日志 << "\n";
    }
  }

private:
  std::string 创建日志条目(const 游戏事件 &事件) const {
    static const std::unordered_map<事件类型, std::string> 事件描述 = {
        {事件类型::玩家移动, "移动到"},
        {事件类型::玩家升级, "升级到"},
        {事件类型::获得物品, "获得了"},
        {事件类型::击败敌人, "击败了"},
        {事件类型::生命值变化, "生命值变为"},
        {事件类型::游戏保存, "保存了游戏"}};

    std::string 描述 = 事件描述.at(事件.类型);
    std::string 条目 = std::format("[{}] {} {}", 事件.来源, 描述, 事件.目标);

    if (事件.数值 > 0) {
      条目 += std::format(" ({})", 事件.数值);
    }

    if (!事件.位置.empty()) {
      条目 += std::format(" @ {}", 事件.位置);
    }

    return 条目;
  }

  std::vector<std::string> 日志记录;
};

int main() {
  // 创建事件系统（主题）
  游戏事件系统 事件系统;

  // 创建观察者系统
  成就系统 成就追踪;
  用户界面 UI系统;
  自动存档系统 存档系统;
  游戏日志系统 日志系统;

  // 注册观察者
  事件系统.注册观察者(&成就追踪);
  事件系统.注册观察者(&UI系统);
  事件系统.注册观察者(&存档系统);
  事件系统.注册观察者(&日志系统);

  // 模拟游戏事件
  事件系统.发布事件(事件类型::玩家移动, "玩家1", "", 0, "起始村庄");
  事件系统.发布事件(事件类型::击败敌人, "玩家1", "哥布林", 1, "黑暗森林");
  事件系统.发布事件(事件类型::获得物品, "玩家1", "治疗药水", 3, "黑暗森林");
  事件系统.发布事件(事件类型::生命值变化, "玩家1", "", 85, "黑暗森林");

  for (int i = 0; i < 10; i++) {
    事件系统.发布事件(事件类型::击败敌人, "玩家1", "骷髅兵", i + 1, "亡灵墓地");
  }

  事件系统.发布事件(事件类型::玩家升级, "玩家1", "", 10, "亡灵墓地");
  事件系统.发布事件(事件类型::获得物品, "玩家1", "王者之剑", 1, "龙之巢穴");
  事件系统.发布事件(事件类型::击败敌人, "玩家1", "巨龙", 1, "龙之巢穴");
  事件系统.发布事件(事件类型::玩家升级, "玩家1", "", 50, "龙之巢穴");
  事件系统.发布事件(事件类型::游戏保存, "系统", "自动保存");

  // 显示日志
  日志系统.显示日志();

  // 注销观察者（游戏结束时）
  事件系统.注销观察者(&成就追踪);
  事件系统.注销观察者(&UI系统);
  事件系统.注销观察者(&存档系统);
  事件系统.注销观察者(&日志系统);

  return 0;
}
