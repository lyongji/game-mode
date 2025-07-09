#include <iostream>
#include <memory>
#include <string>

// 前向声明
class 游戏角色;

// 状态接口
class 状态接口 {
public:
  virtual ~状态接口() = default;
  virtual void 进入() = 0;
  virtual void 退出() = 0;
  virtual void 更新(float 时间差) = 0;
  virtual void 处理输入(char 输入) = 0;
  virtual std::string 获取状态名() const = 0;
};

// 游戏角色（上下文）
class 游戏角色 {
public:
  游戏角色();

  void 切换状态(std::unique_ptr<状态接口> 新状态) {
    if (当前状态)
      当前状态->退出();
    当前状态 = std::move(新状态);
    当前状态->进入();
  }

  void 更新(float 时间差) { 当前状态->更新(时间差); }
  void 处理输入(char 输入) { 当前状态->处理输入(输入); }
  std::string 获取当前状态名() const { return 当前状态->获取状态名(); }

  // 状态创建方法（解决循环依赖）
  std::unique_ptr<状态接口> 创建站立状态();
  std::unique_ptr<状态接口> 创建跳跃状态();
  std::unique_ptr<状态接口> 创建下蹲状态();
  std::unique_ptr<状态接口> 创建攻击状态();

private:
  std::unique_ptr<状态接口> 当前状态;
};

// 具体状态实现
class 站立状态 : public 状态接口 {
public:
  站立状态(游戏角色 &角色) : 角色(角色) {}

  void 进入() override { std::cout << "进入站立状态\n"; }
  void 退出() override { std::cout << "退出站立状态\n"; }
  void 更新(float) override {}

  void 处理输入(char 输入) override {
    switch (输入) {
    case ' ':
      角色.切换状态(角色.创建跳跃状态());
      break;
    case 'd':
      角色.切换状态(角色.创建下蹲状态());
      break;
    case 'a':
      角色.切换状态(角色.创建攻击状态());
      break;
    }
  }

  std::string 获取状态名() const override { return "站立"; }

private:
  游戏角色 &角色;
};

class 跳跃状态 : public 状态接口 {
public:
  跳跃状态(游戏角色 &角色) : 角色(角色) {}

  void 进入() override {
    std::cout << "进入跳跃状态\n";
    剩余时间 = 1.0f; // 跳跃持续1秒
  }

  void 退出() override { std::cout << "退出跳跃状态\n"; }

  void 更新(float 时间差) override {
    剩余时间 -= 时间差;
    if (剩余时间 <= 0) {
      角色.切换状态(角色.创建站立状态());
    }
  }

  void 处理输入(char 输入) override {
    if (输入 == ' ')
      std::cout << "跳跃中: 无法再次跳跃\n";
  }

  std::string 获取状态名() const override { return "跳跃"; }

private:
  游戏角色 &角色;
  float 剩余时间;
};

class 下蹲状态 : public 状态接口 {
public:
  下蹲状态(游戏角色 &角色) : 角色(角色) {}

  void 进入() override { std::cout << "进入下蹲状态\n"; }
  void 退出() override { std::cout << "退出下蹲状态\n"; }
  void 更新(float) override {}

  void 处理输入(char 输入) override {
    if (输入 == 'd')
      角色.切换状态(角色.创建站立状态());
  }

  std::string 获取状态名() const override { return "下蹲"; }

private:
  游戏角色 &角色;
};

class 攻击状态 : public 状态接口 {
public:
  攻击状态(游戏角色 &角色) : 角色(角色) {}

  void 进入() override {
    std::cout << "进入攻击状态\n";
    攻击计时器 = 0.5f; // 攻击动画持续0.5秒
  }

  void 退出() override { std::cout << "退出攻击状态\n"; }

  void 更新(float 时间差) override {
    攻击计时器 -= 时间差;
    if (攻击计时器 <= 0) {
      角色.切换状态(角色.创建站立状态());
    }
  }

  void 处理输入(char 输入) override {
    if (输入 == 'a')
      std::cout << "攻击中: 无法再次攻击\n";
  }

  std::string 获取状态名() const override { return "攻击"; }

private:
  游戏角色 &角色;
  float 攻击计时器;
};

// 游戏角色成员函数实现（在状态类定义后）
游戏角色::游戏角色() { 切换状态(创建站立状态()); }

std::unique_ptr<状态接口> 游戏角色::创建站立状态() {
  return std::make_unique<站立状态>(*this);
}

std::unique_ptr<状态接口> 游戏角色::创建跳跃状态() {
  return std::make_unique<跳跃状态>(*this);
}

std::unique_ptr<状态接口> 游戏角色::创建下蹲状态() {
  return std::make_unique<下蹲状态>(*this);
}

std::unique_ptr<状态接口> 游戏角色::创建攻击状态() {
  return std::make_unique<攻击状态>(*this);
}

int main() {
  游戏角色 角色;

  std::cout << "初始状态: " << 角色.获取当前状态名() << "\n";

  std::cout << "\n按下空格键（跳跃）\n";
  角色.处理输入(' ');
  std::cout << "当前状态: " << 角色.获取当前状态名() << "\n";

  std::cout << "\n更新游戏（0.7秒后）\n";
  角色.更新(0.7f);
  std::cout << "当前状态: " << 角色.获取当前状态名() << "\n";

  std::cout << "\n按下下蹲键\n";
  角色.处理输入('d');
  std::cout << "当前状态: " << 角色.获取当前状态名() << "\n";

  std::cout << "\n再次按下下蹲键\n";
  角色.处理输入('d');
  std::cout << "当前状态: " << 角色.获取当前状态名() << "\n";

  std::cout << "\n按下攻击键\n";
  角色.处理输入('a');
  std::cout << "当前状态: " << 角色.获取当前状态名() << "\n";

  std::cout << "\n更新游戏（0.6秒后）\n";
  角色.更新(0.6f);
  std::cout << "当前状态: " << 角色.获取当前状态名() << "\n";

  std::cout << "\n按下下蹲键\n";
  角色.处理输入('d');
  std::cout << "当前状态: " << 角色.获取当前状态名() << "\n";

  return 0;
}
