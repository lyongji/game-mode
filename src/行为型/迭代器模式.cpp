#include <algorithm>
#include <functional>
#include <memory>
#include <print>
#include <vector>

// 游戏实体基类
class 游戏实体 {
public:
  virtual ~游戏实体() = default;
  virtual void 更新() = 0;
  virtual void 渲染() = 0;
  virtual std::string 获取类型() const = 0;
  virtual int 获取ID() const = 0;
  virtual float 获取X() const = 0;
  virtual float 获取Y() const = 0;
};

// 具体实体：玩家
class 玩家 : public 游戏实体 {
public:
  玩家(int id, float x, float y) : id(id), x(x), y(y) {}

  void 更新() override {
    // 玩家更新逻辑
  }

  void 渲染() override {
    // 玩家渲染逻辑
  }

  std::string 获取类型() const override { return "玩家"; }
  int 获取ID() const override { return id; }
  float 获取X() const override { return x; }
  float 获取Y() const override { return y; }

private:
  int id;
  float x, y;
};

// 具体实体：敌人
class 敌人 : public 游戏实体 {
public:
  敌人(int id, float x, float y, std::string 类型)
      : id(id), x(x), y(y), 类型(类型) {}

  void 更新() override {
    // 敌人AI逻辑
  }

  void 渲染() override {
    // 敌人渲染逻辑
  }

  std::string 获取类型() const override { return "敌人(" + 类型 + ")"; }
  int 获取ID() const override { return id; }
  float 获取X() const override { return x; }
  float 获取Y() const override { return y; }

private:
  int id;
  float x, y;
  std::string 类型;
};

// 具体实体：道具
class 道具 : public 游戏实体 {
public:
  道具(int id, float x, float y, std::string 名称)
      : id(id), x(x), y(y), 名称(名称) {}

  void 更新() override {
    // 道具逻辑（如旋转）
  }

  void 渲染() override {
    // 道具渲染
  }

  std::string 获取类型() const override { return "道具(" + 名称 + ")"; }
  int 获取ID() const override { return id; }
  float 获取X() const override { return x; }
  float 获取Y() const override { return y; }

private:
  int id;
  float x, y;
  std::string 名称;
};

// 迭代器接口
class 实体迭代器 {
public:
  virtual ~实体迭代器() = default;
  virtual void 开始() = 0;
  virtual void 下一个() = 0;
  virtual bool 是否结束() const = 0;
  virtual 游戏实体 *当前() = 0;
};

// 具体迭代器：所有实体迭代器
class 所有实体迭代器 : public 实体迭代器 {
public:
  所有实体迭代器(std::vector<std::unique_ptr<游戏实体>> &实体列表)
      : 实体列表(实体列表), 当前位置(0) {}

  void 开始() override { 当前位置 = 0; }

  void 下一个() override {
    if (!是否结束())
      ++当前位置;
  }

  bool 是否结束() const override { return 当前位置 >= 实体列表.size(); }

  游戏实体 *当前() override {
    return 是否结束() ? nullptr : 实体列表[当前位置].get();
  }

private:
  std::vector<std::unique_ptr<游戏实体>> &实体列表;
  size_t 当前位置;
};

// 具体迭代器：类型过滤迭代器
class 类型过滤迭代器 : public 实体迭代器 {
public:
  类型过滤迭代器(std::vector<std::unique_ptr<游戏实体>> &实体列表,
                 std::function<bool(游戏实体 *)> 过滤条件)
      : 实体列表(实体列表), 过滤条件(过滤条件), 当前位置(0) {
    前进到下一个有效项();
  }

  void 开始() override {
    当前位置 = 0;
    前进到下一个有效项();
  }

  void 下一个() override {
    if (!是否结束()) {
      ++当前位置;
      前进到下一个有效项();
    }
  }

  bool 是否结束() const override { return 当前位置 >= 实体列表.size(); }

  游戏实体 *当前() override {
    return 是否结束() ? nullptr : 实体列表[当前位置].get();
  }

private:
  void 前进到下一个有效项() {
    while (!是否结束() && !过滤条件(实体列表[当前位置].get())) {
      ++当前位置;
    }
  }

  std::vector<std::unique_ptr<游戏实体>> &实体列表;
  std::function<bool(游戏实体 *)> 过滤条件;
  size_t 当前位置;
};

// 具体迭代器：空间分区迭代器
class 空间分区迭代器 : public 实体迭代器 {
public:
  空间分区迭代器(std::vector<std::unique_ptr<游戏实体>> &实体列表, float 最小X,
                 float 最小Y, float 最大X, float 最大Y)
      : 实体列表(实体列表), 最小X(最小X), 最小Y(最小Y), 最大X(最大X),
        最大Y(最大Y), 当前位置(0) {
    前进到下一个有效项();
  }

