
add_requires("flecs")

target("ecs例子")
  set_kind("binary")
  add_files("./flecs.cpp")
  add_packages("flecs" )
