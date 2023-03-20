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
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

private:
    void initVertexBufferForLaterUse();
    void initAttributeVariable(GLuint arrtibute, GLint num, GLuint buffer);
    void drawOneViewport(float viewportX, float  viewportY, float  viewportWidth, float  viewportHeight, float bgColorR, float bgColorG, float bgColorB,
                         QMatrix4x4 projMatrix, QMatrix4x4 viewMatrix, QMatrix4x4 modelMatrixTriangleSet1, QMatrix4x4 modelMatrixTriangleSet2);
    
private:
    QOpenGLShaderProgram* program;  
    GLuint aPosition, aColor, uMVPMatrix;
    GLuint numVertices, vertexBuffer, colorBuffer;

    QMatrix4x4 modelMatrix1, frontViewMatrix, pespProjMatrix, transformMatrix;  // use qt-opengl framework for 3D space Transformation
    float mouseLastX, mouseLastY;
    bool mouseDragging = false;
    float angleX = 0.0, angleY = 0.0;
};

#endif // GLWIDGET_H
