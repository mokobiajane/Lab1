/* Mokobia Jane Chidima, st135735@student.spbu.ru, 24.B83-mm */
/* 
 * @file main.cpp
 * @author Mokobia Jane Chidima
 * @date 2025-06-02
 * @brief BMP Image Processing: grayscale, rotation, and Gaussian filtering with parallelism.
 */

#include "main.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
#include <algorithm>

using namespace std::chrono;

/**
 * @brief Launches multiple threads to apply a function in parallel over a range.
 * 
 * @tparam Func Callable type for the work to be done.
 * @param start Start index.
 * @param end End index.
 * @param num_threads Number of threads to use.
 * @param func Function to apply (should take start and end).
 */

// Divide work for threads
template <typename Func>
void parallel_for(int start, int end, int num_threads, Func func) {
    std::vector<std::thread> threads;
    int block_size = (end - start) / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        int block_start = start + i * block_size;
        int block_end = (i == num_threads - 1) ? end : block_start + block_size;
        threads.emplace_back(func, block_start, block_end);
    }

    for (auto& t : threads) t.join();
}

/**
 * @brief Loads a BMP file, converts it to grayscale, and returns the image buffer.
 * 
 * @param file_path Path to the BMP file.
 * @param width Reference to store image width.
 * @param height Reference to store image height.
 * @return unsigned char* Pointer to grayscale image data (heap-allocated).
 */

unsigned char* load_bmp(const char* file_path, int& width, int& height) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return nullptr;
    }

    BITMAPFILEHEADER bmpHeader;
    BITMAPINFOHEADER dibHeader;

    file.read(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));
    file.read(reinterpret_cast<char*>(&dibHeader), sizeof(dibHeader));

    if (bmpHeader.bfType != 0x4D42) {
        std::cerr << "Not a valid BMP file." << std::endl;
        return nullptr;
    }

    width = dibHeader.biWidth;
    height = std::abs(dibHeader.biHeight);

    int row_padded = (width * 3 + 3) & (~3);
    std::vector<unsigned char> image_data(row_padded * height);
    file.seekg(bmpHeader.bfOffBits, std::ios::beg);
    file.read(reinterpret_cast<char*>(image_data.data()), image_data.size());

    unsigned char* grayscale_image = new unsigned char[width * height];
    int num_threads = std::thread::hardware_concurrency();

    parallel_for(0, height, num_threads, [&](int y_start, int y_end) {
        for (int y = y_start; y < y_end; ++y) {
            for (int x = 0; x < width; ++x) {
                int pixel_index = y * row_padded + x * 3;
                unsigned char r = image_data[pixel_index + 2];
                unsigned char g = image_data[pixel_index + 1];
                unsigned char b = image_data[pixel_index];
                unsigned char gray = static_cast<unsigned char>(0.3 * r + 0.59 * g + 0.11 * b);
                grayscale_image[y * width + x] = gray;
            }
        }
    });

    return grayscale_image;
}

/**
 * @brief Saves an 8-bit grayscale image buffer to a BMP file.
 * 
 * @param file_path Output BMP file path.
 * @param image_data Pointer to grayscale image data.
 * @param width Image width.
 * @param height Image height.
 */

void save_bmp(const char* file_path, unsigned char* image_data, int width, int height) {
    int row_padded = (width + 3) & (~3);
    uint32_t biSizeImage = row_padded * height;

    BITMAPINFOHEADER dibHeader = { 40, width, height, 1, 8, 0, biSizeImage, 0, 0, 256, 0 };
    BITMAPFILEHEADER bmpHeader = { 0x4D42, 0, 0, 0, 54 + 256 * 4 };
    bmpHeader.bfSize = bmpHeader.bfOffBits + dibHeader.biSizeImage;

    std::ofstream file(file_path, std::ios::binary);
    if (!file) {
        std::cerr << "Error saving image: " << file_path << std::endl;
        return;
    }

    file.write(reinterpret_cast<const char*>(&bmpHeader), sizeof(bmpHeader));
    file.write(reinterpret_cast<const char*>(&dibHeader), sizeof(dibHeader));

    for (int i = 0; i < 256; ++i) {
        file.put(i); file.put(i); file.put(i); file.put(0);
    }

    std::vector<unsigned char> padded_row(row_padded);

    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            padded_row[x] = image_data[y * width + x];
        }
        for (int x = width; x < row_padded; ++x) {
            padded_row[x] = 0;
        }
        file.write(reinterpret_cast<const char*>(padded_row.data()), row_padded);
    }
}

/**
 * @brief Calculates the memory usage in bytes for an image buffer.
 * 
 * @param width Image width.
 * @param height Image height.
 * @return size_t Memory used in bytes.
 */

size_t calculate_memory_usage(int width, int height) {
    return static_cast<size_t>(width * height);
}

unsigned char* rotate_clockwise(unsigned char* image_data, int width, int height) {
    unsigned char* rotated_image = new unsigned char[width * height];
    int num_threads = std::thread::hardware_concurrency();

    parallel_for(0, height, num_threads, [&](int y_start, int y_end) {
        for (int y = y_start; y < y_end; ++y) {
            for (int x = 0; x < width; ++x) {rotated_image[x * height + (height - y - 1)] = image_data[y * width + x];
            }
        }
    });

    return rotated_image;
}
/**
 * @brief Rotates the grayscale image 90 degrees counterclockwise.
 * 
 * @param image_data Pointer to original grayscale image.
 * @param width Image width.
 * @param height Image height.
 * @return unsigned char* Rotated image buffer (heap-allocated).
 */

