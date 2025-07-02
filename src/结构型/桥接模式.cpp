#include <print>
#include <string>

// 渲染器抽象基类 - 定义渲染接口
class 渲染器 {
public:
  virtual void 渲染(const std::string &图形) = 0; // 纯虚函数，需要子类实现
};

// OpenGL渲染实现
class OpenGL渲染器 : public 渲染器 {
public:
  void 渲染(const std::string &图形) override {
    std::println("OpenGL渲染器:{}", 图形); // 实现OpenGL渲染逻辑
  }
};

// DirectX渲染实现
class DirectX渲染器 : public 渲染器 {
public:
  void 渲染(const std::string &图形) override {
    std::println("DirectX渲染器:{}", 图形); // 实现DirectX渲染逻辑
  }
};

// 形状抽象基类 - 使用桥接模式连接渲染器
class 形状 {
protected:
  渲染器 *渲染器实例; // 桥接关键：持有渲染器引用

public:
  // 构造函数注入渲染器实例
  形状(渲染器 *渲染器实例) : 渲染器实例(渲染器实例) {}

  virtual void 绘制() = 0; // 绘制接口
};

// 矩形形状实现
class 矩形 : public 形状 {
public:
  矩形(渲染器 *渲染器实例) : 形状(渲染器实例) {}

  void 绘制() override {
    渲染器实例->渲染("矩形"); // 委托给渲染器渲染矩形
  }
};

// 圆形形状实现
class 圆形 : public 形状 {
public:
  圆形(渲染器 *渲染器实例) : 形状(渲染器实例) {}

  void 绘制() override {
    渲染器实例->渲染("圆形"); // 委托给渲染器渲染圆形
  }
};

// 渲染器工厂抽象基类 - 工厂方法模式
class 渲染器工厂 {
public:
  virtual 渲染器 *创建渲染器() = 0; // 工厂方法接口
};

// OpenGL渲染器工厂
class OpenGL渲染器工厂 : public 渲染器工厂 {
public:
  渲染器 *创建渲染器() override {
    return new OpenGL渲染器(); // 创建OpenGL渲染器实例
  }
};

// DirectX渲染器工厂
class DirectX渲染器工厂 : public 渲染器工厂 {
public:
  渲染器 *创建渲染器() override {
    return new DirectX渲染器(); // 创建DirectX渲染器实例
  }
};

int main(int argc, char *argv[]) {
  // 使用工厂创建渲染器 - 可切换不同实现
  渲染器工厂 *工厂 = new OpenGL渲染器工厂(); // 创建OpenGL工厂
  // 渲染器工厂 *工厂 = new DirectX渲染器工厂(); // 或创建DirectX工厂

  渲染器 *渲染器实例 = 工厂->创建渲染器(); // 工厂方法创建具体渲染器

  // 创建形状并注入渲染器依赖
  形状 *圆形实例 = new 圆形(渲染器实例);

  // 绘制形状（实际调用渲染器渲染）
  圆形实例->绘制();

  return 0;
}
