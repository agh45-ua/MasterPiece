#include <iostream>
#include "raylib.h"

int main()
{
    std::cout
        << "Raylib version: "
        << RAYLIB_VERSION_MAJOR << "."
        << RAYLIB_VERSION_MINOR << "."
        << RAYLIB_VERSION_PATCH
        << std::endl;

    return 0;
}
