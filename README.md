<p align="left">
<img src="images/xnpp_logo5.png" alt="xnpp logo">
</p>

# 🚀 Xnpp (xn++)

A Modern, Cross-Platform C++ Implementation of the Microsoft XNA 4.0 Framework.

Xnpp is a high-performance game framework that brings the beloved XNA 4.0 API to the modern C++ ecosystem. Built with a focus on cross-platform compatibility and efficiency, it leverages a powerful tech stack: SDL3 for windowing and input, bgfx for agnostic graphics rendering, and miniaudio for robust sound management.

## 📦 Project Structure

The framework is modularized into four core components to ensure flexibility and maintainability:

- `xnpp-csharp`: A foundational compatibility layer. It provides C++ implementations of essential C# classes and logic found in the original XNA 4.0 source, bridging the gap between managed and native code.
- `xnpp-framework`: The heart of the project. A pure C++ library that replicates the XNA 4.0 API surface, allowing developers to use familiar patterns like Game, SpriteBatch, and GraphicsDevice.
- `xnpp-content-pipeline`: A dedicated library containing the logic for processing game assets. It handles the compilation of textures, sound effects, music, and custom data formats into optimized binary files.
- `xnpp-content-cli`: A powerful command-line tool built on top of the content pipeline. It automates the asset build process, making it easy to integrate into CI/CD pipelines or CMake build workflows.

## 🛠 Tech Stack

- Language: Modern C++ (C++20/23)
- Graphics: [bgfx](https://github.com/bkaradzic/bgfx) (DirectX, Vulkan, Metal, OpenGL)
- Platform Layer: [SDL3](https://github.com/libsdl-org/SDL)
- Audio: [miniaudio](https://github.com/mackron/miniaudio)
- Package Management: [vcpkg](https://github.com/microsoft/vcpkg) & CMake

## 💻 Project examples

[https://github.com/borgesdan/XnppProjects](https://github.com/borgesdan/XnppProjects)

## ⚙️ Adjustments and improvements

The project is still under development and the next updates will focus on the following tasks:
- [x] Basic classes
- [x] C# classes
- [x] Content Pipeline
- [x] Cross platform (SDL3/bgfx)
- [x] Project examples
- [ ] 3D support

## 📝 License

This project is under MIT license. See [LICENSE](LICENSE.md) for more details.
