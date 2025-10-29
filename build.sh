#!/bin/bash
# build.sh - Script para compilar y ejecutar el juego sin bin/

set -e

# Compilar
g++ -o game src/*.cpp -I src/ -I vendor/include/ -L vendor/lib -lraylib -lGL -lm -lpthread -lrt -lX11

echo "¡Compilación completada! Ejecutable: ./game"

# Ejecutar el juego
./game
