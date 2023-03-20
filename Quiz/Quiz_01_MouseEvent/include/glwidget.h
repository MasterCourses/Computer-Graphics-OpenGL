#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QMouseEvent>

class glWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    glWidget(QWidget *parent = nullptr);
    ~glWidget();
signals:

public slots:

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void mousePressEvent(QMouseEvent* event);

private:
    QOpenGLShaderProgram* program;        
    QVector<QVector2D> gPoints; // store all clicked positions
    QVector<QVector4D> gColors; // store colors of each points
};

#endif // GLWIDGET_H
