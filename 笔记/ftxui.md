# 外部文档
[FTXUI](https://github.com/ArthurSonzogni/FTXUI)
[文档](https://arthursonzogni.github.io/FTXUI/index.html)
[DeepWiki](https://deepwiki.com/ArthurSonzogni/FTXUI)

# 使用

三层架构:

FTXUI 建立在三层架构之上，该架构将关注点分开，并允许用户在不同的抽象级别上工作。

screen：最低层，负责终端渲染和像素作
- 屏幕层是 FTXUI 的基础，负责直接与终端对接。

dom：中间层，定义 UI 元素的结构和外观
- DOM（文档对象模型）层提供了一种定义 UI 结构和外观的声明性方法。

component：最高层，提供具有事件处理的交互式元素
- 组件层为 DOM 元素添加了交互性，允许 UI 响应用户输入。

每个层都建立在其下面的层的功能之上。应用程序代码通常与交互式 UI 的组件层交互，但可以直接使用 dom 或 screen 层进行更简单的用例。
