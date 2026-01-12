#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <cstring>

// Ruta base de assets
#ifndef ASSETS_PATH
#define ASSETS_PATH "/usr/share/masterpiece/assets/"
#endif

// Función para obtener la ruta correcta de un asset
inline std::string GetAssetPath(const std::string& assetName) {
    // Primero intenta la ruta local (para desarrollo)
    std::string localPath = "assets/" + assetName;
    if (std::filesystem::exists(localPath)) {
        return localPath;
    }
    // Luego se prueba la ruta relativa al ejecutable (por si se está ejecutando desde el directorio bin/)
    std::string binPath = "../assets/" + assetName;
    if (std::filesystem::exists(binPath)) {
        return binPath;
    }
    // Si no existe, usa la ruta instalada
    return std::string(ASSETS_PATH) + assetName;
}

// Función para detectar si el idioma actual es japonés
inline bool IsJapaneseLanguage() {
    const char* lang = std::getenv("LANGUAGE");
    if (!lang) lang = std::getenv("LANG");
    if (!lang) lang = std::getenv("LC_ALL");
    
    if (lang) {
        std::string langStr(lang);
        // Buscar "ja" en la variable de idioma
        return (langStr.find("ja") == 0 || langStr.find("ja_") != std::string::npos);
    }
    return false;
}

// Función para obtener la fuente según el idioma
inline std::string GetFontPath() {
    if (IsJapaneseLanguage()) {
        return GetAssetPath("NotoSansJP.ttf");
    }
    return GetAssetPath("Poppins-Bold.ttf");
}

#endif // CONFIG_H