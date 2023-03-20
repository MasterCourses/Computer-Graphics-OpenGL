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

private:
    QOpenGLShaderProgram* program;  
    GLuint aPosition, aColor, uModelMatrix;
    GLuint numRectVertex, rectVerticesBuffer;
    GLuint numColor, redColorBuffer, greenColorBuffer, blueColorBuffer, yellowColorBuffer;

    // use algebra-matrix for 3D Transformation
    QMatrix4x4 transformMatrix;
    QVector<QMatrix4x4> matrixStack;

    // store the input from sliders (variables for tx, red, green and yellow arms angle )    
    float tx;
    float redAngle;
    float greenAngle;
    float yellowAngle;
};

#endif // GLWIDGET_H
