#include <memory>
#include <print>

// 抽象主题
class 图像 {
public:
  virtual void 显示() = 0;
  virtual ~图像() = default;
};

// 真实主题
class 高分辨率图像 : public 图像 {
  std::string 文件名;

public:
  高分辨率图像(const std::string &名称) : 文件名(名称) {
    从磁盘加载(); // 模拟高开销操作
  }

  void 显示() override { std::println("显示高分辨率图像: {}", 文件名); }

private:
  void 从磁盘加载() { std::println("从磁盘加载图像: {} (高开销操作)", 文件名); }
};

// 代理
class 图像代理 : public 图像 {
  std::string 文件名;
  std::unique_ptr<高分辨率图像> 真实图像;

public:
  图像代理(const std::string &名称) : 文件名(名称) {}

  void 显示() override {
    if (!真实图像) {
      真实图像 = std::make_unique<高分辨率图像>(文件名);
    }
    真实图像->显示();
  }
};

int main() {
  std::println("===== 虚拟代理示例 =====");

  // 创建代理对象（此时不加载图像）
  图像代理 代理("假期照片.jpg");
  std::println("代理已创建，图像尚未加载");

  // 第一次访问时才加载图像
  std::println("\n第一次显示图像:");
  代理.显示();

  // 后续访问直接使用已加载的图像
  std::println("\n第二次显示图像:");
  代理.显示();
}
