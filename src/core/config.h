#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>
#include <filesystem>

// Ruta base de assets
#ifndef ASSETS_PATH
#define ASSETS_PATH "/usr/share/masterpiece/assets/"
#endif

// Función para obtener la ruta correcta de un asset
inline std::string GetAssetPath(const std::string& assetName) {
    // Primero intenta la ruta local (para desarrollo)
    std::string localPath = "assets/" + assetName;
    std::cout << localPath << std::endl;
    if (std::filesystem::exists(localPath)) {
        std::cout << "Asset in local path exists" << std::endl;
        return localPath;
    }
    // Luego se prueba la ruta relativa al ejecutable (por si se está ejecutando desde el directorio bin/)
    localPath = "../assets/" + assetName;;
    std::cout << localPath << std::endl;
    if (std::filesystem::exists(localPath)) {
        std::cout << "Asset in local path exists" << std::endl;
        return localPath;
    }
    // Si no existe, usa la ruta instalada
    std::string response = std::string(ASSETS_PATH) + assetName;
    std::cout << response << std::endl;
    return response;
}

#endif // CONFIG_H