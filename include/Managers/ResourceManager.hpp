#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <string>
#include <stdexcept>
#include <cassert>

/**
 * @brief Contains unique identifiers for texture assets used in the game.
 */
namespace Textures {
    enum class ID {
        Player,
        Enemy,
        Background,
        MenuButton
    };
}

/**
 * @brief Contains unique identifiers for font assets used in the game.
 */
namespace Fonts {
    enum class ID {
        Main,
        Title
    };
}

/**
 * @brief A generic template manager for loading and storing SFML resources.
 *
 * ResourceManager acts as a central repository to prevent loading the same resource
 * multiple times from the hard drive. It utilizes smart pointers (std::unique_ptr)
 * for safe memory management.
 *
 * @tparam Resource The SFML resource type (e.g., sf::Texture, sf::Font).
 * @tparam Identifier The enum class used as a key (e.g., Textures::ID, Fonts::ID).
 */
template <typename Resource, typename Identifier>
class ResourceManager {
public:
    /**
     * @brief Loads a resource from a file on disk and stores it in the manager.
     *
     * @param id The unique identifier key to associate with this resource.
     * @param filename The path to the file on disk (relative or absolute).
     * @throws std::runtime_error If the file fails to load via SFML's loadFromFile().
     */
    void load(Identifier id, const std::string& filename)
    {
        std::unique_ptr<Resource> resource = std::make_unique<Resource>();

        if (!resource->loadFromFile(filename)) {
            throw std::runtime_error("ResourceManager::load - Unable to load the file : " + filename);
        }

        mResourceMap.insert(std::make_pair(id, std::move(resource)));
    }

    /**
     * @brief Retrieves a reference to an already loaded resource.
     *
     * @param id The unique identifier of the resource to retrieve.
     * @return Resource& A reference to the requested resource.
     * @note Triggers an assertion failure if the resource has not been loaded prior.
     */
    Resource& get(Identifier id)
    {
        auto found = mResourceMap.find(id);

        assert(found != mResourceMap.end() && "ResourceManager::get - Resource not found");

        return *found->second;
    }

    /**
     * @brief Retrieves a constant reference to an already loaded resource (Read-only).
     *
     * @param id The unique identifier of the resource to retrieve.
     * @return const Resource& A constant reference to the requested resource.
     * @note Triggers an assertion failure if the resource has not been loaded prior.
     */
    const Resource& get(Identifier id) const
    {
        auto found = mResourceMap.find(id);

        assert(found != mResourceMap.end() && "ResourceManager::get - Resource not found !");

        return *found->second;
    }

private:
    /// Dictionary mapping unique identifiers to unique pointers of resources.
    std::unordered_map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

/// Type alias for managing sf::Texture resources using Textures::ID keys.
using TextureManager = ResourceManager<sf::Texture, Textures::ID>;

/// Type alias for managing sf::Font resources using Fonts::ID keys.
using FontManager = ResourceManager<sf::Font, Fonts::ID>;