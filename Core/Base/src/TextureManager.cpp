#include "TextureManager.hpp"


namespace AVLIT {

const Texture *TextureManager::addTexture(const std::string &name, uchar **data, uint width, uint height,
                                          TextureType textureType, TextureInternalFormat internalFormat,
                                          TextureFormat format) {
    auto texture = std::make_unique<Texture>(internalFormat, format, textureType);

    Texture *texturePtr = texture.get();
    texturePtr->allocate(data, width, height);

    m_textures.emplace(name, std::move(texture));

    return texturePtr;
}

const Texture *TextureManager::texture(const std::string &path) const {
    const auto textureIt = m_textures.find(path);

    if(textureIt == m_textures.cend())
        return nullptr;
    return textureIt->second.get();
}

} // namespace AVLIT