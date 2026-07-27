# 风灵月影修改器

这是一个新的 Windows 游戏修改器项目骨架，当前目标是先搭好可扩展源码结构，后续再逐步加入真实游戏适配、热键、配置文件和图形界面。

## 当前内容

- C++20 + CMake 项目结构
- 命令行入口
- 修改项 `Cheat` 抽象
- 修改项管理器 `Trainer`
- 目标进程查找与附加接口
- 安全内存读写封装
- 配置加载占位
- Windows 平台接口预留

## 目录结构

```text
.
├── CMakeLists.txt
├── README.md
├── docs/
│   ├── development-plan.md
│   └── safety-rules.md
└── src/
    ├── main.cpp
    ├── core/
    │   ├── cheat.cpp
    │   ├── cheat.hpp
    │   ├── config.cpp
    │   ├── config.hpp
    │   ├── memory.cpp
    │   ├── memory.hpp
    │   ├── trainer.cpp
    │   └── trainer.hpp
    └── platform/
        ├── process.cpp
        └── process.hpp
```

## 构建方式

需要安装 CMake 和支持 C++20 的编译器。Windows 推荐 Visual Studio 2022。

```bash
cmake -S . -B build
cmake --build build
```

也可以在当前开发环境中用 `g++` 做快速语法验证：

```bash
g++ -std=c++20 -Isrc \
  src/main.cpp src/core/cheat.cpp src/core/config.cpp src/core/memory.cpp \
  src/core/trainer.cpp src/platform/process.cpp \
  -o /tmp/fengling_trainer_test
```

## 运行方式

```bash
fengling_trainer --list
fengling_trainer --target Game.exe --enable infinite_health
```

## 下一步

1. 确定第一个支持的游戏和进程名。
2. 为第一个游戏建立离线测试地址表。
3. 增加热键系统。
4. 增加 JSON 配置文件。
5. 再决定做命令行版、Win32 原生界面、Qt 界面或 ImGui 界面。
