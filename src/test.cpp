#include <gtest/gtest.h>
#include <fstream>
#include <cstdlib>

// Helper: check if a file exists
bool file_exists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

// Helper: get file size
std::streamsize file_size(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    return file.is_open() ? static_cast<std::streamsize>(file.tellg()) : std::streamsize(-1);
}

const std::string input = "images/input_image.bmp";


TEST(FullPipelineTest, GaussianFilterOutput) {
    const std::string output = "images/output_filtered.bmp";
    ASSERT_TRUE(file_exists(input)) << "Input file not found.";

    std::string cmd = "./lab " + input + " " + output + " filter";
    ASSERT_EQ(std::system(cmd.c_str()), 0) << "Filter command failed.";

    ASSERT_TRUE(file_exists(output));
    ASSERT_GT(file_size(output), 0);
}

TEST(FullPipelineTest, RotateClockwiseOutput) {
    const std::string output = "images/output_clockwise.bmp";
    ASSERT_TRUE(file_exists(input)) << "Input file not found.";

    std::string cmd = "./lab " + input + " " + output + " rotate_cw";
    ASSERT_EQ(std::system(cmd.c_str()), 0) << "Clockwise rotation failed.";

    ASSERT_TRUE(file_exists(output));
    ASSERT_GT(file_size(output), 0);
}

TEST(FullPipelineTest, RotateCounterclockwiseOutput) {
    const std::string output = "images/output_counterclockwise.bmp";
    ASSERT_TRUE(file_exists(input)) << "Input file not found.";

    std::string cmd = "./lab " + input + " " + output + " rotate_ccw";
    ASSERT_EQ(std::system(cmd.c_str()), 0) << "Counterclockwise rotation failed.";

    ASSERT_TRUE(file_exists(output));
    ASSERT_GT(file_size(output), 0);
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}