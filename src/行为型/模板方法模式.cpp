#include <iostream>
#include <memory>
// 抽象战斗类
class 战斗流程 {
public:
  virtual ~战斗流程() = default;
  // 模板方法：定义战斗流程
  void 执行战斗流程() {
    准备战斗();
    执行战斗();
    结束战斗();
  }

protected:
  // 准备战斗（默认实现）
  virtual void 准备战斗() {
    std::cout << "角色正在准备武器和护甲..." << std::endl;
  }
  // 执行战斗（纯虚函数，由子类实现）
  virtual void 执行战斗() = 0;
  // 结束战斗（默认实现）
  virtual void 结束战斗() {
    std::cout << "战斗结束，清理战场并恢复状态。" << std::endl;
  }
};
// 战士类
class 战士 : public 战斗流程 {
protected:
  void 执行战斗() override {
    std::cout << "战士使用剑进行近战攻击！" << std::endl;
  }
};
// 法师类
class 法师 : public 战斗流程 {
protected:
  void 准备战斗() override {
    std::cout << "法师正在准备法术书和魔杖..." << std::endl;
  }
  void 执行战斗() override { std::cout << "法师施放火球术！" << std::endl; }
  void 结束战斗() override {
    战斗流程::结束战斗(); // 调用基类的结束战斗
    std::cout << "法师冥想以恢复魔力。" << std::endl;
  }
};
int main() {
  std::unique_ptr<战斗流程> 战士战斗 = std::make_unique<战士>();
  std::unique_ptr<战斗流程> 法师战斗 = std::make_unique<法师>();
  std::cout << "===== 战士的战斗流程 =====" << std::endl;
  战士战斗->执行战斗流程();
  std::cout << "\n===== 法师的战斗流程 =====" << std::endl;
  法师战斗->执行战斗流程();
  return 0;
}
