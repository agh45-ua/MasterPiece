# Makefile Masterpiece

# --- Compilador y flags ---
CXX := g++
CXXFLAGS := -Wall -std=c++17
CXXFLAGS += -I./src/core -I./src/states -I./vendor/include

# --- Librerías externas (Raylib) ---
LDFLAGS := -L./vendor/lib -lraylib -lm -ldl -lpthread -lGL -lX11 -lXrandr -lXi -lXcursor -lXxf86vm

# --- Estructura de carpetas ---
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
ASSETS_DIR := assets
TARGET := $(BIN_DIR)/masterpiece

# --- Buscar recursivamente todos los .cpp ---
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# --- Variables para instalación / empaquetado ---
APP_NAME := masterpiece
PREFIX ?= /usr
DESTDIR ?=
BINDIR := $(PREFIX)/bin
LIBDIR := $(PREFIX)/lib/$(APP_NAME)
DATADIR := $(PREFIX)/share/$(APP_NAME)

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

# Instalación según estructura requerida
install: $(TARGET)
	@echo "Instalando $(APP_NAME)..."
	install -d $(DESTDIR)$(BINDIR)
	install -d $(DESTDIR)$(LIBDIR)
	install -m 0755 $(TARGET) $(DESTDIR)$(BINDIR)/$(APP_NAME)
	install -d $(DESTDIR)$(DATADIR)/assets
	@if [ -d $(ASSETS_DIR) ]; then cp -r $(ASSETS_DIR)/* $(DESTDIR)$(DATADIR)/assets/; fi
	@echo "Instalación completada."

# Limpiar compilación
clean:
	@echo "Limpiando..."
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@rm -rf debian/$(APP_NAME) debian/*.deb ../$(APP_NAME)_*.deb || true

# Crear paquete .deb
dist: clean
	@echo "Generando paquete .deb..."
	dpkg-buildpackage -us -uc -b
	@echo "Paquete .deb creado en directorio padre."

.PHONY: all install clean dist