  void 开始() override {
    当前位置 = 0;
    前进到下一个有效项();
  }

  void 下一个() override {
    if (!是否结束()) {
      ++当前位置;
      前进到下一个有效项();
    }
  }

  bool 是否结束() const override { return 当前位置 >= 实体列表.size(); }

  游戏实体 *当前() override {
    return 是否结束() ? nullptr : 实体列表[当前位置].get();
  }

private:
  bool 在区域内(游戏实体 *实体) {
    return 实体->获取X() >= 最小X && 实体->获取X() <= 最大X &&
           实体->获取Y() >= 最小Y && 实体->获取Y() <= 最大Y;
  }

  void 前进到下一个有效项() {
    while (!是否结束() && !在区域内(实体列表[当前位置].get())) {
      ++当前位置;
    }
  }

  std::vector<std::unique_ptr<游戏实体>> &实体列表;
  float 最小X, 最小Y, 最大X, 最大Y;
  size_t 当前位置;
};

// 实体管理器（聚合）
class 实体管理器 {
public:
  // 添加实体
  void 添加实体(std::unique_ptr<游戏实体> 实体) {
    实体列表.push_back(std::move(实体));
  }

  // 移除实体（安全删除）
  void 移除实体(int id) {
    auto 位置 = std::ranges::find_if(
        实体列表, [id](auto &实体) { return 实体->获取ID() == id; });

    if (位置 != 实体列表.end()) {
      待删除列表.push_back(std::move(*位置));
      实体列表.erase(位置);
    }
  }

  // 创建迭代器
  std::unique_ptr<实体迭代器> 创建迭代器() {
    return std::make_unique<所有实体迭代器>(实体列表);
  }

  std::unique_ptr<实体迭代器>
  创建类型过滤迭代器(std::function<bool(游戏实体 *)> 过滤条件) {
    return std::make_unique<类型过滤迭代器>(实体列表, 过滤条件);
  }

  std::unique_ptr<实体迭代器> 创建空间分区迭代器(float 最小X, float 最小Y,
                                                 float 最大X, float 最大Y) {
    return std::make_unique<空间分区迭代器>(实体列表, 最小X, 最小Y, 最大X,
                                            最大Y);
  }

  // 更新所有实体
  void 更新() {
    for (auto &实体 : 实体列表) {
      实体->更新();
    }
  }

  // 处理待删除实体
  void 清理待删除实体() { 待删除列表.clear(); }

  // 获取实体列表引用（用于范围视图）
  auto &获取实体列表() { return 实体列表; }

private:
  std::vector<std::unique_ptr<游戏实体>> 实体列表;
  std::vector<std::unique_ptr<游戏实体>> 待删除列表;
};

// 范围库适配器 - 修复类型兼容性问题
class 实体迭代器范围 {
public:
  // 迭代器适配器类型
  struct 迭代器适配器 {
    using iterator_category = std::input_iterator_tag;
    using value_type = 游戏实体 *;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;

    实体迭代器 *原始迭代器;

    迭代器适配器() : 原始迭代器(nullptr) {}
    迭代器适配器(实体迭代器 *iter) : 原始迭代器(iter) {
      if (原始迭代器 && !原始迭代器->是否结束()) {
        原始迭代器->开始();
      }
    }

    value_type operator*() const {
      return 原始迭代器 ? 原始迭代器->当前() : nullptr;
    }

    迭代器适配器 &operator++() {
      if (原始迭代器 && !原始迭代器->是否结束()) {
        原始迭代器->下一个();
      }
      return *this;
    }

    bool operator==(const 迭代器适配器 &其他) const {
      // 两者都指向空或同一个迭代器
      if (原始迭代器 == 其他.原始迭代器)
        return true;

      // 任一为空的情况
      if (!原始迭代器 || !其他.原始迭代器) {
        return (!原始迭代器 || 原始迭代器->是否结束()) &&
               (!其他.原始迭代器 || 其他.原始迭代器->是否结束());
      }

      // 比较迭代器状态
      return 原始迭代器->是否结束() && 其他.原始迭代器->是否结束();
    }

    bool operator!=(const 迭代器适配器 &其他) const { return !(*this == 其他); }
  };

  explicit 实体迭代器范围(std::unique_ptr<实体迭代器> 迭代器)
      : 迭代器(std::move(迭代器)) {}

  // 开始迭代器
  迭代器适配器 begin() { return 迭代器适配器(迭代器.get()); }

