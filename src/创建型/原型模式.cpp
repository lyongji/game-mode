
#include <memory>
#include <print>

class 球 {
public:
  virtual ~球() = default;
  virtual std::unique_ptr<球> 克隆() const = 0;
  // 需要访问派生类成员时
  template <typename 子类型> 子类型 *作为() {
    return dynamic_cast<子类型 *>(this);
  }
};

// CRTP 模式
template <class 子类> class 球接口 : public 球 {
public:
  std::unique_ptr<球>
  克隆() const override { // 使用模板自动实现 克隆 的辅助工具类
    const 子类 *子类实例 =
        static_cast<const 子类 *>(this); // 静态转换 子类为父类
    return std::make_unique<子类>(*子类实例);
  }
};

class 爆炸球 : public 球接口<爆炸球> {};

class 火焰球 : public 球接口<火焰球> {
public:
  int 温度{100}; // 如果有成员参数,也会一并复制到新对象中
};
int main(int argc, char *argv[]) {
  // 原型模式,将对象自己变成自己的工厂.
  // 避免 构造时的开销
  火焰球 火球;
  std::println("{}", 火球.温度);

  auto 火球2 = 火球.克隆(); // 返回的是基类指针
  // 使用 dynamic_cast 基类指针转换为派生类指针
  if (auto 火球2指针 = dynamic_cast<火焰球 *>(火球2.get())) {
    std::println("{}", 火球2指针->温度); // 输出100
  }
  // 使用 static_cast 静态转换 为派生类
  auto 火球2指针 = static_cast<火焰球 *>(火球2.get());
  std::println("{}", 火球2指针->温度); // 输出100

  // 使用 父类 提供的访问函数
  auto 火球2指针2 = 火球2->作为<火焰球>();
  std::println("{}", 火球2指针2->温度); // 输出100

  return 0;
}
