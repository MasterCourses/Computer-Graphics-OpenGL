#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

#include <iostream>
#include <stack>
#include "algebra3.h"
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
    GLfloat* getMatrixElements(mat4 matrix);
    
private:
    QOpenGLShaderProgram* program;  
    GLuint aPosition, aColor, uModelMatrix;
    GLuint numRectVertex, rectVerticesBuffer;
    GLuint numColor, redColorBuffer, greenColorBuffer, blueColorBuffer;

    // use algebra-matrix for 3D Transformation
    mat4 M; 
    stack<mat4> matrixStack;
    static GLfloat transformMatrix[16];

};

#endif // GLWIDGET_H
