#include <memory>
#include <optional>
#include <print>
#include <string>
#include <unordered_map>
#include <vector>

// 玩家数据结构
struct 玩家数据 {
  std::unordered_map<std::string, int> 击杀记录;
  std::unordered_map<std::string, bool> 物品收集;
  std::unordered_map<std::string, bool> 区域探索;
  std::unordered_map<std::string, bool> NPC对话;
};

// 任务类型枚举
enum class 任务类型 { 击杀, 收集, 探索, 对话, 未知 };

// 任务验证请求结构体
struct 任务验证请求 {
  任务类型 类型;
  std::string 目标ID;
  int 需求数量 = 1;
  const 玩家数据 &玩家;
};

// 处理器接口
class 任务验证器 {
public:
  virtual ~任务验证器() = default;

  // 链式设置下一个处理器
  std::shared_ptr<任务验证器> 设置下一个(std::shared_ptr<任务验证器> 下一个) {
    下一个处理器 = 下一个;
    return 下一个;
  }

  // 处理请求入口
  std::string 处理(const 任务验证请求 &请求) {
    if (auto 结果 = 处理请求(请求)) {
      return *结果;
    }
    return 传递请求(请求);
  }

protected:
  virtual std::optional<std::string> 处理请求(const 任务验证请求 &) = 0;

  std::string 传递请求(const 任务验证请求 &请求) {
    if (下一个处理器) {
      return 下一个处理器->处理(请求);
    }
    return "⚠️ 错误：未知任务类型";
  }

private:
  std::shared_ptr<任务验证器> 下一个处理器;
};

// 具体处理器：击杀验证
class 击杀验证器 : public 任务验证器 {
protected:
  std::optional<std::string> 处理请求(const 任务验证请求 &请求) override {
    if (请求.类型 != 任务类型::击杀)
      return std::nullopt;

    const auto &记录 = 请求.玩家.击杀记录;
    int 当前数量 = 记录.count(请求.目标ID) ? 记录.at(请求.目标ID) : 0;

    if (当前数量 >= 请求.需求数量) {
      return std::format("✅ 击杀验证: 已击杀 {} ({}≥{})", 请求.目标ID,
                         当前数量, 请求.需求数量);
    }
    return std::format("❌ 击杀验证: 需要击杀 {} 个 {} (当前:{})",
                       请求.需求数量, 请求.目标ID, 当前数量);
  }
};

// 具体处理器：收集验证
class 收集验证器 : public 任务验证器 {
protected:
  std::optional<std::string> 处理请求(const 任务验证请求 &请求) override {
    if (请求.类型 != 任务类型::收集)
      return std::nullopt;

    const auto &收集品 = 请求.玩家.物品收集;
    bool 已收集 = 收集品.count(请求.目标ID) && 收集品.at(请求.目标ID);

    return 已收集 ? std::format("✅ 收集验证: 已收集物品 {}", 请求.目标ID)
                  : std::format("❌ 收集验证: 需要收集物品 {}", 请求.目标ID);
  }
};

// 具体处理器：探索验证
class 探索验证器 : public 任务验证器 {
protected:
  std::optional<std::string> 处理请求(const 任务验证请求 &请求) override {
    if (请求.类型 != 任务类型::探索)
      return std::nullopt;

    const auto &区域 = 请求.玩家.区域探索;
    bool 已探索 = 区域.count(请求.目标ID) && 区域.at(请求.目标ID);

    return 已探索 ? std::format("✅ 探索验证: 已探索区域 {}", 请求.目标ID)
                  : std::format("❌ 探索验证: 需要探索区域 {}", 请求.目标ID);
  }
};

// 具体处理器：对话验证
class 对话验证器 : public 任务验证器 {
protected:
  std::optional<std::string> 处理请求(const 任务验证请求 &请求) override {
    if (请求.类型 != 任务类型::对话)
      return std::nullopt;

    const auto &NPC列表 = 请求.玩家.NPC对话;
    bool 已对话 = NPC列表.count(请求.目标ID) && NPC列表.at(请求.目标ID);

    return 已对话 ? std::format("✅ 对话验证: 已与 {} 对话", 请求.目标ID)
                  : std::format("❌ 对话验证: 需要与 {} 对话", 请求.目标ID);
  }
};

// 默认处理器（处理未知类型）
class 默认验证器 : public 任务验证器 {
protected:
  std::optional<std::string> 处理请求(const 任务验证请求 &) override {
    return "⚠️ 错误：无法处理的任务类型";
  }
};

// 获取任务类型名称
std::string 获取类型名称(任务类型 类型) {
  switch (类型) {
  case 任务类型::击杀:
    return "击杀";
  case 任务类型::收集:
    return "收集";
  case 任务类型::探索:
    return "探索";
  case 任务类型::对话:
    return "对话";
  default:
    return "未知";
  }
}

int main() {
  // 初始化玩家数据
  玩家数据 当前玩家{.击杀记录 = {{"史莱姆", 5}, {"巨龙", 2}},
                    .物品收集 = {{"圣剑", true}, {"治疗药水", false}},
                    .区域探索 = {{"迷雾森林", true}, {"龙之洞穴", false}},
                    .NPC对话 = {{"商人", true}, {"国王", false}}};

  // 构建责任链 (使用智能指针自动管理内存)
  auto 击杀处理器 = std::make_shared<击杀验证器>();
  auto 收集处理器 = std::make_shared<收集验证器>();
  auto 探索处理器 = std::make_shared<探索验证器>();
  auto 对话处理器 = std::make_shared<对话验证器>();
  auto 默认处理器 = std::make_shared<默认验证器>();

  // 链接处理器
  击杀处理器->设置下一个(收集处理器)
      ->设置下一个(探索处理器)
      ->设置下一个(对话处理器)
      ->设置下一个(默认处理器);

  // 创建测试任务
  std::vector<任务验证请求> 任务列表 = {
      {任务类型::击杀, "巨龙", 3, 当前玩家},
      {任务类型::收集, "治疗药水", 1, 当前玩家},
      {任务类型::探索, "龙之洞穴", 1, 当前玩家},
      {任务类型::对话, "国王", 1, 当前玩家},
      {任务类型::击杀, "史莱姆", 5, 当前玩家},
      {任务类型::收集, "圣剑", 1, 当前玩家},
      {任务类型::未知, "神秘任务", 1, 当前玩家} // 测试未知类型
  };

  // 输出结果
  std::println("\n===== 任务验证结果 =====");
  for (const auto &任务 : 任务列表) {
    std::println("\n[{}任务] 目标: {}", 获取类型名称(任务.类型), 任务.目标ID);

    std::string 结果 = 击杀处理器->处理(任务);
    std::println("  {}", 结果);
  }

  std::println("\n===== 验证结束 =====");
}
