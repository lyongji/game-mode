#include <functional>
#include <memory>
#include <print>
#include <stdexcept>
#include <utility>

struct 子弹 {
  virtual void 激发() = 0;
  virtual ~子弹() = default;
};

struct 实体子弹 : 子弹 {
  void 激发() override { std::println("物理伤害"); }
};

struct 能量子弹 : 子弹 {
  void 激发() override { std::println("能量伤害"); }
};

struct ak47子弹 : 子弹 {
  int 攻击力 = 0;
  ak47子弹(int 攻击力) : 攻击力(攻击力) {}
  void 激发() override { std::println("造成{}点物理伤害", 攻击力); }
};

class 枪 {
public:
  // 智能指针 自动管理内存
  virtual std::unique_ptr<子弹> 射击() = 0;
  virtual ~枪() = default;
};

// 使用模板批量制造子弹工厂
// 使用概念库 <concepts>约束为从子弹派生的类型  std::derived_from<子弹>子弹类
template <std::derived_from<子弹> 子弹类> class 子弹枪 : public 枪 {

  // 使用 move_only_function 更高效
  using 创建函数类型 = std::move_only_function<std::unique_ptr<子弹类>()>;
  创建函数类型 创建函数;

public:
  // 默认构造 (无参)
  子弹枪() : 创建函数([] { return std::make_unique<子弹类>(); }) {}

  // 带参数构造 - 使用完美转发
  template <typename... 参数类型>
    requires std::constructible_from<子弹类, 参数类型...>
  explicit 子弹枪(参数类型 &&...参数)
      : 创建函数([... 捕获参数 = std::forward<参数类型>(
                      参数)]() mutable -> std::unique_ptr<子弹类> {
          return std::make_unique<子弹类>(
              std::forward<decltype(捕获参数)>(捕获参数)...);
        }) {}
  // 初始化列表专用构造函数
  template <typename T>
    requires requires(std::initializer_list<T> il) {
      { std::make_unique<子弹类>(il) } -> std::same_as<std::unique_ptr<子弹类>>;
    }
  explicit 子弹枪(std::initializer_list<T> il)
      : 创建函数([il]() mutable { return std::make_unique<子弹类>(il); }) {}

  // 支持工厂函数作为参数
  explicit 子弹枪(创建函数类型 工厂函数) : 创建函数(std::move(工厂函数)) {}

  // 移动操作
  子弹枪(子弹枪 &&) = default;
  子弹枪 &operator=(子弹枪 &&) = default;

  // 禁止复制
  子弹枪(const 子弹枪 &) = delete;
  子弹枪 &operator=(const 子弹枪 &) = delete;

  std::unique_ptr<子弹> 射击() override { return 创建函数(); }
};

void 玩家(枪 &枪实例) {
  for (int i = 0; i < 2; i++) {
    std::unique_ptr<子弹> 射出子弹 = 枪实例.射击();
    射出子弹->激发();
  }
}

int main() {
  // 1. 无参构造
  auto 手枪 = 子弹枪<实体子弹>();
  auto b1 = 手枪.射击();
  b1->激发(); // 实体子弹激发

  // 2. 带简单参数
  auto ak47 = 子弹枪<ak47子弹>(100);
  auto b2 = ak47.射击();
  b2->激发(); // 造成100点物理伤害

  // 3. 带复杂参数
  struct 元素子弹 : 子弹 {
    std::string 元素;
    int 伤害;
    元素子弹(std::string 元素, int 伤害) : 元素(std::move(元素)), 伤害(伤害) {}
    void 激发() override { std::println("{}元素伤害: {}点", 元素, 伤害); }
  };

  auto 火焰枪 = 子弹枪<元素子弹>("火焰", 150);
  auto b3 = 火焰枪.射击();
  b3->激发(); // 火焰元素伤害: 150点

  // 4. 使用初始化列表
  struct 霰弹 : 子弹 {
    std::vector<int> 弹丸;
    霰弹(std::initializer_list<int> 弹丸) : 弹丸(弹丸) {}
    void 激发() override {
      std::print("霰弹发射: ");
      for (int d : 弹丸)
        std::print("{} ", d);
      std::println("");
    }
  };

  auto 霰弹枪 = 子弹枪<霰弹>({10, 20, 30, 40});
  auto b4 = 霰弹枪.射击();
  b4->激发(); // 霰弹发射: 10 20 30 40

  // 5. 使用自定义工厂函数
  auto 随机伤害工厂 = [] {
    int 伤害 = 80 + (std::rand() % 40); // 80-119
    return std::make_unique<ak47子弹>(伤害);
  };

  auto 随机枪 = 子弹枪<ak47子弹>(std::move(随机伤害工厂));
  auto b5 = 随机枪.射击();
  b5->激发(); // 造成[80-119]点物理伤害

  // 6. 支持移动语义
  auto 创建冰冻枪 = [] { return 子弹枪<元素子弹>("冰冻", 75); };

  auto 冰冻枪 = 创建冰冻枪(); // 移动构造
  auto b6 = 冰冻枪.射击();
  b6->激发(); // 冰冻元素伤害: 75点

  玩家(冰冻枪);
  return 0;
}
