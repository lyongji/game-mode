#include <print>
#include <stdexcept>
#include <type_traits>

struct 子弹 {
  virtual void 激发() = 0;
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

// 可以为工厂添加参数
struct ak47 : 枪 {
  bool 受潮状态 = true;
  ak47(bool 受潮状态) : 受潮状态(受潮状态) {}
  子弹 *射击() override {
    if (受潮状态) {
      return new ak47子弹(5); // 受潮了
    } else {
      return new ak47子弹(10); // 正常攻击力
    }
  }
};

// 使用模板批量制造子弹工厂
template <class 子弹类> struct 子弹枪 : 枪 {
  // 类型检查是否是子弹的子类
  static_assert(std::is_base_of<子弹, 子弹类>::value, "必须是子弹的子类");

  子弹 *射击() override { return new 子弹类(); }
};

// 超级工厂模式
枪 *获得枪(std::string 名称) {
  if (名称 == "ak47") {
    return new 子弹枪<实体子弹>();
  } else if (名称 == "能量枪") {
    return new 子弹枪<能量子弹>();
  } else {
    throw std::runtime_error("没有这种枪!");
  }
}

void 玩家(枪 *枪实例) {
  for (int i = 0; i < 2; i++) {
    子弹 *射出子弹 = 枪实例->射击();
    射出子弹->激发();
  }
}

int main() {
  // 子弹工厂
  玩家(new 手枪);
  玩家(new 能量枪);
  // 可以为工厂添加参数
  玩家(new ak47(true));
  // 使用模板批量制造子弹工厂
  玩家(new 子弹枪<实体子弹>());
  玩家(new 子弹枪<能量子弹>());
  // 超级工厂模式
  玩家(获得枪("ak47"));
  玩家(获得枪("能量枪"));

  return 0;
}
