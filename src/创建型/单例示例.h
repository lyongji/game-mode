// 单例示例.h
#pragma once

class 游戏饿汉 {
public:
  // 显式删除所有拷贝控制成员（拷贝构造、移动构造、拷贝赋值、移动赋值）
  // 防止通过任何方式复制实例
  游戏饿汉(游戏饿汉 &&) = delete;
  游戏饿汉(const 游戏饿汉 &) = delete;
  游戏饿汉 &operator=(const 游戏饿汉 &) = delete;
  游戏饿汉 &operator=(游戏饿汉 &&) = delete;

  static 游戏饿汉 实例;
  void 更新();

private:
  // 构造函数在私有区,无法在类外构造类
  游戏饿汉() {}
};

class 游戏懒汉 {
public:
  游戏懒汉(游戏懒汉 &&) = delete;                 // 修正参数类型
  游戏懒汉(const 游戏懒汉 &) = delete;            // 增加拷贝构造删除
  游戏懒汉 &operator=(const 游戏懒汉 &) = delete; // 增加赋值运算符删除
  游戏懒汉 &operator=(游戏懒汉 &&) = delete;      // 移动赋值删除

  static 游戏懒汉 &实例化();

private:
  游戏懒汉() {}
};

// 单例模式通用懒汉模板
// 任何类型 T，只要以 singleton<T>() 形式获取，都能保证每个 T
// 都只有一份对象。（前提是你不要再 T() 创建对象）
// -------------------------------------------------
// 警告：这种写法同样不适用于多 DLL 的情况！如果需要在多 DLL
// 环境中使用，请乖乖分离模板的声明和定义。
template <typename 类型> class 单例对象 {
public:
  static 类型 &实例化() {
    static 类型 实例;
    return 实例;
  }
  单例对象(const 单例对象 &) = delete;
  单例对象 &operator=(const 单例对象 &) = delete;

protected:
  单例对象() = default;
};

// 使用 单例对象类模板
class 游戏管理器 : public 单例对象<游戏管理器> {
  friend class 单例对象<游戏管理器>;

private:
  游戏管理器() {} // 私有构造函数
};
