#pragma once

#include <Applications/Demo/include/CustomOpenGLWidget.hpp>
#include <Applications/Demo/include/EventManager.hpp>
#include <Applications/Demo/include/Application.hpp>

#include <QMainWindow>
#include <QTimer>
#include <QTreeWidgetItem>

#include <memory>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public slots:
    void initializeDemo();

    void resizeEvent(QResizeEvent *) override;

    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void loadModel();

    void setSkybox();

    void reloadShaders();

    Ui::MainWindow *ui;

    EventManager m_eventManager;

    QTimer m_drawTimer;

    std::unique_ptr<Application> m_app;
};