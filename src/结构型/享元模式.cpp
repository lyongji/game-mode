#include <format> // 添加 format 头文件
#include <memory>
#include <print>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector> // 添加 vector 头文件

struct vec2 {
  int x, y;
};

class 贴图 {
public:
  virtual void 绘制(vec2 位置) = 0;
  virtual ~贴图() = default; // 添加虚析构函数确保正确释放资源
};

class 火焰贴图 : public 贴图 {
public:
  std::string 持有的贴图;
  火焰贴图() : 持有的贴图("🔥火焰贴图") {}

  void 绘制(vec2 位置) override {
    std::println("持有的贴图:{};位置:x {} y {}", 持有的贴图, 位置.x, 位置.y);
  }
};

class 寒冰贴图 : public 贴图 {
public:
  std::string 持有的贴图1;
  std::string 持有的贴图2;
  寒冰贴图() : 持有的贴图1("🧊寒冰贴图1"), 持有的贴图2("🧊寒冰贴图2") {}

  void 绘制(vec2 位置) override {
    std::println("持有的贴图:{};位置:x {} y {}", 持有的贴图1, 位置.x, 位置.y);
    std::println("持有的贴图:{};位置:x {} y {}", 持有的贴图2, 位置.x, 位置.y);
  }
};

class 贴图工厂 {
private:
  static std::unordered_map<std::string, std::shared_ptr<贴图>> 贴图映射;

public:
  static std::shared_ptr<贴图> 获取贴图(const std::string &类型) {
    auto it = 贴图映射.find(类型);
    if (it != 贴图映射.end()) {
      return it->second;
    }

    // 动态创建新贴图类型
    std::shared_ptr<贴图> 新贴图;
    if (类型 == "火焰") {
      新贴图 = std::make_shared<火焰贴图>();
    } else if (类型 == "寒冰") {
      新贴图 = std::make_shared<寒冰贴图>();
    } else {
      throw std::runtime_error("未知的贴图类型: " + 类型);
    }

    贴图映射[类型] = 新贴图;
    return 新贴图;
  }
};

// 静态成员初始化
std::unordered_map<std::string, std::shared_ptr<贴图>> 贴图工厂::贴图映射;

// 子弹类使用享元贴图
class 子弹 {
private:
  vec2 位置;
  vec2 速度;
  std::shared_ptr<贴图> 精灵;

public:
  子弹(vec2 初始位置, vec2 初始速度, const std::string &贴图类型)
      : 位置(初始位置), 速度(初始速度), 精灵(贴图工厂::获取贴图(贴图类型)) {}

  void 更新() {
    位置.x += 速度.x;
    位置.y += 速度.y;
  }

  void 绘制() {
    if (精灵) {
      精灵->绘制(位置);
    }
  }
};

int main() {
  try {
    // 创建共享贴图的子弹对象
    std::vector<子弹> 子弹列表;

    // 创建火焰子弹
    for (int i = 0; i < 3; i++) {
      子弹列表.emplace_back(vec2{i * 10, 0}, vec2{1, 1}, "火焰");
    }

    // 创建寒冰子弹
    for (int i = 0; i < 2; i++) {
      子弹列表.emplace_back(vec2{0, i * 20}, vec2{-1, 0}, "寒冰");
    }

    // 更新并绘制所有子弹
    std::println("===== 初始状态 =====");
    for (auto &子弹对象 : 子弹列表) {
      子弹对象.绘制();
    }

    // 更新位置后再次绘制
    std::println("\n===== 更新后状态 =====");
    for (auto &子弹对象 : 子弹列表) {
      子弹对象.更新();
      子弹对象.绘制();
    }

    // 测试享元效果 - 再次获取相同贴图
    std::println("\n===== 测试享元效果 =====");
    auto 共享火焰贴图 = 贴图工厂::获取贴图("火焰");
    auto 共享寒冰贴图 = 贴图工厂::获取贴图("寒冰");

    // 内存地址相同证明是同一个对象
    std::println("火焰贴图地址: {}", static_cast<void *>(共享火焰贴图.get()));
    std::println("寒冰贴图地址: {}", static_cast<void *>(共享寒冰贴图.get()));

    // 测试未知类型异常
    // auto 未知贴图 = 贴图工厂::获取贴图("未知"); // 将抛出异常

  } catch (const std::exception &e) {
    std::println("错误: {}", e.what());
  }

  return 0;
}