  // 结束迭代器 - 返回同类型的默认构造对象
  迭代器适配器 end() {
    return 迭代器适配器(); // 默认构造表示结束
  }

private:
  std::unique_ptr<实体迭代器> 迭代器;
};

// 创建范围视图的辅助函数
auto 创建实体范围视图(std::unique_ptr<实体迭代器> 迭代器) {
  return 实体迭代器范围(std::move(迭代器));
}

// 客户端代码
int main() {
  实体管理器 管理器;

  // 添加游戏实体
  管理器.添加实体(std::make_unique<玩家>(1, 10.0f, 20.0f));
  管理器.添加实体(std::make_unique<敌人>(2, 30.0f, 40.0f, "兽人"));
  管理器.添加实体(std::make_unique<敌人>(3, 50.0f, 60.0f, "哥布林"));
  管理器.添加实体(std::make_unique<道具>(4, 70.0f, 80.0f, "血瓶"));
  管理器.添加实体(std::make_unique<道具>(5, 90.0f, 100.0f, "魔法书"));
  管理器.添加实体(std::make_unique<敌人>(6, 15.0f, 25.0f, "骷髅"));

  // 1. 使用传统迭代器模式遍历
  std::println("==== 所有游戏实体 ====");
  auto 迭代器 = 管理器.创建迭代器();
  for (迭代器->开始(); !迭代器->是否结束(); 迭代器->下一个()) {
    auto 实体 = 迭代器->当前();
    std::println("实体 {}: {} - 位置({:.1f}, {:.1f})", 实体->获取ID(),
                 实体->获取类型(), 实体->获取X(), 实体->获取Y());
  }

  // 2. 使用范围库适配器
  std::println("\n==== 所有敌人（使用范围库） ====");
  auto 敌人范围 = 创建实体范围视图(管理器.创建类型过滤迭代器(
      [](游戏实体 *实体) { return 实体->获取类型().starts_with("敌人"); }));

  for (auto 实体 : 敌人范围) {
    if (实体) { // 确保实体有效
      std::println("敌人 {}: {} - 位置({:.1f}, {:.1f})", 实体->获取ID(),
                   实体->获取类型(), 实体->获取X(), 实体->获取Y());
    }
  }

  // 3. 使用范围库算法
  std::println("\n==== 使用范围库算法 ====");
  auto 所有实体范围 = 创建实体范围视图(管理器.创建迭代器());

  // 计算玩家数量
  int 玩家数量 = 0;
  for (auto 实体 : 所有实体范围) {
    if (实体 && 实体->获取类型() == "玩家") {
      玩家数量++;
    }
  }
  std::println("玩家数量: {}", 玩家数量);

  // 重新创建范围（迭代器已消耗）
  auto 所有实体范围2 = 创建实体范围视图(管理器.创建迭代器());

  // 查找所有道具
  std::println("\n所有道具:");
  for (auto 实体 : 所有实体范围2) {
    if (实体 && 实体->获取类型().starts_with("道具")) {
      std::println("道具 {}: {}", 实体->获取ID(), 实体->获取类型());
    }
  }

  // 4. 空间分区查询
  std::println("\n==== 区域(10-40, 20-50)内的实体 ====");
  auto 区域范围 = 创建实体范围视图(管理器.创建空间分区迭代器(10, 20, 40, 50));

  for (auto 实体 : 区域范围) {
    if (实体) {
      std::println("区域实体 {}: {} - 位置({:.1f}, {:.1f})", 实体->获取ID(),
                   实体->获取类型(), 实体->获取X(), 实体->获取Y());
    }
  }

  // 5. 组合范围库操作（手动过滤）
  std::println("\n==== 区域内的敌人 ====");
  auto 区域范围2 = 创建实体范围视图(管理器.创建空间分区迭代器(10, 20, 40, 50));

  for (auto 实体 : 区域范围2) {
    if (实体 && 实体->获取类型().starts_with("敌人")) {
      std::println("区域敌人 {}: {}", 实体->获取ID(), 实体->获取类型());
    }
  }

  // 6. 移除实体
  管理器.移除实体(3); // 移除哥布林
  std::println("\n==== 移除哥布林后的敌人 ====");

  auto 更新后敌人范围 =
      创建实体范围视图(管理器.创建类型过滤迭代器([](游戏实体 *实体) {
        return 实体 && 实体->获取类型().starts_with("敌人");
      }));

  for (auto 实体 : 更新后敌人范围) {
    if (实体) {
      std::println("敌人 {}: {}", 实体->获取ID(), 实体->获取类型());
    }
  }

  // 清理待删除实体
  管理器.清理待删除实体();
}
