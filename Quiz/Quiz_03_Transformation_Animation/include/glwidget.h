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
    void initVertexBufferForLaterUse();
    void initAttributeVariable(GLuint arrtibute, GLint num, GLuint buffer);
    void draw();
    
private:
    QOpenGLShaderProgram* program;  
    GLuint aPosition, aColor, uModelMatrix;
    GLint pointAngle = 0; GLint triangleAngle = 0;
    GLuint numCenterPoint, vertCenterPointBuffer, colCenterPointBuffer;
    GLuint numRotatingPoint, vertRotatingPointBuffer, colRotatingPointBuffer;
    GLuint numTriangle, vertTriangleBuffer, colTriangleBuffer;

    // use algebra-matrix for 3D Transformation
    mat4 M1, M2, M3; // M1: for green triangle; M2: for center white point; M3: for red point

};

#endif // GLWIDGET_H
