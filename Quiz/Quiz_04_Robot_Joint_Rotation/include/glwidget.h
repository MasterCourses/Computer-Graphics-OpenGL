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

signals:
    void xTranslateChange(int value);
    void jointRedRotationChange(int value);
    void jointGreenRotationChange(int value);
    void jointYellowRotationChange(int value);

public slots:
    void setXTranslate(int value);
    void setJointForRedRotation(int angle);
    void setJointForGreenRotation(int angle);
    void setJointForYellowRotation(int angle); 

private:
    void initVertexBufferForLaterUse();
    void initAttributeVariable(GLuint arrtibute, GLint num, GLuint buffer);
    void redraw();
    GLfloat* getMatrixElements(mat4 matrix);

private:
    QOpenGLShaderProgram* program;  
    GLuint aPosition, aColor, uModelMatrix;
    GLuint numRectVertex, rectVerticesBuffer;
    GLuint numColor, redColorBuffer, greenColorBuffer, blueColorBuffer, yellowColorBuffer;

    // use algebra-matrix for 3D Transformation
    mat4 M; 
    stack<mat4> matrixStack;
    static GLfloat transformMatrix[16];

    // store the input from sliders (variables for tx, red, green and yellow arms angle )    
    float tx;
    float redAngle;
    float greenAngle;
    float yellowAngle;
};

#endif // GLWIDGET_H
