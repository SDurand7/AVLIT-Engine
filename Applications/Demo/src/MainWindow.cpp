#include "MainWindow.hpp"

#include "ui_MainWindow.h"

#include <QKeyEvent>
#include <QFileDialog>
#include <QDrag>
#include <QMimeData>
#include <QOpenGLContext>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), m_drawTimer(this) {
    QSurfaceFormat format;
    format.setVersion(4, 4);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    ui->setupUi(this);

    connect(ui->oglWidget, SIGNAL(contextCreated()), this, SLOT(initializeDemo()));

    m_drawTimer.start(1000 / 60.f);
    connect(&m_drawTimer, SIGNAL(timeout()), &m_eventManager, SLOT(pollEvents()));
    connect(&m_drawTimer, SIGNAL(timeout()), ui->oglWidget, SLOT(update()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::initializeDemo() {
    m_app = std::make_unique<Application>(ui->oglWidget->width(), ui->oglWidget->height());
    const float speed = 10.f;

    m_eventManager.addKey(Qt::Key::Key_Z, [demo = m_app.get(), speed](float elapsed) {
        float distance = speed * elapsed;
        demo->translateCameraFront(distance);
    });

    m_eventManager.addKey(Qt::Key::Key_S, [demo = m_app.get(), speed](float elapsed) {
        float distance = speed * elapsed;
        demo->translateCameraFront(-distance);
    });

    m_eventManager.addKey(Qt::Key::Key_D, [demo = m_app.get(), speed](float elapsed) {
        float distance = speed * elapsed;
        demo->translateCameraRight(distance);
    });

    m_eventManager.addKey(Qt::Key::Key_Q, [demo = m_app.get(), speed](float elapsed) {
        float distance = speed * elapsed;
        demo->translateCameraRight(-distance);
    });

    m_eventManager.setMouseCallback(MouseEvent::LEFT_DRAGGED, [demo = m_app.get()](QPoint start, QPoint end) {
        int dx = end.x() - start.x();
        int dy = end.y() - start.y();
        demo->rotateCamera(dx, dy);
    });

    m_eventManager.setMouseReleaseCallback(
        MouseEvent::LEFT_CLICKED, [demo = m_app.get(), &eventManager = m_eventManager]() {
            eventManager.setMouseCallback(MouseEvent::LEFT_DRAGGED, [demo](QPoint start, QPoint end) {
                int dx = end.x() - start.x();
                int dy = end.y() - start.y();
                demo->rotateCamera(dx, dy);
            });
        });
}

void MainWindow::resizeEvent(QResizeEvent *) {
    if(m_app)
        m_app->resize(ui->oglWidget->width(), ui->oglWidget->height());
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    event->accept();
    if(ui->oglWidget->hasFocus())
        m_eventManager.keyPressed(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    event->accept();
    m_eventManager.keyReleased(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    event->accept();
    if(ui->oglWidget->hasFocus())
        m_eventManager.mousePressed(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    event->accept();
    m_eventManager.mouseReleased(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    event->accept();
    if(ui->oglWidget->hasFocus())
        m_eventManager.mouseMoved(event);
}

void MainWindow::loadModel() {
    QString filename =
        QFileDialog::getOpenFileName(this, tr("Open model"), "../../../", tr("Model (*.obj *.dae *.fbx)"));

    if(!filename.isNull()) {
        ui->oglWidget->makeCurrent();

        QAction *action = new QAction;
        action->setText(QString::fromStdString(m_app->loadModel(filename.toStdString())));
        action->setAutoRepeat(false);

        ui->oglWidget->doneCurrent();
    }
}

void MainWindow::setSkybox() {
    std::vector<std::string> filenames;
    filenames.reserve(6);

    std::string sides[6] = {"right", "left", "top", "bottom", "front", "back"};
    std::string lastDir{"../../../"};

    for(int i = 0; i < 6; ++i) {
        QString filename =
            QFileDialog::getOpenFileName(this, tr(("Open skybox " + sides[i] + " image").c_str()), tr(lastDir.c_str()),
                                         tr(("Skybox " + sides[i] + " image (*.jpg *.png)").c_str()));

        if(filename.isNull()) {
            return;
        }

        filenames.emplace_back(filename.toStdString());
        lastDir = filenames.back().substr(0, filenames.back().find_last_of('/'));
    }

    ui->oglWidget->makeCurrent();
    m_app->setSkybox(filenames);
    ui->oglWidget->doneCurrent();
}

void MainWindow::reloadShaders() {
    ui->oglWidget->makeCurrent();
    m_app->reloadShaders();
    ui->oglWidget->doneCurrent();
}
