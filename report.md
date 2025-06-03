/**
 * @mainpage BMP Image Processing with Parallelism
 *
 # summary of the time taken in my project
 *
 * Grayscale conversion took 27 ms
 *
 * Memory allocated for loading the image: 2457600 bytes (2400 KB)
 *
 * Clockwise rotation took 23 ms
 *
 * Counterclockwise rotation took 26 ms
 *
 * Gaussian filtering took 282 ms
 *
 * Debug pixel value (before filter): 188, after filter: 187
 *
 */
# 📘 BMP Image Processor Report

**Author:** Mokobia Jane Chidima  
**Email:** st135735@student.spbu.ru  
**Group:** 24.B83-mm  
**Project:** Parallel BMP Image Processing in C++

---

## 📑 Table of Contents

- [1. Introduction](#1-introduction)
- [2. Project Structure](#2-project-structure)
- [3. Features Implemented](#3-features-implemented)
- [4. Parallelism](#4-parallelism)
- [5. Memory and Performance](#5-memory-and-performance)
- [6. Code Documentation](#6-code-documentation)
- [7. Doxygen Setup](#7-doxygen-setup)
- [8. Conclusion](#8-conclusion)

---

## 1. Introduction

This project performs BMP image processing using parallel computing in C++. It handles:

- Grayscale conversion
- Image rotation (clockwise and counterclockwise)
- Gaussian blur filtering

---

## 2. Project Structure

```
project-root/
├── include/
│   └── main.h
├── src/
│   └── main.cpp
├── images/
│   └── input_image.bmp
├── docs/
│   └── report.md
├── Makefile
├── Doxyfile
└── .github/workflows/ci.yml
```

---

## 3. Features Implemented

- ✅ **Grayscale conversion** from 24-bit RGB to 8-bit grayscale
- ✅ **Clockwise rotation** (90°)
- ✅ **Counterclockwise rotation** (90°)
- ✅ **5x5 Gaussian blur** smoothing filter
- ✅ **Multithreaded execution** using `std::thread`
- ✅ **BMP file I/O support**

---

## 4. Parallelism

A custom `parallel_for` template distributes work across multiple CPU threads for the following operations:

- Grayscale conversion
- Rotation (clockwise & counterclockwise)
- Gaussian filtering

Threads are automatically scaled based on `std::thread::hardware_concurrency()`.

---

## 5. Memory and Performance

Memory usage is estimated for image buffers. Timings for each step are measured using `std::chrono`. Example output:

```text
Grayscale conversion took 27 ms
Clockwise rotation took 23 ms
Gaussian filtering took 31 ms
```

---

## 6. Code Documentation

Each function is documented using [Doxygen](https://www.doxygen.nl/manual/docblocks.html) style:

```cpp
/**
 * @brief Converts a BMP image to grayscale.
 * @param file_path Path to input BMP.
 * @param width Reference to output width.
 * @param height Reference to output height.
 * @return Pointer to grayscale image buffer.
 */
unsigned char* load_bmp(const char* file_path, int& width, int& height);
```

To generate documentation:

```bash
doxygen docs/Doxyfile
xdg-open docs/html/index.html
```

It will generate output in `docs/html/index.html`.

---

## 7. Doxygen Setup

The `Doxyfile` has the following key settings:

```ini
PROJECT_NAME = "Parallel BMP Processor"
INPUT = src include report.md
GENERATE_LATEX = NO
RECURSIVE = YES
```

---

## 8. Conclusion

This project showcases efficient image processing in C++ with multithreading and clean documentation. It also serves as a template for further GPU-based or SIMD-optimized processing in the future.

---

🔗 **Related Files:**

- [README.md](./README.md)
- [main.cpp](./src/main.cpp)
- [main.h](./include/main.h)
- [CI Workflow](./.github/workflows/ci.yml)
