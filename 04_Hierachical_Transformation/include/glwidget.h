#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

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
    void initVertexBufferForLaterUse();
    void initAttributeVariable(GLuint arrtibute, GLint num, GLuint buffer);
    void draw();
    
private:
    QOpenGLShaderProgram* program;  
    GLuint aPosition, aColor, uModelMatrix;
    GLuint numRectVertex, rectVerticesBuffer;
    GLuint numColor, redColorBuffer, greenColorBuffer, blueColorBuffer;

    // use qt-opengl for 3D Transformation
    QMatrix4x4 transformMatrix;
    QVector<QMatrix4x4> matrixStack;
};

#endif // GLWIDGET_H
