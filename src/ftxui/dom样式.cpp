#include <ftxui/dom/elements.hpp>  // 引入FTXUI的DOM元素库
#include <ftxui/screen/screen.hpp> // 引入FTXUI的屏幕库

int main() {
  using namespace ftxui; // 使用FTXUI命名空间

  // 创建一个简单的文档，包含三个文本元素 Element
  Element 文档 =
      vbox(hbox({
               // 水平排列的盒子
               // 通过 | 管道符 来修饰文本样式
               text("左") | border,        // 左侧带边框的文本
               text("中") | border | flex, // 中间带边框且可伸缩的文本
               text("右") | border,        // 右侧带边框的文本
           }),
           separator(),
           hbox({
               text("标准"),
               text("粗体") | bold,
               text("斜体") | italic,
               text("暗淡") | dim,
               text("反转") | inverted,
               text("下划线") | underlined,
               text("文字色") | color(Color::Blue),
               text("背景色") | bgcolor(Color::Blue),
           }));

  // 创建一个屏幕，宽度为全屏，高度自适应文档内容
  auto 屏幕 = Screen::Create(Dimension::Full(),   // 宽度：填满整个屏幕
                             Dimension::Fit(文档) // 高度：根据文档内容自动调整
  );

  // 将文档渲染到屏幕上
  Render(屏幕, 文档);

  // 将屏幕内容打印到控制台
  屏幕.Print();
}
