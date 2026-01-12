#include "ResourceManager.hpp"
#include <iostream>

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

Texture2D ResourceManager::GetTexture(const std::string& path) {
    // Si no está en el mapa, lo cargamos
    if (textures.find(path) == textures.end()) {
        textures[path] = LoadTexture(path.c_str());
        TraceLog(LOG_INFO, "Resource Manager: Textura cargada desde [%s]", path.c_str());
    }
    return textures[path];
}

Font ResourceManager::GetFont(const std::string& path) {
    if (fonts.find(path) == fonts.end()) {
        fonts[path] = LoadFont(path.c_str());
        TraceLog(LOG_INFO, "Resource Manager: Fuente cargada desde [%s]", path.c_str());
    }
    return fonts[path];
}

void ResourceManager::UnloadAll() {
    for (auto& pair : textures) UnloadTexture(pair.second);
    for (auto& pair : fonts) UnloadFont(pair.second);
    
    textures.clear();
    fonts.clear();
    TraceLog(LOG_INFO, "Resource Manager: Todos los recursos han sido liberados");
}