unsigned char* rotate_counterclockwise(unsigned char* image_data, int width, int height) {
    unsigned char* rotated_image = new unsigned char[width * height];
    int num_threads = std::thread::hardware_concurrency();

    parallel_for(0, height, num_threads, [&](int y_start, int y_end) {
        for (int y = y_start; y < y_end; ++y) {
            for (int x = 0; x < width; ++x) {
                rotated_image[(width - x - 1) * height + y] = image_data[y * width + x];
            }
        }
    });

    return rotated_image;
}

/**
 * @brief Applies a 5x5 Gaussian blur filter to the grayscale image.
 * 
 * @param image_data Pointer to grayscale image.
 * @param width Image width.
 * @param height Image height.
 * @return unsigned char* Filtered image (heap-allocated).
 */

unsigned char* apply_gaussian_filter(unsigned char* image_data, int width, int height) {
    unsigned char* filtered_image = new unsigned char[width * height];
    float kernel[5][5] = {
        {1 / 273.0f, 4 / 273.0f, 7 / 273.0f, 4 / 273.0f, 1 / 273.0f},
        {4 / 273.0f,16 / 273.0f,26 / 273.0f,16 / 273.0f, 4 / 273.0f},
        {7 / 273.0f,26 / 273.0f,41 / 273.0f,26 / 273.0f, 7 / 273.0f},
        {4 / 273.0f,16 / 273.0f,26 / 273.0f,16 / 273.0f, 4 / 273.0f},
        {1 / 273.0f, 4 / 273.0f, 7 / 273.0f, 4 / 273.0f, 1 / 273.0f}
    };

    int num_threads = std::thread::hardware_concurrency();

    parallel_for(2, height - 2, num_threads, [&](int y_start, int y_end) {
        for (int y = y_start; y < y_end; ++y) {
            for (int x = 2; x < width - 2; ++x) {
                float sum = 0.0f;
                for (int ky = -2; ky <= 2; ++ky)
                    for (int kx = -2; kx <= 2; ++kx)
                        sum += image_data[(y + ky) * width + (x + kx)] * kernel[ky + 2][kx + 2];
                filtered_image[y * width + x] = static_cast<unsigned char>(sum);
            }
        }
    });

    // Handle edges (non-parallel)
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            if (y < 2 || x < 2 || y >= height - 2 || x >= width - 2)
                filtered_image[y * width + x] = image_data[y * width + x];

    return filtered_image;
}

/**
 * @brief Entry point of the program.
 * 
 * Loads an image, applies grayscale conversion, rotations, and filtering.
 * Saves results to output BMP files and reports timing and memory usage.
 * 
 * @return int 0 on success, -1 on failure.
 */

int main() {
    int width, height;
    const char* input_file = "images/input_image.bmp";
    const char* output_file_clockwise = "images/output_clockwise.bmp";
    const char* output_file_counterclockwise = "images/output_counterclockwise.bmp";
    const char* output_file_filtered = "images/output_filtered.bmp";
    
    auto start = high_resolution_clock::now();
    unsigned char* image_data = load_bmp(input_file, width, height);
    if (!image_data) return -1;
    auto end = high_resolution_clock::now();
    std::cout << "Grayscale conversion took "
              << duration_cast<milliseconds>(end - start).count() << " ms\n";

    size_t memory_allocated = calculate_memory_usage(width, height);
    std::cout << "Memory allocated for loading the image: "
              << memory_allocated << " bytes ("
              << (memory_allocated / 1024.0) << " KB)" << std::endl;

    start = high_resolution_clock::now();
    unsigned char* rotated_clockwise = rotate_clockwise(image_data, width, height);
    end = high_resolution_clock::now();
    std::cout << "Clockwise rotation took "
              << duration_cast<milliseconds>(end - start).count() << " ms\n";

    save_bmp(output_file_clockwise, rotated_clockwise, height, width);

    start = high_resolution_clock::now();
    unsigned char* rotated_counterclockwise = rotate_counterclockwise(image_data, width, height);
    end = high_resolution_clock::now();
    std::cout << "Counterclockwise rotation took "
              << duration_cast<milliseconds>(end - start).count() << " ms\n";

    save_bmp(output_file_counterclockwise, rotated_counterclockwise, height, width);

    start = high_resolution_clock::now();
    unsigned char* filtered_image = apply_gaussian_filter(rotated_clockwise, height, width);
    end = high_resolution_clock::now();
    std::cout << "Gaussian filtering took "
              << duration_cast<milliseconds>(end - start).count() << " ms\n";

    save_bmp(output_file_filtered, filtered_image, height, width);

    // Debug pixel
    int debug_y = std::min(100, width - 1);
    int debug_x = std::min(100, height - 1);
    std::cout << "Debug pixel value (before filter): "
              << static_cast<int>(rotated_clockwise[debug_y * height + debug_x])
              << ", after filter: "
              << static_cast<int>(filtered_image[debug_y * height + debug_x]) << std::endl;

    // Cleanup
    delete[] image_data;
    delete[] rotated_clockwise;
    delete[] rotated_counterclockwise;
    delete[] filtered_image;

    std::cout << "Processing complete." << std::endl;
    return 0;
}