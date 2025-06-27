#include <print>

struct 子弹 {
  virtual void 激发() = 0;
};

struct 实体子弹 : 子弹 {
  void 激发() override { std::println("物理伤害"); }
};

struct 能量子弹 : 子弹 {
  void 激发() override { std::println("能量伤害"); }
};

struct 枪 {
  virtual 子弹 *射击() = 0;
};

// 子弹工厂
struct 手枪 : 枪 {
  子弹 *射击() override { return new 实体子弹; }
};

struct 能量枪 : 枪 {
  子弹 *射击() override { return new 能量子弹; }
};

void 玩家(枪 *枪实例) {
  for (int i = 0; i < 10; i++) {
    子弹 *射出子弹 = 枪实例->射击();
    射出子弹->激发();
  }
}

int main() {

  玩家(new 手枪);
  玩家(new 能量枪);

  return 0;
}
