# Makefile Masterpiece

# --- Compilador y flags ---
CXX := ccache g++
CXXFLAGS := -Wall -std=c++17
CXXFLAGS += -I./src/core -I./src/states -I./vendor/include

# --- Librerías externas (Raylib) ---
LDFLAGS := -L./vendor/lib -lraylib -lm -ldl -lpthread -lGL

# --- Estructura de carpetas ---
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
TARGET := $(BIN_DIR)/masterpiece

# --- Buscar recursivamente todos los .cpp ---
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)


### REGLAS PRINCIPALES ###

# Compilación por defecto
all: $(TARGET)

# Enlace final
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "Enlazando $(TARGET)..."
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compilación de cada archivo fuente
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compilando $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar compilación
clean:
	@echo "Limpiando..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)
