#ifndef ALIT_TEXTUREMANAGER_HPP
#define ALIT_TEXTUREMANAGER_HPP

#include <unordered_map>
#include <string>

#include <Core/Base/include/Macros.hpp>
#include <Core/Base/include/Types.hpp>
#include <Core/Renderer/include/Texture.hpp>


namespace ALIT {

class TextureManager {
public:
    TextureManager() = default;

    const Texture *addTexture(const std::string &path, const uchar *data, int width, int height,
                              InternalFormat internalFormat, Format format);

    const Texture *texture(const std::string &path) const;

    const Texture *addCubemap(uchar *datas[6], int width[6], int height[6]);

private:
    std::unordered_map<std::string, TextureUptr> m_textures;

    TextureUptr m_cubemap;
};

} // namespace ALIT

#endif