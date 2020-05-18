#include "OGLVAO.hpp"

#include <Core/Base/include/Mesh.hpp>


namespace AVLIT {

OGLVAO::OGLVAO(const Mesh &mesh, bool isDebug) : m_buffers(2) {
    glGenVertexArrays(1, &m_vaoID);
    glGenBuffers(2, m_buffers.data());
    glBindVertexArray(m_vaoID);

    const auto &indices = mesh.indices();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

    const auto &vertices = mesh.vertices();
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    if(mesh.hasNormals()) {
        GLuint normalsID;
        glGenBuffers(1, &normalsID);
        const auto &normals = mesh.normals();
        glBindBuffer(GL_ARRAY_BUFFER, normalsID);
        glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * normals.size(), normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(2);
        m_buffers.push_back(normalsID);
    } else {
        glVertexAttrib3f(2, 1.f, 0.f, 0.f);
    }

    if(!isDebug) {
        if(mesh.hasTexCoords()) {
            GLuint texCoordsID;
            glGenBuffers(1, &texCoordsID);
            const auto &texCoords = mesh.texCoords();
            glBindBuffer(GL_ARRAY_BUFFER, texCoordsID);
            glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(GLfloat) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
            glEnableVertexAttribArray(1);
            m_buffers.push_back(texCoordsID);
        } else {
            glVertexAttrib2f(1, 0.f, 0.f);
        }

        if(mesh.hasColors()) {
            GLuint colorsID;
            glGenBuffers(1, &colorsID);
            const auto &colors = mesh.colors();
            glBindBuffer(GL_ARRAY_BUFFER, colorsID);
            glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * colors.size(), colors.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
            glEnableVertexAttribArray(3);
            m_buffers.push_back(colorsID);
        } else {
            glVertexAttrib3f(3, 1.f, 1.f, 1.f);
        }

        if(mesh.hasTangentsAndBitangents()) {
            GLuint tangentsIDs[2];
            glGenBuffers(2, tangentsIDs);
            const auto &tangents = mesh.tangents();
            const auto &bitangents = mesh.bitangents();
            glBindBuffer(GL_ARRAY_BUFFER, tangentsIDs[0]);
            glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * tangents.size(), tangents.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
            glEnableVertexAttribArray(4);
            m_buffers.push_back(tangentsIDs[0]);

            glBindBuffer(GL_ARRAY_BUFFER, tangentsIDs[1]);
            glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * bitangents.size(), bitangents.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
            glEnableVertexAttribArray(5);
            m_buffers.push_back(tangentsIDs[1]);
        }
    }
    glBindVertexArray(0);
} // namespace AVLIT

OGLVAO::~OGLVAO() {
    glDeleteBuffers(static_cast<GLsizei>(m_buffers.size()), m_buffers.data());
    glDeleteVertexArrays(1, &m_vaoID);
}

} // namespace AVLIT
