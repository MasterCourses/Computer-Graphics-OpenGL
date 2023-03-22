#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QMouseEvent>

#define M_PI       3.14159265358979323846  

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

    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

private:
    void initVertexBufferForLaterUse();
    void initAttributeVariable(GLuint arrtibute, GLint num, GLuint buffer);
    void draw();
   
private:
    QOpenGLShaderProgram* program;  
    GLuint aPosition, aColor, aNormal, uMVPMatrix, uModelMatrix, uLightPosition, uViewPosition, uKa, uKd, uKs, uShininess;
    GLuint numVertices, vertexBuffer, colorBuffer, normalBuffer;


    QMatrix4x4 modelMatrix, mvpMatrix;  
    float cameraX = 3.0, cameraY = 3.0, cameraZ = 7.0;
    float mouseLastX, mouseLastY;
    bool mouseDragging = false;
    float angleX = 0.0, angleY = 0.0;
};

#endif // GLWIDGET_H
