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
  auto 名称 = std::string{};
  auto 邮箱 = std::string{};
  auto 密码 = std::string{};

  auto 名称输入 = Input(&名称, "输入名称");
  auto 邮箱输入 = Input(&邮箱, "输入邮箱");
  InputOption 输入选项;
  输入选项.password = true;
  auto 密码输入 = Input(&密码, "输入密码", 输入选项);

  auto 垂直容器 = Container::Vertical({
      名称输入,
      邮箱输入,
      密码输入,
  });

  // 创建一个屏幕，宽度为全屏，高度自适应文档内容
  auto 屏幕 = ScreenInteractive::Fullscreen();

  Component 组件 =
      // 将文档渲染到交互屏幕上，通过renderer 包裹，将元素转换为组件
      Renderer(垂直容器, [&] {
        return vbox({
            text("请" + 名称 + ":" + 邮箱 + " 登录"),
            separatorDouble(),
            hbox(text("名称:"), 名称输入->Render()),
            hbox(text("邮箱:"), 邮箱输入->Render()),
            hbox(text("密码:"), 密码输入->Render()),
        });
      });

  屏幕.Loop(组件); // 循环渲染
}
