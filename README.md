## 简介

MoliMoli 是一个使用 C++23, [nlohmann/json](https://github.com/nlohmann/json), [ArthurSonzogni/FTXUI](https://github.com/ArthurSonzogni/FTXUI) 实现的型项目。主要功能为创建零钱罐并往里面存钱，纯属娱乐；当然它十分轻量。

## 特性

- 很有趣，不是吗？
- 支持**Windows, Linux**
- 理论上也支持**macOS**

## 环境要求

- CMake >= 4.3
- 支持 C++23 的编译器
- *理论上更低版本也能构建，但作者目前没测试过*

## 项目结构

.
├── include
│   ├── add.h
│   ├── dataManage.h
│   └── userInterface.h
├── src
│   ├── add.cpp
│   ├── dataManage.cpp
│   ├── main.cpp
│   └── userInterface.cpp
├── CMakeLists.txt
└── README.md