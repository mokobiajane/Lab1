# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -O2

# Directories
SRC_DIR := src
INC_DIR := include
IMG_DIR := images

# Output binaries
BIN := lab
TEST_BIN := test

# Source files
MAIN_SRC := $(SRC_DIR)/main.cpp
TEST_SRC := $(SRC_DIR)/test.cpp

# Object files
MAIN_OBJ := $(MAIN_SRC:.cpp=.o)

# Default target
all: $(BIN)

# Build main executable
$(BIN): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile main object
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/main.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build and run tests
run-tests: $(BIN) $(TEST_BIN)
	./$(TEST_BIN)


$(TEST_BIN): $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $< -o $@ -lgtest -lgtest_main -pthread

# Clean build files
clean:
	rm -f $(SRC_DIR)/*.o $(BIN) $(TEST_BIN)
	rm -f $(IMG_DIR)/output_*.bmp

# Generate docs
docs:
	doxygen Doxyfile

.PHONY: all clean run-tests docs 