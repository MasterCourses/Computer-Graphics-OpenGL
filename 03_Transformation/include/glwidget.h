#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

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
    QMatrix4x4  transformMatrix; // use qt-opengl framework
};

#endif // GLWIDGET_H
