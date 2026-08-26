#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <string>
#include <stdexcept>
#include <cassert>

namespace Textures {
    enum class ID {
        Player,
        Enemy,
        Background,
        MenuButton
    };
}

namespace Fonts {
    enum class ID {
        Main,
        Title
    };
}

template <typename Resource, typename Identifier>
class ResourceManager {
public:


    void load(Identifier id, const std::string& filename)
    {

        std::unique_ptr<Resource> resource = std::make_unique<Resource>();

        if (!resource->loadFromFile(filename)) {
            throw std::runtime_error("ResourceManager::load - Unable to load the file : " + filename);
        }

        mResourceMap.insert(std::make_pair(id, std::move(resource)));
    }

    Resource& get(Identifier id)
    {

        auto found = mResourceMap.find(id);

        assert(found != mResourceMap.end() && "ResourceManager::get - Resource not found");

        return *found->second;
    }

    const Resource& get(Identifier id) const
    {
        auto found = mResourceMap.find(id);

        assert(found != mResourceMap.end() && "ResourceManager::get - Resource not found !");

        return *found->second;
    }

private:
    std::unordered_map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};


using TextureManager = ResourceManager<sf::Texture, Textures::ID>;
using FontManager = ResourceManager<sf::Font, Fonts::ID>;