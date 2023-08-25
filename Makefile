# Defina o compilador e as opções de compilação
CXX = g++
CXXFLAGS = -Wall -Wextra -Iinclude -I/usr/include/SDL2

# Diretórios
SRC_DIR = src
OBJ_DIR = obj

# Lista de arquivos fonte
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Gere nomes de arquivos objeto correspondentes
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Nome do executável
TARGET = program.out

# Flags do linker
LDFLAGS = -lSDL2

# Flags de debug
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