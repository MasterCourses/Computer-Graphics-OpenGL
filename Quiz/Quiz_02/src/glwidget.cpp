#include "glwidget.h"

const char* vertexShaderSource =
"attribute vec4 aPosition;                                        \n"
"attribute vec4 aColor;                                           \n"
"varying vec4 vColor;                                             \n"
"void main()                                                      \n"
"{                                                                \n"
"    gl_Position = aPosition;                                     \n"
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

    //TODO - 1~5:
    int n = initVertexBuffers();  // initialize vbo buffer
    
    //TODO-6: get reference of the attribute variable for vertex position
    //TODO-7: layout of current vertex buffer object (gl.vertexAttribPointer)
    //TODO-8: enable the attribute array        
    GLuint aPosition = glGetAttribLocation(program->programId(), "aPosition"); // set the vertex attribute
    glVertexAttribPointer(aPosition, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)0);
    glEnableVertexAttribArray(aPosition);
    GLuint aColor = glGetAttribLocation(program->programId(), "aColor");
    glVertexAttribPointer(aColor, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(sizeof(GLfloat) * 2)); // set the color attribute
    glEnableVertexAttribArray(aColor);
     
    // Clear glwidget by background color before drawing
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //TODO-9 repeat TODO-6~8 for the attribute variable to store vertex color information
    glEnable(GL_PROGRAM_POINT_SIZE);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, n); // draw

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

    //TODO-1: vertex and color array
    GLfloat vertices[] = {
        // position(x, y)   // color-(R,G,B)
        -0.5, 0.5,          1.0, 0.0, 0.0,
        -0.5, -0.5,         1.0, 1.0, 1.0,
        0.5, 0.5,           1.0, 1.0, 1.0,
        0.5, -0.5,          0.0, 0.0, 1.0
    };

    //TODO-2: how many vertices to draw?
    int n = 4; // draw n vertices

    //TODO-3: create a vertex buffer
    glGenBuffers(1, &vertexBuffer); // creat the vertex buffer object
    //TODO-4: bind buffer (gl.bindBuffer)
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);    // copy data into the vertexBuffer(vbo)
    //TODO-5: bind buffer data (gl.bufferData)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    return n;
}
