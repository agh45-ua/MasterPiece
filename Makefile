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
CORE_OBJS := $(BUILD_DIR)/core/GameState.o $(BUILD_DIR)/core/StateMachine.o
STATE_OBJS := $(BUILD_DIR)/states/InicioState.o $(BUILD_DIR)/states/PersonajesState.o \
              $(BUILD_DIR)/states/GameOverState.o $(BUILD_DIR)/states/MainGameState.o
MAIN_OBJ := $(BUILD_DIR)/core/main.o

OBJS := $(CORE_OBJS) $(STATE_OBJS) $(MAIN_OBJ)

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

# Comprobación tests
test:
	@echo "===> Preparando entorno de pruebas..."
	@mkdir -p $(BUILD_DIR)
	@echo "===> Configurando CMake..."
	@cd $(BUILD_DIR) && cmake ..
	@echo "===> Compilando ejecutables de test..."
	@cd $(BUILD_DIR) && make test_statemachine test_states test_gameplay
	@echo "===> Ejecutando CTest..."
	@cd $(BUILD_DIR) && ctest --output-on-failure
	
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