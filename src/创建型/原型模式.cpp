
#include <memory>

class 球 {
public:
  virtual std::unique_ptr<球> 克隆() = 0;
};

class 爆炸球 : public 球 {
public:
  std::unique_ptr<球> 克隆() override {
    return std::make_unique<爆炸球>(*this); // 调用 爆炸球的复制构造函数
  }
};

class 火焰球 : public 球 {
public:
  std::unique_ptr<球> 克隆() override {
    return std::make_unique<火焰球>(*this); // 调用 爆炸球的复制构造函数
  }

  int 温度{100}; // 如果有成员参数,也会一并复制到新对象中
};
int main(int argc, char *argv[]) {
  // 原型模式,将对象自己变成自己的工厂.
  // 避免 构造时的开销

  return 0;
}
