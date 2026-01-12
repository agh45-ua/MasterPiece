#ifndef CONFIG_H
#define CONFIG_H

// PARCHE PARA MSVC (Windows)
#ifdef _WIN32
    #ifndef _CRT_SECURE_NO_WARNINGS
        #define _CRT_SECURE_NO_WARNINGS
    #endif
#endif

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
    std::string localPath = "assets/" + assetName;
    if (std::filesystem::exists(localPath)) {
        return localPath;
    }
    std::string binPath = "../assets/" + assetName;
    if (std::filesystem::exists(binPath)) {
        return binPath;
    }
    return std::string(ASSETS_PATH) + assetName;
}

// Función para detectar si el idioma actual es japonés
inline bool IsJapaneseLanguage() {
    const char* lang = std::getenv("LANGUAGE");
    if (!lang) lang = std::getenv("LANG");
    if (!lang) lang = std::getenv("LC_ALL");
    
    if (lang) {
        std::string langStr(lang);
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

// BLOQUE DE INTERNACIONALIZACIÓN
#ifndef _WIN32
    #include <libintl.h>
    #include <locale.h>
    #ifndef _
        #define _(String) gettext(String)
    #endif
#else
    #ifndef _
        #define _(String) (String)
    #endif
    #define gettext(String) (String)
    #define textdomain(Domain)
    #define bindtextdomain(Domain, Dir)
    #define setlocale(Category, Locale)
    #ifndef LC_ALL
        #define LC_ALL 0
    #endif
#endif

#endif // CONFIG_H
