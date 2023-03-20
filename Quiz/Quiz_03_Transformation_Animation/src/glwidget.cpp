#include "glwidget.h"

const char* vertexShaderSource =
"attribute vec4 aPosition;                                        \n"
"attribute vec4 aColor;                                           \n"
"varying vec4 vColor;                                             \n"
"uniform mat4 uModelMatrix;                                       \n"
"void main()                                                      \n"
"{                                                                \n"
"    gl_Position = uModelMatrix * aPosition;                      \n"
"    gl_PointSize = 10.0;                                         \n"
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

    //NOTE: You are NOT allowed to change the vertex information here
    numCenterPoint = 3;
    GLfloat centerPointLoc[] = { 0.0, 0.0, 0.0 }; //center white point location
    GLfloat centerPointColor[] = { 1.0, 1.0, 1.0 }; //center white point color

    // initArrayBufferForLaterUse
    glGenBuffers(1, &vertCenterPointBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertCenterPointBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(centerPointLoc), centerPointLoc, GL_STATIC_DRAW);
    glGenBuffers(1, &colCenterPointBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colCenterPointBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(centerPointColor), centerPointColor, GL_STATIC_DRAW);

    //NOTE: You are NOT allowed to change the vertex information here
    numRotatingPoint = 3;
    GLfloat rotatingPointLoc[] = { 0.0, 0.0, 0.0 }; //rotating red point location 
    GLfloat rotatingPointColor[] = { 1.0, 0.0, 0.0 }; //rotating red point color 

    // initArrayBufferForLaterUse
    glGenBuffers(1, &vertRotatingPointBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertRotatingPointBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rotatingPointLoc), rotatingPointLoc, GL_STATIC_DRAW);
    glGenBuffers(1, &colRotatingPointBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colRotatingPointBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rotatingPointColor), rotatingPointColor, GL_STATIC_DRAW);

    //NOTE: You are NOT allowed to change the vertex information here
    numTriangle = 9;
    GLfloat triangleVertices[] = { 0.0, 0.2, 0.0, -0.1, -0.3, 0.0, 0.1, -0.3, 0.0 }; //green rotating triangle vertices
    GLfloat triangleColor[] = { 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0 }; //green trotating riangle color
    
    // initArrayBufferForLaterUse
    glGenBuffers(1, &vertTriangleBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertTriangleBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    glGenBuffers(1, &colTriangleBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colTriangleBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColor), triangleColor, GL_STATIC_DRAW);

}

void glWidget::draw()
{
    // Clear glwidget by background color before drawing
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ////////////// Begin: draw the rotating green triangle
    transformMatrix.setToIdentity(); //set identity matrix to transformMat
    ///******TODO: you can multiple transformMat.translate() and transformMat.rotate() to make the rotating grenn triangle
    triangleAngle++;
    transformMatrix.rotate(pointAngle, QVector3D(0.0f, 0.0f, 1.0f)); // rotate(degree, axis)
    transformMatrix.translate(QVector3D(0.0f, 0.4f, 0.0f));
    transformMatrix.rotate(triangleAngle, QVector3D(0.0f, 0.0f, 1.0f)); // rotate(degree, axis)
    transformMatrix.translate(QVector3D(0.0f, -0.2f, 0.0f));
    //Note: You are NOT Allowed to change the following code
    initAttributeVariable(aPosition, 3, vertTriangleBuffer); //set triangle  vertex to shader varibale
    initAttributeVariable(aColor, 3, colTriangleBuffer); //set triangle  color to shader varibale
    glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, transformMatrix.data()); //pass current transformMat to shader
    glDrawArrays(GL_TRIANGLES, 0, numTriangle / 3);
    ////////////// END: draw the rotating green triangle
    
    //// Begin: draw the center white point
    transformMatrix.setToIdentity(); //set identity matrix to transformMat  
    initAttributeVariable(aPosition, 3, vertCenterPointBuffer); //set center point vertex to shader varibale
    initAttributeVariable(aColor, 3, colCenterPointBuffer); //set center point color into shader varibale
    glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, transformMatrix.data()); //pass current transformMat to shader
    glEnable(GL_PROGRAM_POINT_SIZE);
    glDrawArrays(GL_POINTS, 0, numCenterPoint / 3);
    //// END: draw the center white point

    /////////******Suggestion: read the following code and understand what's going on here (about the transofmation) 
    //// Begin: draw the rotating red point
    pointAngle++; //rotating angle of the red point
    transformMatrix.setToIdentity(); //set identity matrix to transformMat
    transformMatrix.rotate(pointAngle, QVector3D(0.0f, 0.0f, 1.0f)); // rotate(degree, axis)
    transformMatrix.translate(QVector3D(0.0f, 0.4f, 0.0f));
    initAttributeVariable(aPosition, 3, vertRotatingPointBuffer); //set rotating point vertex to shader varibale
    initAttributeVariable(aColor, 3, colRotatingPointBuffer); //set rotating point color to shader varibale
    glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, transformMatrix.data()); //pass current transformMat to shader
    glEnable(GL_PROGRAM_POINT_SIZE);
    glDrawArrays(GL_POINTS, 0, numRotatingPoint / 3);
    //// END: draw the rotating red point

    update();
    
}

void glWidget::initAttributeVariable(GLuint arrtibute, GLint num, GLuint buffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(arrtibute, num, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(arrtibute);
}


