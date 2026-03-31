# 🚀 Modern C++ Interpreter (Smart & Lightweight)

A high-performance, memory-safe interpreter engine built with **Modern C++17**. This project focuses on leveraging **Smart Pointers** for automatic memory management and **Modern Design Patterns** for language processing.

## 🌟 Why this Interpreter?
- **Smart Memory Management:** Uses `std::unique_ptr` throughout the AST to ensure zero memory leaks without the overhead of a Garbage Collector.
- **Modern Syntax Handling:** Built with `std::variant` and `std::visit` for robust runtime value handling (Numbers, Strings, etc.).
- **Lightweight Architecture:** Minimalistic design, making it easy to embed or extend.
- **Clear Syntax:** Designed for readability and future-proof language features.

## 🏗 Technical Architecture (AST Nodes)
The parser transforms source code into a tree of specialized nodes:
- **BinaryExpr:** Handles arithmetic with operator precedence.
- **VariableAssignment:** Efficiently manages scope and state using an Environment system.
- **StringNode:** Built-in support for escape sequences (`\n`, `\t`).

## 🛠 Prerequisites & Build
- **Compiler:** GCC 7+ / Clang 5+ (C++17 Support)
- **Build System:** CMake 3.10+

```bash
mkdir build && cd build
cmake ..
cmake --build .
## 📅 Bala Language Roadmap 2026

| Phase | Feature | Expected Date | Status |
| :--- | :--- | :--- | :--- |
| **01** | **Relational & Logical Operators** | 2026-04-17 | ⏳ Planning |
| **02** | **Standard Input** | 2026-05-18 | ⏳ Planning |
| **03** | **Conditional Statements (If)** | 2026-06-18 | ⏳ Planning |
| **04** | **For Loop (Iteration)** | 2026-07-18 | ⏳ Planning |

---

> **Note for Contributors:** We follow a monthly schedule for adding core features. If you would like to contribute, please open an Issue or contact us directly.
