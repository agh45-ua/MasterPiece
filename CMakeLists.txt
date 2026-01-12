cmake_minimum_required(VERSION 3.15)
project(MasterPiece LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W3 /D_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 1. BUSCAR DEPENDENCIAS (Raylib)
# ------------------------------------------
find_package(raylib CONFIG REQUIRED)

# 2. DEFINIR FUENTES Y CREAR LIBRERÍA (Target GameLogic)
# ------------------------------------------
# Listado explícito para evitar GLOB (Práctica recomendada por el profesor)
set(GAME_SOURCES
    src/core/GameState. cpp
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

# Enlazamos raylib a GameLogic (esto proporciona automáticamente los directorios de inclusión)
target_link_libraries(GameLogic PUBLIC raylib)

# ...y DESPUÉS le asignamos sus directorios de inclusión
target_include_directories(GameLogic PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/states
)

# 3. Librerias del sistema
if (WIN32)
    # Librerías Multimedia de Windows
    target_link_libraries(GameLogic PUBLIC winmm gdi32 shell32 user32 opengl32)
elseif (UNIX AND NOT APPLE)
    # Librerías de Linux (X11, GL, etc.)
    target_link_libraries(GameLogic PUBLIC m dl pthread GL X11 Xrandr Xi Xcursor Xxf86vm)
endif()

# 4. CREAR EJECUTABLE PRINCIPAL (Target MasterPiece)
# ------------------------------------------
# El ejecutable depende de main. cpp
add_executable(MasterPiece src/core/main.cpp)

# Enlazamos a la lógica (que ya incluye raylib).
# NOTA: MasterPiece hereda los INCLUDES de GameLogic automáticamente por ser PUBLIC.
target_link_libraries(MasterPiece PRIVATE GameLogic)

# 5. RECURSOS, TESTS Y EMPAQUETADO
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
