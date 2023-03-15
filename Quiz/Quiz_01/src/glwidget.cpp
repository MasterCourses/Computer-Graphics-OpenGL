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
   
    // Clear glwidget by background color before drawing
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);     

    // TODO-3: draw all points in "g_points" one-by-one
    int len = gPoints.size();
    for (int i = 0; i < len; i++) {
        vec2 xy = gPoints[i];
        vec4 rgba = gColors[i];
        glUniform4f(uPosition, xy[0], xy[1], 0.0, 1.0); //TODO: pass position of a point into shader to draw
        glUniform4f(uFragColor, rgba[0], rgba[1], rgba[2], rgba[3]); //TODO: pass color of a point into shader to draw
        glEnable(GL_PROGRAM_POINT_SIZE);
        glDrawArrays(GL_POINTS, 0, 1); //draw a point      
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

void glWidget::mousePressEvent(QMouseEvent* event)
{
    //Todo-1: convert x and y to glwidget space and normal them to (-1, 1) for opengl to use
    QPoint clickPos = event->pos();    
    float x = (float)clickPos.x() / (float)this->width() * 2.0 - 1.0;    
    float y = -((float)clickPos.y() / (float)this->height()) * 2.0 + 1.0;
    //put mouse click position to g_points
    gPoints.push_back(vec2(x, y));
    
    //Todo-2: calculate color of the point
    if (x >= 0 && y > 0) {
        gColors.push_back(vec4(1.0, 0.0, 0.0, 1.0)); //red
    }
    else if (x < 0 && y < 0) {
        gColors.push_back(vec4(0.0, 1.0, 0.0, 1.0)); //green
    }
    else if (x < 0 && y >= 0) {
        gColors.push_back(vec4(0.0, 0.0, 1.0, 1.0)); //blue
    }
    else {
        gColors.push_back(vec4(1.0, 1.0, 1.0, 1.0)); //white
    }

    // Clear glwidget by background color before drawing
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    update(); // refresh screen
}
