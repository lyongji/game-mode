#include <memory>
#include <print>
#include <string>

// 组件接口：饮料
class 饮料 {
public:
  virtual ~饮料() = default;
  virtual std::string 描述() const = 0;
  virtual double 价格() const = 0;
};

// 具体组件：浓缩咖啡
class 浓缩咖啡 : public 饮料 {
public:
  std::string 描述() const override { return "浓缩咖啡"; }

  double 价格() const override { return 12.0; }
};

// 具体组件：红茶
class 红茶 : public 饮料 {
public:
  std::string 描述() const override { return "红茶"; }

  double 价格() const override { return 8.0; }
};

// 装饰器基类
class 调料装饰器 : public 饮料 {
protected:
  饮料 *被装饰的饮料;

public:
  explicit 调料装饰器(饮料 *饮料实例) : 被装饰的饮料(饮料实例) {}

  std::string 描述() const override { return 被装饰的饮料->描述(); }

  double 价格() const override { return 被装饰的饮料->价格(); }
};

// 具体装饰器：牛奶
class 牛奶 : public 调料装饰器 {
public:
  using 调料装饰器::调料装饰器;

  std::string 描述() const override { return 调料装饰器::描述() + " + 牛奶"; }

  double 价格() const override { return 调料装饰器::价格() + 3.0; }
};

// 具体装饰器：糖浆
class 糖浆 : public 调料装饰器 {
public:
  using 调料装饰器::调料装饰器;

  std::string 描述() const override { return 调料装饰器::描述() + " + 糖浆"; }

  double 价格() const override { return 调料装饰器::价格() + 2.0; }
};

// 具体装饰器：奶油
class 奶油 : public 调料装饰器 {
public:
  using 调料装饰器::调料装饰器;

  std::string 描述() const override { return 调料装饰器::描述() + " + 奶油"; }

  double 价格() const override { return 调料装饰器::价格() + 4.0; }
};

// 具体装饰器：珍珠（用于红茶）
class 珍珠 : public 调料装饰器 {
public:
  using 调料装饰器::调料装饰器;

  std::string 描述() const override { return 调料装饰器::描述() + " + 珍珠"; }

  double 价格() const override { return 调料装饰器::价格() + 5.0; }
};

// 打印饮料信息
void 打印饮料信息(const 饮料 *饮品) {
  std::println("饮料: {} | 价格:{}元", 饮品->描述(), 饮品->价格());
}

int main() {
  // 创建基础浓缩咖啡
  std::println("===== 制作浓缩咖啡 =====");
  饮料 *我的咖啡 = new 浓缩咖啡();
  打印饮料信息(我的咖啡);

  // 添加牛奶
  std::println("===== 添加牛奶 =====");
  我的咖啡 = new 牛奶(我的咖啡);
  打印饮料信息(我的咖啡);

  // 添加糖浆
  std::println("===== 添加糖浆 =====");
  我的咖啡 = new 糖浆(我的咖啡);
  打印饮料信息(我的咖啡);

  // 添加奶油
  std::println("===== 添加糖浆 =====");
  我的咖啡 = new 奶油(我的咖啡);
  打印饮料信息(我的咖啡);

  // 制作珍珠奶茶
  std::println("===== 制作珍珠奶茶 =====");
  饮料 *我的奶茶 = new 红茶();
  我的奶茶 = new 牛奶(我的奶茶);
  我的奶茶 = new 糖浆(我的奶茶);
  我的奶茶 = new 珍珠(我的奶茶);
  打印饮料信息(我的奶茶);

  // 使用智能指针的安全方式
  std::println("===== 智能指针版 =====");
  auto 摩卡咖啡 =
      std::shared_ptr<饮料>(new 奶油(new 糖浆(new 牛奶(new 浓缩咖啡()))));

  std::println("豪华摩卡咖啡: {} |总价: {}元", 摩卡咖啡->描述(),
               摩卡咖啡->价格());

  // 清理内存
  delete 我的咖啡;
  delete 我的奶茶;

  return 0;
}
