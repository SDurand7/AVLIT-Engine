#include "OGLFramebuffer.hpp"

#include <random>
#include <cmath>

#include <Core/Base/include/Math.hpp>

#include "bluenoise.h"


namespace AVLIT {

GLint OGLFramebuffer::m_defaultID = -1;

OGLFramebuffer::OGLFramebuffer(std::vector<std::tuple<GLint, GLenum, GLenum>> &&buffersInfos, GLuint width,
                               GLuint height, GLuint textureUnit)
    : m_textures(buffersInfos.size()),
      m_samplers(m_textures.size()), m_buffersInfos{std::move(buffersInfos)}, m_size{std::pair<GLuint, GLuint>{
                                                                                  width, height}} {
    OGLFramebuffer::saveDefault();

    glGenFramebuffers(1, &m_fboID);
    bind();

    glGenTextures(m_textures.size(), m_textures.data());
    glGenSamplers(m_samplers.size(), m_samplers.data());
    glBindSamplers(textureUnit, m_samplers.size(), m_samplers.data());
    std::vector<GLuint> attachements(m_textures.size());

    for(size_t i = 0; i < m_textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + textureUnit + i);
        attachements[i] = GL_COLOR_ATTACHMENT0 + i;
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, std::get<0>(m_buffersInfos[i]), width, height, 0, std::get<1>(m_buffersInfos[i]),
                     std::get<2>(m_buffersInfos[i]), nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachements[i], GL_TEXTURE_2D, m_textures[i], 0);

        glSamplerParameteri(m_samplers[i], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glSamplerParameteri(m_samplers[i], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    glDrawBuffers(attachements.size(), attachements.data());

    glGenRenderbuffers(1, &m_rboID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rboID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rboID);

    OGLFramebuffer::bindDefault();
}

OGLFramebuffer::OGLFramebuffer(GLuint width, GLuint height, GLuint textureUnit, GLint textureCompareMode)
    : m_textures(1), m_samplers(2), m_size{std::pair<GLuint, GLuint>{width, height}} {
    OGLFramebuffer::saveDefault();

    glGenFramebuffers(1, &m_fboID);
    bind();

    glGenTextures(1, m_textures.data());
    glGenSamplers(2, m_samplers.data());
    glBindSamplers(textureUnit, m_samplers.size(), m_samplers.data());

    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_textures[0]);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_textures[0], 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glSamplerParameteri(m_samplers[0], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(m_samplers[0], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_samplers[0], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(m_samplers[0], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(m_samplers[0], GL_TEXTURE_COMPARE_MODE, textureCompareMode);

    glSamplerParameteri(m_samplers[1], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(m_samplers[1], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(m_samplers[1], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(m_samplers[1], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    OGLFramebuffer::bindDefault();
}

OGLFramebuffer::OGLFramebuffer(GLuint width, GLuint height, GLuint noiseW, GLuint noiseH, GLuint textureUnit)
    : m_firstToResize{1}, m_textures(2), m_samplers(2), m_size{std::pair<GLuint, GLuint>{width, height}} {
    OGLFramebuffer::saveDefault();

    glGenFramebuffers(1, &m_fboID);
    bind();

    glGenTextures(2, m_textures.data());
    glGenSamplers(2, m_samplers.data());
    glBindSamplers(textureUnit, m_samplers.size(), m_samplers.data());

    glSamplerParameteri(m_samplers[0], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(m_samplers[0], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(m_samplers[0], GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(m_samplers[0], GL_TEXTURE_WRAP_T, GL_REPEAT);

    glSamplerParameteri(m_samplers[1], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(m_samplers[1], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(m_samplers[1], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(m_samplers[1], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // Generating random tangent vectors
    std::vector<Vec3> vectors;

    const int size = noiseW * noiseH;
    vectors.reserve(size);

    // Bluenoise pretty much doesn't make a difference in that case, but it's still cool 
    for(int i = 0; i < noiseH; ++i) {
        for (int j = 0; j < noiseW; ++j) {
            vectors.emplace_back(tile[i][j][0] * 2.f - 1.f, tile[i][j][1] * 2.f - 1.f, 0.f);

            vectors[i] = normalize(vectors[i]);
        }
    }

    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, noiseW, noiseH, 0, GL_RGB, GL_FLOAT, vectors.data());

    glActiveTexture(GL_TEXTURE0 + textureUnit + 1);
    glBindTexture(GL_TEXTURE_2D, m_textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textures[1], 0);
    m_buffersInfos.push_back(std::tuple<GLint, GLenum, GLenum>{GL_R32F, GL_RED, GL_FLOAT});

    GLuint attachement = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &attachement);

    OGLFramebuffer::bindDefault();
}

OGLFramebuffer::OGLFramebuffer(GLuint width, GLuint height, GLuint textureUnit)
    : m_textures(1), m_samplers(1), m_size{std::pair<GLuint, GLuint>{width, height}} {
    OGLFramebuffer::saveDefault();

    glGenFramebuffers(1, &m_fboID);
    bind();

    glGenTextures(1, m_textures.data());
    glGenSamplers(1, m_samplers.data());
    glBindSamplers(textureUnit, m_samplers.size(), m_samplers.data());

    glSamplerParameteri(m_samplers[0], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(m_samplers[0], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(m_samplers[0], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(m_samplers[0], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textures[0], 0);
    m_buffersInfos.push_back(std::tuple<GLint, GLenum, GLenum>{GL_R32F, GL_RED, GL_FLOAT});

    GLuint attachement = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &attachement);

    OGLFramebuffer::bindDefault();
}

OGLFramebuffer::~OGLFramebuffer() {
    glDeleteTextures(m_textures.size(), m_textures.data());
    glDeleteSamplers(m_samplers.size(), m_samplers.data());
    glDeleteRenderbuffers(1, &m_rboID);
    glDeleteFramebuffers(1, &m_fboID);
}

void OGLFramebuffer::resize(GLuint width, GLuint height) {
    for(int i = m_firstToResize; i < static_cast<int>(m_textures.size()); ++i) {
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, std::get<0>(m_buffersInfos[i - m_firstToResize]), width, height, 0,
                     std::get<1>(m_buffersInfos[i - m_firstToResize]), std::get<2>(m_buffersInfos[i - m_firstToResize]),
                     nullptr);
    }

    if(m_rboID != 0) {
        glBindRenderbuffer(GL_RENDERBUFFER, m_rboID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    }

    m_size.first = width;
    m_size.second = height;
}

} // namespace AVLIT