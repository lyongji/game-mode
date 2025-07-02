

#include <memory>
#include <print>
#include <typeindex>
#include <unordered_map>
#include <vector>

// 前向声明游戏对象类
class 游戏对象;

// 组件基类
class 组件 {
public:
  virtual ~组件() = default; // 虚析构函数确保正确释放派生类资源

  /**
   * 每帧更新组件的逻辑
   * @param 所属对象 指向拥有该组件的游戏对象
   */
  virtual void 更新(游戏对象 *所属对象) = 0;

  /**
   * 处理游戏事件
   * @param 事件类型 事件标识符
   * @param 事件数据 事件相关数据
   */
  virtual void 处理事件(const std::string &事件类型, void *事件数据) {}
};

// 游戏对象类
class 游戏对象 {
private:
  std::string 对象名称; // 游戏对象名称
  bool 是否激活 = true; // 对象激活状态

  // 组件存储容器：使用unique_ptr管理组件生命周期
  std::vector<std::unique_ptr<组件>> 组件库;

  // 组件类型索引：加速组件查找
  std::unordered_map<std::type_index, 组件 *> 组件索引;

public:
  explicit 游戏对象(std::string 名称 = "") : 对象名称(std::move(名称)) {}

  /**
   * 添加组件到游戏对象
   * @tparam T 组件类型
   * @tparam Args 组件构造参数类型
   * @param 构造参数 组件构造参数
   * @return 指向新组件的指针
   */
  template <typename T, typename... Args> T *添加组件(Args &&...构造参数) {
    // 创建组件实例
    auto 新组件 = std::make_unique<T>(std::forward<Args>(构造参数)...);
    auto 组件指针 = 新组件.get();

    // 存储组件并更新索引
    组件库.push_back(std::move(新组件));
    组件索引[typeid(T)] = 组件指针;

    return 组件指针;
  }

  /**
   * 获取指定类型的组件
   * @tparam T 组件类型
   * @return 指向组件的指针（未找到返回nullptr）
   */
  template <typename T> T *获取组件() {
    auto 查找结果 = 组件索引.find(typeid(T));
    if (查找结果 != 组件索引.end()) {
      return dynamic_cast<T *>(查找结果->second);
    }

    // 遍历查找（确保类型安全）
    for (auto &组件实例 : 组件库) {
      if (auto 结果 = dynamic_cast<T *>(组件实例.get())) {
        // 更新索引以便下次快速查找
        组件索引[typeid(T)] = 结果;
        return 结果;
      }
    }
    return nullptr;
  }

  /**
   * 每帧更新所有组件
   */
  void 更新() {
    if (!是否激活)
      return;

    for (auto &组件实例 : 组件库) {
      组件实例->更新(this);
    }
  }

  /**
   * 向所有组件广播事件
   * @param 事件类型 事件标识符
   * @param 事件数据 事件相关数据
   */
  void 广播事件(const std::string &事件类型, void *事件数据 = nullptr) {
    for (auto &组件实例 : 组件库) {
      组件实例->处理事件(事件类型, 事件数据);
    }
  }

  // 设置激活状态
  void 设置激活(bool 激活) { 是否激活 = 激活; }

  // 获取对象名称
  const std::string &获取名称() const { return 对象名称; }
};

// ====================== 具体组件实现 ======================

// 可移动组件：处理对象位置和移动逻辑
class 可移动组件 : public 组件 {
public:
  struct 位置 {
    float x = 0, y = 0, z = 0;
  };
  struct 速度 {
    float x = 0, y = 0, z = 0;
  };

  位置 当前位置;
  速度 当前速度;

  void 更新(游戏对象 *所属对象) override {
    // 更新位置
    当前位置.x += 当前速度.x;
    当前位置.y += 当前速度.y;
    当前位置.z += 当前速度.z;

    std::print("{} 移动到 ({}, {}, {})\n", 所属对象->获取名称(), 当前位置.x,
               当前位置.y, 当前位置.z);
  }
};

// 玩家控制组件：处理玩家输入
class 玩家控制组件 : public 组件 {
public:
  void 更新(游戏对象 *所属对象) override {
    // 获取同对象的移动组件
    if (auto 移动组件 = 所属对象->获取组件<可移动组件>()) {
      // 模拟按键检测
      if (模拟按键按下(87)) { // W键
        移动组件->当前速度.y += 0.1f;
        std::print("↑ 加速\n");
      }
      if (模拟按键按下(83)) { // S键
        移动组件->当前速度.y -= 0.1f;
        std::print("↓ 减速\n");
      }
    }
  }

private:
  // 模拟按键检测（实际项目中替换为真实输入系统）
  bool 模拟按键按下(int 键码) {
    static int 计数器 = 0;
    return (++计数器 % 30) < 3; // 每30帧触发3帧
  }
};

// 生命值组件：管理对象生命状态
class 生命值组件 : public 组件 {
  int 当前生命值;
  int 最大生命值;

public:
  生命值组件(int 最大生命) : 当前生命值(最大生命), 最大生命值(最大生命) {}

  void 更新(游戏对象 *所属对象) override {
    // 模拟每帧生命值减少
    if (当前生命值 > 0) {
      当前生命值 -= 1;
      std::print("{} 生命值: {}/{}\n", 所属对象->获取名称(), 当前生命值,
                 最大生命值);

      if (当前生命值 <= 0) {
        std::print("💀 {} 被销毁!\n", 所属对象->获取名称());
        所属对象->设置激活(false);
      }
    }
  }

  void 处理事件(const std::string &事件类型, void *事件数据) override {
    if (事件类型 == "受到伤害") {
      int 伤害值 = *static_cast<int *>(事件数据);
      当前生命值 -= 伤害值;
      std::print("⚡ {} 受到 {} 点伤害!\n", "生命值组件", 伤害值);
    }
  }

  const std::string 获取名称() const { return "生命值组件"; }
};

// ====================== 游戏场景示例 ======================

int main() {
  // 创建玩家对象
  auto 玩家 = std::make_unique<游戏对象>("玩家角色");
  玩家->添加组件<可移动组件>();
  玩家->添加组件<玩家控制组件>();
  玩家->添加组件<生命值组件>(100);

  // 创建敌人对象
  auto 敌人 = std::make_unique<游戏对象>("敌人");
  敌人->添加组件<可移动组件>()->当前速度 = {0.2f, 0.0f, 0.0f};
  敌人->添加组件<生命值组件>(50);

  // 游戏主循环
  for (int 帧数 = 0; 帧数 < 40; ++帧数) {
    std::print("\n===== 帧 {} =====\n", 帧数);
    // 更新游戏对象
    玩家->更新();
    敌人->更新();

    // 每30帧敌人受到伤害
    if (帧数 % 30 == 0 && 帧数 > 0) {
      int 伤害值 = 15;
      敌人->广播事件("受到伤害", &伤害值);
    }
  }

  return 0;
}
