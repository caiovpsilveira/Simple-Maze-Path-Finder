# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -Iinclude -I/usr/include/SDL2

# Directories
SRC_DIR = src
OBJ_DIR = obj

# Source files list
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files name
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Binary name
TARGET = program.out

# Linker flags
LDFLAGS = -lSDL2

# Debug flags (make debug)
DEBUG_CXXFLAGS = -DDEBUG

all: $(TARGET)

debug: CXXFLAGS += $(DEBUG_CXXFLAGS)
debug: $(TARGET)

$(TARGET): $(OBJS) $(OBJ_DIR)/main.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/main.o: main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean