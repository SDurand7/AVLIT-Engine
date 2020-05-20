#pragma once

#define MOUSEBINDS 4

#include <QKeyEvent>
#include <QMouseEvent>

#include <bitset>
#include <array>
#include <functional>
#include <chrono>
#include <unordered_map>


// TODO: - Handle "released" events callbacks
//       - Cleanup
enum class MouseEvent { LEFT_DRAGGED = 0, RIGHT_DRAGGED = 1, LEFT_CLICKED = 2, RIGHT_CLICKED = 3, COUNT = 4 };

class EventManager : public QObject {
    Q_OBJECT
public:
    EventManager();

    void addKey(Qt::Key key, std::function<void(float)> &&callback, bool repeat = true);

    void setMouseCallback(MouseEvent type, std::function<void(QPoint, QPoint)> &&callback);

    void setMouseReleaseCallback(MouseEvent type, std::function<void()> &&callback);

public slots:
    void pollEvents();

    void keyPressed(QKeyEvent *event);

    void keyReleased(QKeyEvent *event);

    void mousePressed(QMouseEvent *event);

    void mouseReleased(QMouseEvent *event);

    void mouseMoved(QMouseEvent *event); // This actually gets called while dragging, because Qt ¯\_(ツ)_/¯


private:
    std::chrono::steady_clock::time_point m_lastPollTime;

    // Keyboard
    std::vector<bool> m_keyboardPressed;
    std::vector<bool> m_repeatEvent;
    std::unordered_map<int, int> m_keyboardCallbackIndices;
    std::vector<std::function<void(float)>> m_keyboardCallbacks;

    // Mouse
    std::bitset<MOUSEBINDS> m_mousePressed;
    std::function<void(QPoint, QPoint)> m_mouseCallbacks[MOUSEBINDS];
    std::function<void()> m_mouseReleaseCallbacks[MOUSEBINDS]; 
    QPoint m_startPos;
    QPoint m_lastPos;
};
