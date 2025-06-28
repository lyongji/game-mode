#include <concepts>
#include <memory>
#include <print> // C++23 的格式化输出库
#include <string>
#include <vector>

// 游戏角色类 - 最终产品
class 游戏角色 {
public:
  void 设置职业(std::string_view 职业) { 职业 = 职业; }
  void 设置武器(std::string_view 武器) { 武器 = 武器; }
  void 设置护甲(std::string_view 护甲) { 护甲 = 护甲; }
  void 设置等级(int 等级) { 等级 = 等级; }
  void 添加技能(std::string_view 技能) {
    技能列表.push_back(std::string(技能));
  }

  void 显示属性() const {
    using namespace std; // 允许使用println

    println("\n🎮 角色创建完成:");
    println("  🎭 职业: {}", 职业);
    println("  ⚔️ 武器: {}", 武器);
    println("  🛡️ 护甲: {}", 护甲);
    println("  📈 等级: {}", 等级);

    print("  🧪 技能: ");
    for (size_t i = 0; i < 技能列表.size(); ++i) {
      print("{}", 技能列表[i]);
      if (i < 技能列表.size() - 1)
        print(", ");
    }
    println("\n");
  }

private:
  std::string 职业{"未选择"};
  std::string 武器{"无"};
  std::string 护甲{"无"};
  int 等级{1};
  std::vector<std::string> 技能列表;
};

// 建造者概念定义
template <typename T>
concept 角色建造者概念 = requires(T t) {
  { t.构建职业() } -> std::same_as<void>;
  { t.构建武器() } -> std::same_as<void>;
  { t.构建护甲() } -> std::same_as<void>;
  { t.构建等级() } -> std::same_as<void>;
  { t.构建技能() } -> std::same_as<void>;
  { t.获取角色() } -> std::convertible_to<std::unique_ptr<游戏角色>>;
};

// 建造者基类模板 (使用CRTP模式)
template <typename 角色建造者类型> class 角色建造者基类 {
protected:
  std::unique_ptr<游戏角色> 角色 = std::make_unique<游戏角色>();

public:
  virtual ~角色建造者基类() = default;

  void 构建职业() { static_cast<角色建造者类型 *>(this)->构建职业实现(); }
  void 构建武器() { static_cast<角色建造者类型 *>(this)->构建武器实现(); }
  void 构建护甲() { static_cast<角色建造者类型 *>(this)->构建护甲实现(); }
  void 构建等级() { static_cast<角色建造者类型 *>(this)->构建等级实现(); }
  void 构建技能() { static_cast<角色建造者类型 *>(this)->构建技能实现(); }

  std::unique_ptr<游戏角色> 获取角色() { return std::move(角色); }
};

// 战士建造者
class 战士建造者 : public 角色建造者基类<战士建造者> {
  friend class 角色建造者基类<战士建造者>;

private:
  void 构建职业实现() { 角色->设置职业("狂战士"); }
  void 构建武器实现() { 角色->设置武器("巨剑"); }
  void 构建护甲实现() { 角色->设置护甲("板甲"); }
  void 构建等级实现() { 角色->设置等级(10); }
  void 构建技能实现() {
    角色->添加技能("旋风斩");
    角色->添加技能("狂暴");
    角色->添加技能("冲锋");
  }
};

// 法师建造者
class 法师建造者 : public 角色建造者基类<法师建造者> {
  friend class 角色建造者基类<法师建造者>;

private:
  void 构建职业实现() { 角色->设置职业("大法师"); }
  void 构建武器实现() { 角色->设置武器("法杖"); }
  void 构建护甲实现() { 角色->设置护甲("布甲"); }
  void 构建等级实现() { 角色->设置等级(8); }
  void 构建技能实现() {
    角色->添加技能("火球术");
    角色->添加技能("寒冰箭");
    角色->添加技能("传送术");
  }
};

// 弓箭手建造者
class 弓箭手建造者 : public 角色建造者基类<弓箭手建造者> {
  friend class 角色建造者基类<弓箭手建造者>;

private:
  void 构建职业实现() { 角色->设置职业("神射手"); }
  void 构建武器实现() { 角色->设置武器("长弓"); }
  void 构建护甲实现() { 角色->设置护甲("皮甲"); }
  void 构建等级实现() { 角色->设置等级(7); }
  void 构建技能实现() {
    角色->添加技能("多重射击");
    角色->添加技能("精准射击");
    角色->添加技能("陷阱布置");
  }
};

// 导演类模板 - 修复构造函数问题
template <角色建造者概念 T> class 角色导演 {
public:
  // 添加构造函数解决诊断问题
  角色导演(T &建造者) : 建造者(&建造者) {}

  void 构建角色() {
    std::println("开始构建角色...");
    建造者->构建职业();
    建造者->构建武器();
    建造者->构建护甲();
    建造者->构建等级();
    建造者->构建技能();
    std::println("角色构建完成 ✅");
  }

  std::unique_ptr<游戏角色> 获取角色() { return 建造者->获取角色(); }

private:
  T *建造者;
};

// 角色创建工厂函数 - 更新为使用构造函数
template <角色建造者概念 T> std::unique_ptr<游戏角色> 创建角色() {
  T 建造者;
  角色导演 导演(建造者); // 使用构造函数初始化
  导演.构建角色();
  return 导演.获取角色();
}

int main() {
  using namespace std; // 允许使用println

  // 创建标题
  println("==========================================");
  println("      🎮 游戏角色建造者示例 (C++23)       ");
  println("==========================================");
  println("使用 C++23 特性: <print> 库、概念(Concepts)");
  println("          和模板元编程优化\n");

  // 创建战士角色
  println("🛠️ 创建战士角色...");
  auto 战士角色 = 创建角色<战士建造者>();
  战士角色->显示属性();

  // 创建法师角色
  println("🛠️ 创建法师角色...");
  auto 法师角色 = 创建角色<法师建造者>();
  法师角色->显示属性();

  // 创建弓箭手角色
  println("🛠️ 创建弓箭手角色...");
  auto 弓箭手角色 = 创建角色<弓箭手建造者>();
  弓箭手角色->显示属性();

  // 使用导演直接创建角色（修复后）
  println("🛠️ 使用导演直接创建角色...");
  战士建造者 战士建造者实例;
  角色导演 战士导演(战士建造者实例); // 使用构造函数
  战士导演.构建角色();
  auto 自定义战士 = 战士导演.获取角色();
  自定义战士->显示属性();

  println("==========================================");
  println("      所有角色创建完毕，游戏开始！        ");
  println("==========================================");

  return 0;
}
