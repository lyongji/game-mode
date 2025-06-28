
target("工厂模式")
  set_kind("binary")
  add_files("./工厂模式.cpp")

target("建造者模式")
  set_kind("binary")
  add_files("./建造者模式.cpp")

target("原型模式")
  set_kind("binary")
  add_files("./原型模式.cpp")

target("单例模式")
  set_kind("binary")
  add_includedirs("./")
  add_files("./单例模式.cpp","./单例示例.cpp")

