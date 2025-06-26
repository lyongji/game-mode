#include "ftxui/component/component.hpp" // 动态交互组件
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp" // 交互屏幕
#include "ftxui/screen/color.hpp"
#include <ftxui/dom/elements.hpp>  // 引入FTXUI的DOM元素库
#include <ftxui/screen/screen.hpp> // 引入FTXUI的屏幕库
#include <string>
int main() {
  using namespace ftxui; // 使用FTXUI命名空间

  // 创建一个简单的文档，包含三个文本元素
  int 数字 = 0;
  auto 按钮 =
      Button("点击", [&] { 数字++; }, ButtonOption::Animated(Color::Red));

  // 创建一个屏幕，宽度为全屏，高度自适应文档内容
  auto 屏幕 = ScreenInteractive::Fullscreen();

  Component 组件 =
      // 将文档渲染到交互屏幕上，通过renderer 包裹，将元素转换为组件
      Renderer(按钮, [&] {
        return vbox(text("数字=" + std::to_string(数字)), separator(),
                    按钮->Render());
      });

  屏幕.Loop(组件); // 循环渲染
}
