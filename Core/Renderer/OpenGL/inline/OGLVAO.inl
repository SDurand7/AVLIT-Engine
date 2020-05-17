namespace ALIT {

inline void OGLVAO::operator=(OGLVAO &&vao) noexcept {
    m_vaoID = vao.m_vaoID;
    vao.m_vaoID = 0;
    m_buffers = std::move(vao.m_buffers);
}

inline bool OGLVAO::operator==(const OGLVAO &vao) const { return m_vaoID == vao.m_vaoID; }

inline void OGLVAO::bind() const { glBindVertexArray(m_vaoID); }

inline void OGLVAO::unbindAll() { glBindVertexArray(0); }

} // namespace ALIT