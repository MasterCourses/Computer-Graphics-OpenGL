#include "glwidget.h"

const char* vertexShaderSource =
"attribute vec4 aPosition;                                        \n"
"attribute vec4 aColor;                                           \n"
"varying vec4 vColor;                                             \n"
"uniform mat4 uModelMatrix;                                       \n"
"void main()                                                      \n"
"{                                                                \n"
"    gl_Position = uModelMatrix * aPosition;                      \n"
"    vColor = aColor;                                             \n"
"}                                                                \n";

const char* fragmentShaderSource =
"varying vec4 vColor;                                             \n"
"void main()                                                      \n"
"{                                                                \n"
"	gl_FragColor = vColor;                                        \n"
"}                                                                \n";

glWidget::glWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    tx = 0.0;
    redAngle = 0.0;
    greenAngle = 0.0;
    yellowAngle = 0.0;
}

glWidget::~glWidget()
{

}

void glWidget::initializeGL()
{
    // initialize env
    this->initializeOpenGLFunctions(); 
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // set background color is black
 
    // Create and Compile : vertex/frag shader
    QOpenGLShader* vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    vertexShader->compileSourceCode(vertexShaderSource);
    QOpenGLShader* fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    fragmentShader->compileSourceCode(fragmentShaderSource);

    // Create shader program
    program = new QOpenGLShaderProgram(this);
    // Attach shader to program
    program->addShader(vertexShader);
    program->addShader(fragmentShader);
    // link shader to program
    program->link();

}

void glWidget::paintGL()
{
    // Activate shader program
    program->bind();

    /////prepare attribute reference of the shader
    aPosition = glGetAttribLocation(program->programId(), "aPosition");
    aColor = glGetAttribLocation(program->programId(), "aColor");
    uModelMatrix = glGetUniformLocation(program->programId(), "uModelMatrix"); // set the Uniform attribute for Transformation Matrix 

    /////create vertex buffer of rotating point, center points, rotating triangle for later use
    initVertexBufferForLaterUse();

    redraw(); //call redarw here to show the initial image

    // Release shader program
    program->release();
      
}

void glWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    update();
}

QSize glWidget::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize glWidget::sizeHint() const
{
    return QSize(600, 600);
}

//setup the call back function of Translate_X(tx) Sliders
void glWidget::setXTranslate(int value)
{
    tx = value / 100.0;
    emit xTranslateChange(value);
    redraw();
}

//setup the call back function of red arm rotation Sliders
void glWidget::setJointForRedRotation(int angle)
{
    redAngle = angle;
    emit jointRedRotationChange(angle);
    redraw();
}

//setup the call back function of green arm rotation Sliders
void glWidget::setJointForGreenRotation(int angle)
{
    greenAngle = angle;
    emit jointGreenRotationChange(angle);
    redraw();
}

//setup the call back function of yellow arm rotation Sliders
void glWidget::setJointForYellowRotation(int angle)
{
    yellowAngle = angle;
    emit jointYellowRotationChange(angle);
    redraw();
}

void glWidget::initVertexBufferForLaterUse()
{
    // rect vertex location
    numRectVertex = 8;
    GLfloat rectVertices[] = { -0.5, 0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5 };      
    glGenBuffers(1, &rectVerticesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, rectVerticesBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), rectVertices, GL_STATIC_DRAW);
    initAttributeVariable(aPosition, 2, rectVerticesBuffer); //set rect location to shader varibale

    // Color information
    numColor = 12;
    GLfloat redColor[] = { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 }; 
    GLfloat greenColor[] = { 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0 }; 
    GLfloat blueColor[] = { 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0 }; 
    GLfloat yellowColor[] = { 1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0, 0.0 };
    glGenBuffers(1, &redColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, redColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(redColor), redColor, GL_STATIC_DRAW); // red color buffer
    glGenBuffers(1, &greenColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, greenColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(greenColor), greenColor, GL_STATIC_DRAW); // green color buffer
    glGenBuffers(1, &blueColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, blueColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blueColor), blueColor, GL_STATIC_DRAW); // blue color buffer
    initAttributeVariable(aColor, 3, blueColorBuffer); //set rect blue color to shader varibale  .
    glGenBuffers(1, &yellowColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, yellowColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(yellowColor), yellowColor, GL_STATIC_DRAW); // yellow color buffer

}


