/*Mokobia Jane Chidima, st135735@student.spbu.ru, 24.B83-mm*/

#include "main.h"

// Function to load BMP image and convert to grayscale
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

    // Move file pointer to pixel data
    file.seekg(bmpHeader.bfOffBits, std::ios::beg);

    // Calculate the row size including padding
    int row_padded = (width * 3 + 3) & (~3); 
    std::vector<unsigned char> image_data(row_padded * height);

    file.read(reinterpret_cast<char*>(image_data.data()), image_data.size());

    unsigned char* grayscale_image = new unsigned char[width * height];

    // Convert to grayscale (assuming BMP is in 24-bit color)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int pixel_index = y * row_padded + x * 3;
            unsigned char r = image_data[pixel_index + 2];
            unsigned char g = image_data[pixel_index + 1];
            unsigned char b = image_data[pixel_index];
            unsigned char gray = static_cast<unsigned char>(0.3 * r + 0.59 * g + 0.11 * b);
            grayscale_image[y * width + x] = gray;
        }
    }

    return grayscale_image;
}

// Function to save BMP image
void save_bmp(const char* file_path, unsigned char* image_data, int width, int height) {
    uint32_t biSizeImage = (width + 3) & (~3); 
    biSizeImage *= height;  // Total image size

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

    // Write grayscale color palette (0-255, one color per index)
    for (int i = 0; i < 256; ++i) {
        file.put(i); file.put(i); file.put(i); file.put(0);
    }

    // Row padding
    int row_padded = (width + 3) & (~3);
    std::vector<unsigned char> padded_row(row_padded);

    // Write pixel data (indexing into the grayscale palette)
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            padded_row[x] = image_data[y * width + x];
        }
        file.write(reinterpret_cast<const char*>(padded_row.data()), row_padded);
    }
}

// Rotate image 90 degrees clockwise
unsigned char* rotate_clockwise(unsigned char* image_data, int width, int height) {
    unsigned char* rotated_image = new unsigned char[width * height];

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            rotated_image[x * height + (height - y - 1)] = image_data[y * width + x];
        }
    }

    return rotated_image;
}

// Rotate image 90 degrees counterclockwise
unsigned char* rotate_counterclockwise(unsigned char* image_data, int width, int height) {
    unsigned char* rotated_image = new unsigned char[width * height];

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            rotated_image[(width - x - 1) * height + y] = image_data[y * width + x];
        }
    }

    return rotated_image;
}

// Apply a simple Gaussian filter (just a placeholder for now)
unsigned char* apply_gaussian_filter(unsigned char* image_data, int width, int height) {
    unsigned char* filtered_image = new unsigned char[width * height];

    // In this simple placeholder, we just copy the original data to the filtered image
    std::copy(image_data, image_data + (width * height), filtered_image);

    return filtered_image;
}

int main() {
    int width, height;
    const char* input_file = "input_image.bmp";
    const char* output_file_clockwise = "output_clockwise.bmp";
    const char* output_file_counterclockwise = "output_counterclockwise.bmp";
    const char* output_file_filtered = "output_filtered.bmp";

    // Load image data
    unsigned char* image_data = load_bmp(input_file, width, height);
    if (!image_data) {
        return -1;
    }

    // Rotate and save images
    unsigned char* rotated_clockwise = rotate_clockwise(image_data, width, height);
    save_bmp(output_file_clockwise, rotated_clockwise, height, width);

    unsigned char* rotated_counterclockwise = rotate_counterclockwise(image_data, width, height);
    save_bmp(output_file_counterclockwise, rotated_counterclockwise, height, width);

    unsigned char* filtered_image = apply_gaussian_filter(rotated_clockwise, height, width);
    save_bmp(output_file_filtered, filtered_image, height, width);

    // Free dynamically allocated memory
    delete[] image_data;
    delete[] rotated_clockwise;
    delete[] rotated_counterclockwise;
    delete[] filtered_image;

    std::cout << "Processing complete." << std::endl;

    return 0;
}
