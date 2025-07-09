#include <iostream>
#include <memory>
#include <string>

// 策略接口：攻击行为
class 攻击策略 {
public:
  virtual ~攻击策略() = default;
  virtual void 执行攻击() const = 0;
  virtual std::string 获取策略名() const = 0;
};

// 具体策略：近战攻击
class 近战攻击 : public 攻击策略 {
public:
  void 执行攻击() const override {
    std::cout << "使用剑进行近战攻击！造成15点伤害\n";
  }

  std::string 获取策略名() const override { return "近战攻击"; }
};

// 具体策略：远程攻击
class 远程攻击 : public 攻击策略 {
public:
  void 执行攻击() const override {
    std::cout << "使用弓箭进行远程射击！造成10点伤害\n";
  }

  std::string 获取策略名() const override { return "远程攻击"; }
};

// 具体策略：魔法攻击
class 魔法攻击 : public 攻击策略 {
public:
  void 执行攻击() const override {
    std::cout << "施放火球术！造成25点魔法伤害\n";
  }

  std::string 获取策略名() const override { return "魔法攻击"; }
};

// 上下文：游戏角色
class 游戏角色 {
public:
  explicit 游戏角色(std::unique_ptr<攻击策略> 策略)
      : 当前攻击策略(std::move(策略)) {}

  void 设置攻击策略(std::unique_ptr<攻击策略> 新策略) {
    当前攻击策略 = std::move(新策略);
  }

  void 执行攻击() const {
    if (当前攻击策略) {
      当前攻击策略->执行攻击();
    } else {
      std::cout << "没有攻击策略！\n";
    }
  }

  void 显示当前策略() const {
    std::cout << "当前攻击策略: " << 当前攻击策略->获取策略名() << "\n";
  }

private:
  std::unique_ptr<攻击策略> 当前攻击策略;
};

int main() {
  // 创建角色并设置初始策略
  游戏角色 战士(std::make_unique<近战攻击>());
  std::cout << "=== 战士 ===\n";
  战士.显示当前策略();
  战士.执行攻击();

  // 切换策略
  std::cout << "\n战士更换武器...\n";
  战士.设置攻击策略(std::make_unique<远程攻击>());
  战士.显示当前策略();
  战士.执行攻击();

  // 创建法师角色
  游戏角色 法师(std::make_unique<魔法攻击>());
  std::cout << "\n=== 法师 ===\n";
  法师.显示当前策略();
  法师.执行攻击();

  // 法师临时切换策略
  std::cout << "\n法师使用备用武器...\n";
  法师.设置攻击策略(std::make_unique<近战攻击>());
  法师.显示当前策略();
  法师.执行攻击();

  return 0;
}
