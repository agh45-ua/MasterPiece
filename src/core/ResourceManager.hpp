#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "raylib.h"
#include <map>
#include <string>

class ResourceManager {
public:
    static ResourceManager& getInstance();

    // Métodos para obtener recursos
    Texture2D GetTexture(const std::string& path);
    Font GetFont(const std::string& path);

    // Limpieza total al cerrar el juego
    void UnloadAll();

private:
    ResourceManager() = default;
    ~ResourceManager() = default;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    std::map<std::string, Texture2D> textures;
    std::map<std::string, Font> fonts;
};

#endif