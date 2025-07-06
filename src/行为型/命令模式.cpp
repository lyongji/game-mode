#include <iostream>
#include <memory>
#include <print> // C++23 标准打印头文件
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

// 接收者：游戏角色
class 游戏角色 {
public:
  void 移动(int dx, int dy) {
    x += dx;
    y += dy;
    std::println("角色移动到: ({}, {})", x, y);
  }

  void 攻击() { std::println("角色攻击!"); }

  void 跳跃() { std::println("角色跳跃!"); }

  void 显示位置() const { std::println("当前位置: ({}, {})", x, y); }

  void 撤销移动(int dx, int dy) {
    x -= dx;
    y -= dy;
    std::println("撤销移动: 回到位置({}, {})", x, y);
  }

private:
  int x = 0;
  int y = 0;
};

// 命令接口
class 命令 {
public:
  virtual ~命令() = default;
  virtual void 执行() = 0;
  virtual void 撤销() = 0;
  virtual std::string 获取名称() const = 0;
};

// 具体命令：移动
class 移动命令 : public 命令 {
public:
  移动命令(游戏角色 &角色, int dx, int dy) : 角色(角色), dx(dx), dy(dy) {}

  void 执行() override { 角色.移动(dx, dy); }

  void 撤销() override { 角色.撤销移动(dx, dy); }

  std::string 获取名称() const override {
    return std::format("移动({}, {})", dx, dy);
  }

private:
  游戏角色 &角色;
  int dx, dy;
};

// 具体命令：攻击
class 攻击命令 : public 命令 {
public:
  攻击命令(游戏角色 &角色) : 角色(角色) {}

  void 执行() override { 角色.攻击(); }

  void 撤销() override { std::println("撤销攻击: 攻击效果不可撤销"); }

  std::string 获取名称() const override { return "攻击"; }

private:
  游戏角色 &角色;
};

// 具体命令：跳跃
class 跳跃命令 : public 命令 {
public:
  跳跃命令(游戏角色 &角色) : 角色(角色) {}

  void 执行() override { 角色.跳跃(); }

  void 撤销() override { std::println("撤销跳跃: 角色落地"); }

  std::string 获取名称() const override { return "跳跃"; }

private:
  游戏角色 &角色;
};

// 宏命令：组合多个命令
class 宏命令 : public 命令 {
public:
  void 添加命令(std::shared_ptr<命令> 命令) { 命令列表.push_back(命令); }

  void 执行() override {
    for (auto &命令 : 命令列表) {
      命令->执行();
    }
  }

  void 撤销() override {
    // 按相反顺序撤销
    for (auto it = 命令列表.rbegin(); it != 命令列表.rend(); ++it) {
      (*it)->撤销();
    }
  }

  std::string 获取名称() const override {
    std::string 名称 = "宏命令[";
    for (size_t i = 0; i < 命令列表.size(); ++i) {
      名称 += 命令列表[i]->获取名称();
      if (i < 命令列表.size() - 1)
        名称 += ", ";
    }
    名称 += "]";
    return 名称;
  }

private:
  std::vector<std::shared_ptr<命令>> 命令列表;
};

// 调用者：输入处理器
class 输入处理器 {
public:
  void 设置命令(std::shared_ptr<命令> 命令) { 当前命令 = 命令; }

  void 执行命令() {
    if (当前命令) {
      当前命令->执行();
      命令历史.push(当前命令);
      // 清空重做栈
      while (!重做栈.empty())
        重做栈.pop();
    }
  }

  void 撤销() {
    if (!命令历史.empty()) {
      auto 最后命令 = 命令历史.top();
      命令历史.pop();
      最后命令->撤销();
      重做栈.push(最后命令);
    }
  }

  void 重做() {
    if (!重做栈.empty()) {
      auto 命令 = 重做栈.top();
      重做栈.pop();
      命令->执行();
      命令历史.push(命令);
    }
  }

  void 绑定按键(char 按键, std::shared_ptr<命令> 命令) {
    按键映射[按键] = 命令;
  }

  void 处理输入(char 按键) {
    if (按键映射.find(按键) != 按键映射.end()) {
      设置命令(按键映射[按键]);
      执行命令();
    } else {
      std::println("未知按键: {}", 按键);
    }
  }

  void 显示按键配置() const {
    std::println("\n按键配置:");
    for (const auto &[按键, 命令] : 按键映射) {
      std::println("  {}: {}", 按键, 命令->获取名称());
    }
  }

private:
  std::shared_ptr<命令> 当前命令;
  std::stack<std::shared_ptr<命令>> 命令历史;
  std::stack<std::shared_ptr<命令>> 重做栈;
  std::unordered_map<char, std::shared_ptr<命令>> 按键映射;
};

int main() {
  // 创建游戏角色
  游戏角色 角色;

  // 创建命令对象
  auto 上移 = std::make_shared<移动命令>(角色, 0, 1);
  auto 下移 = std::make_shared<移动命令>(角色, 0, -1);
  auto 左移 = std::make_shared<移动命令>(角色, -1, 0);
  auto 右移 = std::make_shared<移动命令>(角色, 1, 0);
  auto 攻击 = std::make_shared<攻击命令>(角色);
  auto 跳跃 = std::make_shared<跳跃命令>(角色);

  // 创建宏命令：连招
  auto 连招 = std::make_shared<宏命令>();
  连招->添加命令(跳跃);
  连招->添加命令(攻击);
  连招->添加命令(std::make_shared<移动命令>(角色, 1, 0)); // 向右移动

  // 创建输入处理器
  输入处理器 输入;

  // 配置按键
  输入.绑定按键('w', 上移);
  输入.绑定按键('s', 下移);
  输入.绑定按键('a', 左移);
  输入.绑定按键('d', 右移);
  输入.绑定按键('j', 攻击);
  输入.绑定按键('k', 跳跃);
  输入.绑定按键('m', 连招);

  // 显示按键配置
  输入.显示按键配置();

  // 游戏主循环
  std::println("\n===== 游戏开始 =====");
  char 按键;

  while (true) {
    std::println(
        "\n输入按键 (w/a/s/d移动, j攻击, k跳跃, m连招, u撤销, r重做, q退出):");
    std::cin >> 按键;

    if (按键 == 'q')
      break;

    switch (按键) {
    case 'u':
      输入.撤销();
      break;
    case 'r':
      输入.重做();
      break;
    default:
      输入.处理输入(按键);
    }

    // 显示角色位置
    角色.显示位置();
  }

  std::println("游戏结束");
}
