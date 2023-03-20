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

    // initialize vbo buffer
    int triVertexNums = initVertexBuffers(); // for vextex & color buffer

    // set the Uniform attribute for Transformation Matrix 
    GLuint uModelMatrix = glGetUniformLocation(program->programId(), "uModelMatrix");
    
    // use qt-opengl framework for 3D Transformation   
    transformMatrix.rotate(45, QVector3D(0.0f, 0.0f, 1.0f)); // rotate(degree, axis)
    transformMatrix.setToIdentity();
    transformMatrix.translate(QVector3D(0.0f, 0.5f, 0.0f));
    transformMatrix.scale(QVector3D(2.0f, 3.5f, 1.0f));
    //pass the transformation matrix to shader
    glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, transformMatrix.data());

    // Clear glwidget by background color before drawing
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw
    glEnable(GL_PROGRAM_POINT_SIZE);
    glDrawArrays(GL_TRIANGLES, 0, triVertexNums / 2);

    // Release shader program
    program->release();
}

void glWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

QSize glWidget::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize glWidget::sizeHint() const
{
    return QSize(600, 600);
}

int glWidget::initVertexBuffers()
{
    int n = 6;  // triangle vertex numbers

    GLfloat triangleVertices[] = { 0.0, 0.5, -0.3, -0.5, 0.3, -0.5 }; // define the triangle in object space       
    // creat and copy data into the vertexBuffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    // set the vertex attribute
    setAttributeBuffer(2, "aPosition");

    GLfloat triangleColor[] = { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };  // red triangle  
    // creat and copy data into the colorBuffer
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColor), triangleColor, GL_STATIC_DRAW);
    // set the color attribute
    setAttributeBuffer(3, "aColor");

    return n;
}

void glWidget::setAttributeBuffer(GLint num, GLchar* name) 
{
    GLuint aAttribute = glGetAttribLocation(program->programId(), name);
    glVertexAttribPointer(aAttribute, num, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(aAttribute);
}
