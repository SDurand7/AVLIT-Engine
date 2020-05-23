#pragma once

#include <unordered_map>
#include <string>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Renderer/include/Texture.hpp>


namespace AVLIT {

// Handle the creation/destruction of texture loaded in the AssetManager
class TextureManager {
public:
    TextureManager() = default;

    const Texture *addTexture(const std::string &name, uchar **data, uint width, uint height, TextureType textureType,
                              TextureInternalFormat internalFormat = TextureInternalFormat::RGB,
                              TextureFormat format = TextureFormat::RGB);

    const Texture *texture(const std::string &path) const;

private:
    std::unordered_map<std::string, TextureUptr> m_textures;

    TextureUptr m_cubemap;
};

} // namespace AVLIT