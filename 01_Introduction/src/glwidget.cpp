#include "glwidget.h"

const char* vertexShaderSource =
"uniform highp vec4 uPosition;                                    \n"
"void main()                                                      \n"
"{                                                                \n"
"    gl_Position = uPosition;                                     \n"
"    gl_PointSize = 10.0;                                         \n"
"}                                                                \n";

const char* fragmentShaderSource =
"uniform lowp vec4 uFragColor;                                    \n"                                   
"void main()                                                      \n"
"{                                                                \n"
"	gl_FragColor = uFragColor;                                    \n"
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

    // get reference
    GLuint uPosition = glGetUniformLocation(program->programId(), "uPosition");
    GLuint uFragColor = glGetUniformLocation(program->programId(), "uFragColor");
   
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
     
    for (int i = 0; i < 10; i++)
    {      
        // pass data to a uniform vec4 variable in GLSL shader
        glUniform4f(uPosition, (float)rand() / RAND_MAX * 2.0 - 1.0, (float)rand() / RAND_MAX * 2.0 - 1.0, 0.0, 1.0); 
        glUniform4f(uFragColor, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 1.0);                      
        // draw
        glEnable(GL_PROGRAM_POINT_SIZE);
        glDrawArrays(GL_POINTS, 0, 1);        
    }

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
