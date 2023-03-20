#include "glwidget.h"

const char* vertexShaderSource =
"attribute vec4 aPosition;                                                          \n"
"attribute vec4 aColor;                                                             \n"
"uniform mat4 uModelMatrix;                                                         \n"
"uniform mat4 uViewMatrix;                                                          \n"
"uniform mat4 uProjMatrix;                                                          \n"
"varying vec4 vColor;                                                               \n"
"void main()                                                                        \n"
"{                                                                                  \n"
"    gl_Position =  uProjMatrix * uViewMatrix * uModelMatrix * aPosition;           \n" 
"    vColor = aColor;                                                               \n"
"}                                                                                  \n";

const char* fragmentShaderSource =
"varying vec4 vColor;                                                               \n"
"void main()                                                                        \n"
"{                                                                                  \n"
"	gl_FragColor = vColor;                                                          \n"
"}                                                                                  \n";

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
    // Clear glwidget by background color before drawing
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    resizeGL(this->width(), this->height());

    // Activate shader program
    program->bind();

    // initialize vbo buffer
    int n = initVertexBuffers();

    /*set model transformations*/
    QVector<QMatrix4x4> modelTransforms;
    QMatrix4x4 modelMatrix, viewMatrix, projMatrix;

    GLuint uModelMatrix = glGetUniformLocation(program->programId(), "uModelMatrix"); // set the Uniform attribute for Model Matrix  
    GLuint uViewMatrix = glGetUniformLocation(program->programId(), "uViewMatrix"); // set the Uniform attribute for View Matrix 
    GLuint uProjMatrix = glGetUniformLocation(program->programId(), "uProjMatrix"); // set the Uniform attribute for Projection Matrix 

    viewMatrix.lookAt(QVector3D(0, 0, -10), QVector3D(0, 0, 100), QVector3D(0, 1, 0)); // lookAt(eye, center, up)
    glUniformMatrix4fv(uViewMatrix, 1, GL_FALSE, viewMatrix.data()); //pass current View Matrix to shader

    // Orthographic Projection (View to Clip Space)
    //projMatrix.ortho(-1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f); // ortho(left, right, bottom, up, nearPlane, farPlane)
    projMatrix.perspective(30, this->width() / this->height(), 1, 100); // verticalAngel, aspectRatio, nearPlane, farPlane
    glUniformMatrix4fv(uProjMatrix, 1, GL_FALSE, projMatrix.data()); //pass current Projection Matrix to shader

    modelTransforms.push_back(modelMatrix);
    modelMatrix.translate(0.75, 0.0, 0.0);
    glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, modelMatrix.data()); //pass current Model Matrix to shader
    glDrawArrays(GL_TRIANGLES, 0, n);

    modelMatrix = modelTransforms.front();
    modelTransforms.pop_back();
    modelMatrix.translate(-0.75, 0.0, 0.0);
    glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, modelMatrix.data()); //pass current Model Matrix to shader
    glDrawArrays(GL_TRIANGLES, 0, n);

    // Release shader program
    program->release();
      
}

void glWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    //glViewport(100, 100, w , h);

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
    int n = 9; // draw 9 vertices
    GLfloat vertices[] = { //9 vertices (three triangles)  
            0.0, 1.0, -4.0, 0.7, 0.0, 0.0, //x, y, z, r, g, b of the first vertex
            -0.5, -1.0, -4.0, 0.7, 0.0, 0.0,
            0.5, -1.0, -4.0, 0.7, 0.0, 0.0,

            0.0, 1.0, -2.0, 0.0, 0.7, 0.0,
            -0.5, -1.0, -2.0, 0.0, 0.7, 0.0,
            0.5, -1.0, -2.0, 0.0, 0.7, 0.0,

            0.0, 1.0, 0.0, 0.0, 0.0, 0.7,
            -0.5, -1.0, 0.0, 0.0, 0.0, 0.7,
            0.5, -1.0, 0.0, 0.0, 0.0, 0.7,
    };     
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GLuint aPosition = glGetAttribLocation(program->programId(), "aPosition");
    glVertexAttribPointer(aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
    glEnableVertexAttribArray(aPosition);

    GLuint aColor = glGetAttribLocation(program->programId(), "aColor");
    glVertexAttribPointer(aColor, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(aColor);

    return n;
}
