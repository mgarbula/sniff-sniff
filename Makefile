TARGET := sniff
SRC_DIR := src
INC_DIR := net_structs
OBJ_DIR := build

CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -MMD -MP
# -MMD: Generate dependency files (.d) alongside object files. CRITICAL for header tracking.
# -MP: Add a dummy target for each header file in the .d file (prevents errors if a header is deleted)

# Find all .cpp source files recursively in the source directory
# The 'wildcard' function finds files matching the pattern.
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)

# Convert the list of source files (.cpp) into a list of object files (.o)
# The 'patsubst' function substitutes the file extension and changes the directory
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Create a list of dependency files (.d) corresponding to the object files
DEPS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.d, $(SOURCES))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(OBJECTS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	# The -I flag tells the compiler where to look for header files.
	# This automatically includes ALL headers in the $(INC_DIR)
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJ_DIR)

# Include all the generated dependency files (created by the -MMD flag).
# If the .d files exist, they will be read by make, effectively creating rules
# like: build/main.o: src/main.cpp include/my_header.h
-include $(DEPS)