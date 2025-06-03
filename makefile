# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -O2

# Directories
SRC_DIR := src
INC_DIR := include
IMG_DIR := images
BIN := lab

# Sources and objects
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:.cpp=.o)

# Default target
all: $(BIN)

# Build executable
$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Pattern rule for object files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/main.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(SRC_DIR)/*.o $(BIN)
	rm -f $(IMG_DIR)/output_*.bmp

# Run the program
run: $(BIN)
	./$(BIN)


	.PHONY: all clean run
docs:
	doxygen Doxyfile	