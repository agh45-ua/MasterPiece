cmake_minimum_required(VERSION 3.15)
project(MasterPiece LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ==========================================
# BUSCAR RAYLIB DEL SISTEMA
# ==========================================
find_package(raylib QUIET)

# Si no existe raylib instalado, usar vendor/
if (NOT raylib_FOUND)
    message(STATUS "Raylib no encontrado, usando vendor/include y vendor/lib")

    include_directories(${CMAKE_SOURCE_DIR}/vendor/include)
    link_directories(${CMAKE_SOURCE_DIR}/vendor/lib)

    set(RAYLIB_LIB raylib)
else()
    set(RAYLIB_LIB raylib)
endif()

# ==========================================
# ARCHIVOS FUENTE
# ==========================================
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(MasterPiece ${SOURCE_FILES})

# ==========================================
# INCLUDES
# ==========================================
target_include_directories(MasterPiece PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/states
    ${CMAKE_SOURCE_DIR}/vendor/include
)

# Enlace con raylib + librerías necesarias
if (UNIX AND NOT APPLE)
    target_link_libraries(MasterPiece
        PRIVATE
        ${RAYLIB_LIB}
        m dl pthread GL X11 Xrandr Xi Xcursor Xxf86vm
    )
elseif (WIN32)
    target_link_libraries(MasterPiece PRIVATE ${RAYLIB_LIB})
endif()

# ==========================================
# COPIAR ASSETS AUTOMÁTICAMENTE
# ==========================================
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# ==========================================
# PACKAGING AUTOMÁTICO
# ==========================================
include(CPack)

set(CPACK_PACKAGE_NAME "MasterPiece")
set(CPACK_PACKAGE_VERSION "1.0.0")

if (WIN32)
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

cpack_add_component(MasterPiece)
