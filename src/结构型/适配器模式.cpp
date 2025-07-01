#include <print>

class 键盘设备 {
public:
  键盘设备() {}
  ~键盘设备() {}
  bool W键按下() const {
    std::println("W键按下");
    return true;
  }
  bool J键按下() const {
    std::println("J键按下");
    return true;
  }
  bool K键按下() const {
    std::println("K键按下");
    return true;
  }
};

class 手柄设备 {
public:
  手柄设备() {}
  ~手柄设备() {}
  bool 左摇杆上推() const {
    std::println("左摇杆上推");
    return true;
  }
  bool A键按下() const {
    std::println("A键按下");
    return true;
  }
  bool B键按下() const {
    std::println("B键按下");
    return true;
  }
};

class 游戏控制器 {
public:
  virtual bool 向前() = 0;
  virtual bool 攻击() = 0;
  virtual bool 跳跃() = 0;
};
// 手柄适配器
class 手柄适配器 : public 游戏控制器 {
  const 手柄设备 *手柄 = nullptr;

public:
  手柄适配器(const 手柄设备 *手柄实例) : 手柄(手柄实例) {}
  bool 向前() override { return 手柄->左摇杆上推(); }
  bool 攻击() override { return 手柄->A键按下(); }
  bool 跳跃() override { return 手柄->B键按下(); }
};

// 键盘适配器
class 键盘适配器 : public 游戏控制器 {
  const 键盘设备 *键盘 = nullptr;

public:
  键盘适配器(const 键盘设备 *键盘实例) : 键盘(键盘实例) {}
  bool 向前() override { return 键盘->W键按下(); }
  bool 攻击() override { return 键盘->J键按下(); }
  bool 跳跃() override { return 键盘->K键按下(); }
};

void 接受输入(游戏控制器 &输入设备) {
  输入设备.向前();
  输入设备.攻击();
  输入设备.跳跃();
}

int main() {
  手柄设备 手柄;
  键盘设备 键盘;
  手柄适配器 手柄输入(&手柄);
  键盘适配器 键盘输入(&键盘);

  接受输入(手柄输入);
  接受输入(键盘输入);
  return 0;
}
