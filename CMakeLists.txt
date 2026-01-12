cmake_minimum_required(VERSION 3.15)
project(MasterPiece LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 1. BUSCAR DEPENDENCIAS (Raylib)
# ------------------------------------------
find_package(raylib QUIET)

if (NOT raylib_FOUND)
    # Buscamos la librería específica en vendor/ para no usar rutas globales
    find_library(RAYLIB_LIB raylib PATHS ${CMAKE_SOURCE_DIR}/vendor/lib NO_DEFAULT_PATH)
else()
    set(RAYLIB_LIB raylib)
endif()

# 2. DEFINIR FUENTES Y CREAR LIBRERÍA (Target GameLogic)
# ------------------------------------------
# Listado explícito para evitar GLOB (Práctica recomendada por el profesor)
set(GAME_SOURCES
    src/core/GameState.cpp
    src/core/StateMachine.cpp
    src/core/ControlBindings.cpp
    src/core/ResourceManager.cpp

    src/states/InicioState.cpp
    src/states/PersonajesState.cpp
    src/states/GameOverState.cpp
    src/states/MainGameState.cpp
    src/states/ControlesState.cpp
)

# Primero creamos la librería...
add_library(GameLogic STATIC ${GAME_SOURCES})

# ...y DESPUÉS le asignamos sus directorios de inclusión
target_include_directories(GameLogic PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/states
    ${CMAKE_SOURCE_DIR}/vendor/include
)

# 3. CREAR EJECUTABLE PRINCIPAL (Target MasterPiece)
# ------------------------------------------
# El ejecutable depende de main.cpp
add_executable(MasterPiece src/core/main.cpp)

# Enlazamos a la lógica y a raylib. 
# NOTA: MasterPiece hereda los INCLUDES de GameLogic automáticamente por ser PUBLIC.
target_link_libraries(MasterPiece PRIVATE GameLogic ${RAYLIB_LIB})

# Librerías adicionales para Linux
if (UNIX AND NOT APPLE)
    target_link_libraries(MasterPiece PRIVATE m dl pthread GL X11 Xrandr Xi Xcursor Xxf86vm)
endif()

# 4. RECURSOS, TESTS Y EMPAQUETADO
# ------------------------------------------
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

enable_testing()
add_subdirectory(tests)

if (WIN32)
    set(CPACK_PACKAGE_NAME "MasterPiece")
    set(CPACK_PACKAGE_VERSION "1.0.0")
    set(CPACK_GENERATOR "ZIP")
    include(CPack)
    cpack_add_component(MasterPiece)
endif()