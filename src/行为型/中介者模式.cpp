#include <algorithm>
#include <deque>
#include <iostream>
#include <memory>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

// 玩家类前置声明
class 玩家类;

// 中介者接口
class 聊天中介者接口 {
public:
  virtual ~聊天中介者接口() = default;
  virtual void 广播消息(const 玩家类 *发送者, const std::string &消息) = 0;
  virtual void 发送私聊(const 玩家类 *发送者, const std::string &接收者名称,
                        const std::string &消息) = 0;
  virtual void 注册玩家(玩家类 *玩家) = 0;
  virtual void 玩家离开(玩家类 *玩家) = 0;
  virtual void 获取历史消息(玩家类 *玩家) = 0;
};

// 具体玩家类
class 玩家类 {
public:
  玩家类(std::string 名称, 聊天中介者接口 *中介者)
      : 玩家名称(std::move(名称)), 中介者(中介者) {
    中介者->注册玩家(this);
  }

  ~玩家类() { 中介者->玩家离开(this); }

  void 发送广播(const std::string &消息) const { 中介者->广播消息(this, 消息); }

  void 发送私聊(const std::string &接收者名称, const std::string &消息) const {
    中介者->发送私聊(this, 接收者名称, 消息);
  }

  void 请求历史消息() { 中介者->获取历史消息(this); }

  void 接收消息(const std::string &发送者, const std::string &消息,
                bool 是私聊 = false, const std::string &频道 = "") const {
    std::string 前缀 = 是私聊         ? "[私聊] "
                       : 频道.empty() ? ""
                                      : "[" + 频道 + "] ";
    std::cout << 玩家名称 << " << " << 前缀 << 发送者 << ": " << 消息 << "\n";
  }

  std::string 获取名称() const { return 玩家名称; }

private:
  std::string 玩家名称;
  聊天中介者接口 *中介者;
};

// 完整聊天大厅实现
class 聊天大厅 : public 聊天中介者接口 {
public:
  void 广播消息(const 玩家类 *发送者, const std::string &消息) override {
    std::string 过滤消息 = 过滤敏感词(消息);
    std::string 格式化消息 = 格式化处理(过滤消息);

    // 存储历史消息
    历史消息.push_back({发送者->获取名称(), 格式化消息});
    if (历史消息.size() > 100) {
      历史消息.pop_front();
    }

    // 广播给所有玩家
    for (auto &玩家 : 玩家列表) {
      if (玩家 != 发送者) {
        玩家->接收消息(发送者->获取名称(), 格式化消息);
      }
    }
  }

  void 发送私聊(const 玩家类 *发送者, const std::string &接收者名称,
                const std::string &消息) override {
    auto 接收者 =
        std::find_if(玩家列表.begin(), 玩家列表.end(), [&](const 玩家类 *p) {
          return p->获取名称() == 接收者名称;
        });

    if (接收者 != 玩家列表.end()) {
      std::string 过滤消息 = 过滤敏感词(消息);
      (*接收者)->接收消息(发送者->获取名称(), 过滤消息, true);
    } else {
      离线消息存储[接收者名称].push_back({发送者->获取名称(), 消息});
    }
  }

  void 注册玩家(玩家类 *玩家) override {
    玩家列表.push_back(玩家);
    玩家状态[玩家->获取名称()] = "在线";
    std::string 加入消息 = 玩家->获取名称() + " 加入了聊天室";
    历史消息.push_back({"系统", 加入消息});

    // 发送离线消息
    if (离线消息存储.find(玩家->获取名称()) != 离线消息存储.end()) {
      for (auto &[发送者, 消息] : 离线消息存储[玩家->获取名称()]) {
        玩家->接收消息(发送者, 消息, true);
      }
      离线消息存储.erase(玩家->获取名称());
    }
  }

  void 玩家离开(玩家类 *玩家) override {
    auto 位置 = std::find(玩家列表.begin(), 玩家列表.end(), 玩家);
    if (位置 != 玩家列表.end()) {
      玩家列表.erase(位置);
      玩家状态[玩家->获取名称()] = "离线";
      std::string 离开消息 = 玩家->获取名称() + " 离开了聊天室";
      历史消息.push_back({"系统", 离开消息});
    }
  }

  void 获取历史消息(玩家类 *玩家) override {
    for (const auto &[发送者, 消息] : 历史消息) {
      玩家->接收消息(发送者, 消息);
    }
  }

  void 添加敏感词(const std::string &敏感词) { 敏感词列表.push_back(敏感词); }

private:
  std::vector<玩家类 *> 玩家列表;
  std::deque<std::pair<std::string, std::string>> 历史消息;
  std::unordered_map<std::string,
                     std::vector<std::pair<std::string, std::string>>>
      离线消息存储;
  std::unordered_map<std::string, std::string> 玩家状态;
  std::vector<std::string> 敏感词列表;

  std::string 过滤敏感词(const std::string &消息) const {
    std::string 结果 = 消息;
    for (const auto &敏感词 : 敏感词列表) {
      size_t 位置 = 0;
      while ((位置 = 结果.find(敏感词, 位置)) != std::string::npos) {
        结果.replace(位置, 敏感词.length(), "***");
        位置 += 3;
      }
    }
    return 结果;
  }

  std::string 格式化处理(const std::string &消息) const {
    // 简单URL转换
    std::regex 链接正则(R"((https?://\S+))");
    return std::regex_replace(消息, 链接正则, R"(<a href="$1">链接</a>)");
  }
};

int main() {
  auto 大厅 = std::make_unique<聊天大厅>();

  // 添加敏感词
  大厅->添加敏感词("暴力");
  大厅->添加敏感词("色情");

  // 创建玩家
  玩家类 玩家1("小明", 大厅.get());
  玩家类 玩家2("小红", 大厅.get());
  玩家类 玩家3("小刚", 大厅.get());

  // 基本聊天
  玩家1.发送广播("大家好！");
  玩家2.发送广播("欢迎新人！");

  // 私聊测试
  玩家1.发送私聊("小红", "能私下聊吗？");
  玩家2.发送私聊("小明", "当然可以！");

  // 敏感词测试
  玩家3.发送广播("这个游戏没有暴力内容");

  // 历史消息测试
  玩家类 新玩家("小新", 大厅.get());
  新玩家.请求历史消息();

  // 离线消息测试
  玩家类 临时玩家("临时", 大厅.get());
  临时玩家.发送私聊("离线玩家", "这条消息应该被存储");
  {
    玩家类 离线玩家("离线玩家", 大厅.get());
    离线玩家.发送广播("我现在在线");
  } // 离线玩家离开
  临时玩家.发送私聊("离线玩家", "这是离线消息");
  玩家类 返回玩家("离线玩家", 大厅.get()); // 重新加入时接收离线消息

  return 0;
}
