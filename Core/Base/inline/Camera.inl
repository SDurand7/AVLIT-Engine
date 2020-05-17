namespace ALIT {

inline Mat4 Camera::view() const {
    Mat4 view;

    const Vec3 &translation = m_transform[3];
    const Mat3 &rotation = m_transform;

    view = transpose(rotation);
    view[3][0] = -dot(translation, rotation[0]);
    view[3][1] = -dot(translation, rotation[1]);
    view[3][2] = -dot(translation, rotation[2]);
    view[3][3] = 1.f;

    return view;
}

inline const Mat4 &Camera::projection() const {
    if(m_dirtyProjection)
        updateProjectionMatrix();
    return m_projection;
}

inline const Mat4 &Camera::inverseProjection() const {
    if(m_dirtyProjection)
        updateProjectionMatrix();
    return m_invProjection;
}

inline int Camera::width() const { return m_width; }

inline int Camera::height() const { return m_height; }

inline void Camera::setWidth(int width) {
    m_width = width;
    m_dirtyProjection = true;
}

inline void Camera::setHeight(int height) {
    m_height = height;
    m_dirtyProjection = true;
}

inline float Camera::fovx() const { return m_fovy * m_width / m_height; }

inline float Camera::fovy() const { return m_fovy; }

inline void Camera::setFovy(float angle) {
    m_fovy = angle;
    m_dirtyProjection = true;
}

inline float Camera::nearZ() const { return m_near; }

inline float Camera::farZ() const { return m_far; }

inline void Camera::setNearZ(float z) {
    m_near = z;
    m_dirtyProjection = true;
}

inline void Camera::setFarZ(float z) {
    m_far = z;
    m_dirtyProjection = true;
}

inline void Camera::updateProjectionMatrix() const {
    m_projection = perspective(m_fovy, static_cast<float>(m_width) / m_height, m_near, m_far);
    m_invProjection = inverse(m_projection);
    m_dirtyProjection = false;
}

} // namespace ALIT