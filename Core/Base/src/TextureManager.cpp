#include "TextureManager.hpp"


namespace AVLIT {

const Texture *TextureManager::addTexture(const std::string &path, const uchar *data, int width, int height,
                                          InternalFormat internalFormat, Format format) {
    auto texture = std::make_unique<Texture>(data, width, height, internalFormat, format);
    auto texturePtr = texture.get();
    m_textures.emplace(path, std::move(texture));
    return texturePtr;
}

const Texture *TextureManager::texture(const std::string &path) const {
    auto textureIt = m_textures.find(path);
    if(textureIt == m_textures.cend())
        return nullptr;
    return textureIt->second.get();
}

const Texture *TextureManager::addCubemap(uchar *datas[6], int width[6], int height[6]) {
    m_cubemap = std::make_unique<Texture>(datas, width, height);
    return m_cubemap.get();
}

} // namespace AVLIT