//Call this funtion when we have to update the screen (eg. user input happens)
void glWidget::redraw()
{
    // Clear glwidget by background color before drawing
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    transformMatrix.setToIdentity(); //set identity matrix to transformMat
    //TODO-1: translate whole robot here
    transformMatrix.translate(QVector3D(tx, 0.0f, 0.0f));
    transformMatrix.translate(QVector3D(0.0f, -0.5f, 0.0f));
    matrixStack.push_back(transformMatrix);
    transformMatrix.scale(QVector3D(1.0f, 0.4f, 0.0f));
    glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, transformMatrix.data()); //pass current transformMat to shader
    glDrawArrays(GL_TRIANGLE_STRIP, 0, numRectVertex / 2); //draw the blue one

    transformMatrix = matrixStack.front();
    matrixStack.pop_back();
    initAttributeVariable(aColor, 3, redColorBuffer); //set rect red color to shader varibale
    //TODO-2: make the red arm rotate
    transformMatrix.translate(QVector3D(0.0f, 0.2f, 0.0f));
    transformMatrix.rotate(redAngle, QVector3D(0.0f, 0.0f, 1.0f)); // rotate(degree, axis)
    transformMatrix.translate(QVector3D(0.0f, 0.5f, 0.0f));
    matrixStack.push_back(transformMatrix);
    transformMatrix.scale(QVector3D(0.2f, 1.2f, 0.0f));    
    glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, transformMatrix.data()); //pass current transformMat to shader
    glDrawArrays(GL_TRIANGLE_STRIP, 0, numRectVertex / 2); //draw the red one       

    transformMatrix = matrixStack.front();
    matrixStack.pop_back();
    initAttributeVariable(aColor, 3, greenColorBuffer); //set rect green color to shader varibale
    //TODO-3: you may add some functions here 
    //        and modify translate() in next line to rotate the green bar
    transformMatrix.translate(QVector3D(0.0f, 0.5f, 0.0f));
    transformMatrix.rotate(greenAngle, QVector3D(0.0f, 0.0f, 1.0f)); // rotate(degree, axis)
    transformMatrix.translate(QVector3D(0.2f, 0.0f, 0.0f));
    matrixStack.push_back(transformMatrix);
    transformMatrix.scale(QVector3D(0.6f, 0.15f, 0.0f));  
    glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, transformMatrix.data()); //pass current transformMat to shader
    glDrawArrays(GL_TRIANGLE_STRIP, 0, numRectVertex / 2); //draw the green one

    //TODO-4: add code here to draw and rotate the yelloe block
    transformMatrix = matrixStack.front();
    matrixStack.pop_back();
    initAttributeVariable(aColor, 3, yellowColorBuffer); //set rect yellow color to shader varibale
    transformMatrix.translate(QVector3D(0.25f, 0.0f, 0.0f));
    transformMatrix.rotate(yellowAngle, QVector3D(0.0f, 0.0f, 1.0f)); // rotate(degree, axis)
    transformMatrix.translate(QVector3D(0.0f, -0.15f, 0.0f));
    transformMatrix.scale(QVector3D(0.1f, 0.45f, 0.0f));
    glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, transformMatrix.data()); //pass current transformMat to shader
    glDrawArrays(GL_TRIANGLE_STRIP, 0, numRectVertex / 2); //draw the green one

    update();
}

void glWidget::initAttributeVariable(GLuint arrtibute, GLint num, GLuint buffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(arrtibute, num, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(arrtibute);
}