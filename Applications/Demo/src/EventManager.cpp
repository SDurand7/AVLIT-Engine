#include "EventManager.hpp"


EventManager::EventManager() {
    std::fill(m_mouseCallbacks, m_mouseCallbacks + MOUSEBINDS, [](QPointF, QPointF) {});
    std::fill(m_mouseReleaseCallbacks, m_mouseReleaseCallbacks + MOUSEBINDS, []() {});
}

void EventManager::addKey(Qt::Key key, std::function<void(float)> &&callback) {
    m_keyboardPressed.push_back(false);
    m_keyboardCallbackIndices.insert(std::pair<int, int>(key, m_keyboardCallbacks.size()));
    m_keyboardCallbacks.emplace_back(std::move(callback));
}

void EventManager::setMouseCallback(MouseEvent clickType, std::function<void(QPoint, QPoint)> &&callback) {
    m_mouseCallbacks[static_cast<int>(clickType)] = std::move(callback);
}

void EventManager::setMouseReleaseCallback(MouseEvent clickType, std::function<void()> &&callback) {
    m_mouseReleaseCallbacks[static_cast<int>(clickType)] = std::move(callback);
}

void EventManager::pollEvents() {
    const float elapsed =
        std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - m_lastPollTime)
            .count() /
        1000000.f;
    for(int i = 0; i < static_cast<int>(m_keyboardPressed.size()); ++i) {
        if(m_keyboardPressed[i])
            m_keyboardCallbacks[i](elapsed);
    }

    if(m_mousePressed[static_cast<int>(MouseEvent::LEFT_DRAGGED)] && m_startPos != m_lastPos) {
        m_mouseCallbacks[static_cast<int>(MouseEvent::LEFT_DRAGGED)](m_startPos, m_lastPos);
        m_startPos = m_lastPos;
    } else if(m_mousePressed[static_cast<int>(MouseEvent::LEFT_CLICKED)]) {
        m_mouseCallbacks[static_cast<int>(MouseEvent::LEFT_CLICKED)](m_startPos, m_lastPos);
        m_mousePressed[static_cast<int>(MouseEvent::LEFT_CLICKED)] = false;
        m_mousePressed[static_cast<int>(MouseEvent::LEFT_DRAGGED)] = true;
    }

    if(m_mousePressed[static_cast<int>(MouseEvent::RIGHT_DRAGGED)] && m_startPos != m_lastPos) {
        m_mouseCallbacks[static_cast<int>(MouseEvent::RIGHT_DRAGGED)](m_startPos, m_lastPos);
        m_startPos = m_lastPos;
    } else if(m_mousePressed[static_cast<int>(MouseEvent::RIGHT_CLICKED)]) {
        m_mouseCallbacks[static_cast<int>(MouseEvent::RIGHT_CLICKED)](m_startPos, m_lastPos);
        m_mousePressed[static_cast<int>(MouseEvent::RIGHT_CLICKED)] = false;
        m_mousePressed[static_cast<int>(MouseEvent::RIGHT_DRAGGED)] = true;
    }

    m_lastPollTime = std::chrono::steady_clock::now();
}

void EventManager::keyPressed(QKeyEvent *event) {
    if(!event->isAutoRepeat()) {
        const auto it = m_keyboardCallbackIndices.find(event->key());
        if(it != m_keyboardCallbackIndices.cend())
            m_keyboardPressed[it->second] = true;
    }
}

void EventManager::keyReleased(QKeyEvent *event) {
    if(!event->isAutoRepeat()) {
        const auto it = m_keyboardCallbackIndices.find(event->key());
        if(it != m_keyboardCallbackIndices.cend())
            m_keyboardPressed[m_keyboardCallbackIndices[event->key()]] = false;
    }
}

void EventManager::mousePressed(QMouseEvent *event) {
    MouseEvent clickType;
    switch(event->button()) {
    case Qt::MouseButton::LeftButton:
        clickType = MouseEvent::LEFT_CLICKED;
        break;
    case Qt::MouseButton::RightButton:
        clickType = MouseEvent::RIGHT_CLICKED;
        break;
    default:
        return;
    }
    m_mousePressed[static_cast<int>(clickType)] = true;
    m_startPos = m_lastPos = event->pos();
}

void EventManager::mouseReleased(QMouseEvent *event) {
    MouseEvent clickType;
    MouseEvent dragType;
    switch(event->button()) {
    case Qt::MouseButton::LeftButton:
        clickType = MouseEvent::LEFT_CLICKED;
        dragType = MouseEvent::LEFT_DRAGGED;
        break;
    case Qt::MouseButton::RightButton:
        clickType = MouseEvent::RIGHT_CLICKED;
        dragType = MouseEvent::RIGHT_DRAGGED;
        break;
    default:
        return;
    }
    m_mouseReleaseCallbacks[static_cast<int>(clickType)]();
    m_mousePressed[static_cast<int>(clickType)] = false;
    m_mousePressed[static_cast<int>(dragType)] = false;
}

void EventManager::mouseMoved(QMouseEvent *event) {
    if(m_mousePressed[static_cast<int>(MouseEvent::LEFT_CLICKED)]) {
        m_mousePressed[static_cast<int>(MouseEvent::LEFT_DRAGGED)] = true;
        m_mousePressed[static_cast<int>(MouseEvent::LEFT_CLICKED)] = false;
        m_startPos = event->pos();
        m_lastPos = m_startPos;
    } else if(m_mousePressed[static_cast<int>(MouseEvent::LEFT_DRAGGED)]) {
        m_lastPos = event->pos();
    } else if(m_mousePressed[static_cast<int>(MouseEvent::RIGHT_CLICKED)]) {
        m_mousePressed[static_cast<int>(MouseEvent::RIGHT_DRAGGED)] = true;
        m_mousePressed[static_cast<int>(MouseEvent::RIGHT_CLICKED)] = false;
        m_startPos = event->pos();
        m_lastPos = event->pos();
    } else if(m_mousePressed[static_cast<int>(MouseEvent::RIGHT_DRAGGED)]) {
        m_lastPos = event->pos();
    }
}
