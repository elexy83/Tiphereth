#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <SFML/System/String.hpp>

/**
 * @brief Manages text translations and language switching (i18n).
 *
 * The LocalizationManager stores a dictionary of key-value string pairs.
 * It reads dynamic JSON files from the assets directory to load languages.
 */
class LocalizationManager {
private:
    /// Dictionary mapping translation keys to localized strings.
    std::unordered_map<std::string, std::string> m_texts;

    /// The currently loaded language code (e.g., "fr", "en").
    std::string m_currentLanguage;

public:
    /**
     * @brief Loads a specific language translation set from a JSON file.
     *
     * @param languageCode The locale code (e.g., "en", "fr").
     */
    void loadLanguage(const std::string& languageCode) {
        m_currentLanguage = languageCode;
        m_texts.clear();

        std::string filepath = "assets/lang/" + languageCode + ".json";

        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "LocalizationManager::loadLanguage - Impossible d'ouvrir le fichier : " << filepath << std::endl;
            return;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        file.close();

        std::regex pattern(R"(\"([^"]+)\"\s*:\s*\"([^"]*)\")");

        auto words_begin = std::sregex_iterator(content.begin(), content.end(), pattern);
        auto words_end = std::sregex_iterator();

        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            std::smatch match = *i;
            m_texts[match[1].str()] = match[2].str();
        }

        std::cout << "Langue chargee avec succes : " << languageCode << " (" << m_texts.size() << " traductions)" << std::endl;
    }

    /**
     * @brief Retrieves the translated text for a given key.
     *
     * @param key The translation key (e.g., "menu.play").
     * @return sf::String The translated text ready for SFML.
     */
    sf::String get(const std::string& key) const {
        auto it = m_texts.find(key);
        std::string result;
        if (it != m_texts.end()) {
            result = it->second;
        }
        else {
            result = "[" + key + "]";
        }

        return sf::String::fromUtf8(result.begin(), result.end());
    }

    /**
     * @brief Gets the currently active language code.
     *
     * @return const std::string& Current language code.
     */
    const std::string& getCurrentLanguage() const {
        return m_currentLanguage;
    }
};