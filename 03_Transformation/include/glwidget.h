#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

#include "algebra3.h"
#include <array>
using namespace std;

class glWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    glWidget(QWidget *parent = nullptr);
    ~glWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

private:
    int initVertexBuffers(); // initialize vbo 
    void setAttributeBuffer(GLint num, GLchar* name);

private:
    QOpenGLShaderProgram* program;  
    GLuint vertexBuffer, colorBuffer;
    mat4 M; // use algebra-matrix for 3D Transformation
};

#endif // GLWIDGET_H
