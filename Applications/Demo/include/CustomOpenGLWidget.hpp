#ifndef CUSTOMOPENGLWIDGET_HPP
#define CUSTOMOPENGLWIDGET_HPP

#include <Core/Base/include/Engine.hpp>

#include <QOpenGLWidget>


class CustomOpenGLWidget : public QOpenGLWidget {
    Q_OBJECT
public:
    explicit CustomOpenGLWidget(QWidget *parent = nullptr);

    ~CustomOpenGLWidget();

signals:
    void contextCreated();

protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

private slots:
    void release();
};

#endif