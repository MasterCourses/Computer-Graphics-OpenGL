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

    ////For creating animation, in short this code segment will keep calling "draw(gl)" 
    draw();

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
    glGenBuffers(1, &redColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, redColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(redColor), redColor, GL_STATIC_DRAW); // red color buffer
    glGenBuffers(1, &greenColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, greenColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(greenColor), greenColor, GL_STATIC_DRAW); // green color buffer
    glGenBuffers(1, &blueColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, blueColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blueColor), blueColor, GL_STATIC_DRAW); // blue color buffer
    initAttributeVariable(aColor, 3, blueColorBuffer); //set rect blue color to shader varibale  
}

void glWidget::draw()
{
    // Clear glwidget by background color before drawing
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    transformMatrix.setToIdentity();
    transformMatrix.translate(QVector3D(0.0f, -0.5f, 0.0f));
    matrixStack.push_back(transformMatrix);
    transformMatrix.scale(QVector3D(1.0f, 0.4f, 0.0f));    
    glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, transformMatrix.data()); //pass current transformMat to shader
    glDrawArrays(GL_TRIANGLE_STRIP, 0, numRectVertex / 2);

    transformMatrix = matrixStack.front();
    matrixStack.pop_back();    
    initAttributeVariable(aColor, 3, redColorBuffer); //set rect red color to shader varibale
    transformMatrix.translate(QVector3D(0.0f, 0.2f, 0.0f));
    transformMatrix.rotate(-20, QVector3D(0.0f, 0.0f, 1.0f)); // rotate(degree, axis)
    transformMatrix.translate(QVector3D(0.0f, 0.5f, 0.0f));
    matrixStack.push_back(transformMatrix);
    transformMatrix.scale(QVector3D(0.2f, 1.2f, 0.0f));   
    glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, transformMatrix.data()); //pass current transformMat to shader
    glDrawArrays(GL_TRIANGLE_STRIP, 0, numRectVertex / 2);      

    transformMatrix = matrixStack.front();
    matrixStack.pop_back();
    initAttributeVariable(aColor, 3, greenColorBuffer); //set rect green color to shader varibale
    transformMatrix.translate(QVector3D(0.2f, 0.5f, 0.0f));
    transformMatrix.scale(QVector3D(0.6f, 0.15f, 0.0f));
    glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, transformMatrix.data()); //pass current transformMat to shader
    glDrawArrays(GL_TRIANGLE_STRIP, 0, numRectVertex / 2);
    update();
}

void glWidget::initAttributeVariable(GLuint arrtibute, GLint num, GLuint buffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(arrtibute, num, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(arrtibute);
}
