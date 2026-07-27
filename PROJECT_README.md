# 风灵月影修改器

这是一个新的 Windows 游戏修改器项目骨架，当前目标是先搭好可扩展源码结构，后续再逐步加入进程附加、内存读写、热键、配置文件和图形界面。

## 当前内容

- C++20 + CMake 项目结构
- 命令行入口
- 修改项 `Cheat` 抽象
- 目标进程查找接口
- 配置加载占位
- Windows 平台接口预留

## 目录结构

```text
.
├── CMakeLists.txt
├── PROJECT_README.md
├── docs/
│   ├── development-plan.md
│   └── safety-rules.md
└── src/
    ├── main.cpp
    ├── core/
    │   ├── cheat.cpp
    │   ├── cheat.hpp
    │   ├── config.cpp
    │   └── config.hpp
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

## 下一步

1. 确定第一个支持的游戏和进程名。
2. 增加 Windows 进程附加与权限检查。
3. 增加安全的内存读写封装。
4. 增加热键系统。
5. 增加配置文件和修改项列表。
6. 再决定做命令行版、Win32 原生界面、Qt 界面或 ImGui 界面。
