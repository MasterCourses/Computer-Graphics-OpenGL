#include "glwidget.h"

const char* vertexShaderSource =
"attribute vec4 aPosition;                                        \n"
"attribute vec4 aColor;                                           \n"
"uniform mat4 uMVPMatrix;                                         \n"
"varying vec4 vColor;                                             \n"
"void main()                                                      \n"
"{                                                                \n"
"    gl_Position = uMVPMatrix * aPosition;                        \n"
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
    setMouseTracking(true);
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
    uMVPMatrix = glGetUniformLocation(program->programId(), "uMVPMatrix"); // set the Uniform attribute for Model Matrix 

    /////create vertex buffer of triangle, color for later use
    initVertexBufferForLaterUse();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST); //enable scissor test to only apply background clear on one viewport

    frontViewMatrix = QMatrix4x4();
    pespProjMatrix = QMatrix4x4();
    frontViewMatrix.lookAt(QVector3D(0, 0, -10), QVector3D(0, 0, 100), QVector3D(0, 1, 0)); // lookAt(eye, center, up)
    pespProjMatrix.perspective(30, this->width() / this->height(), 1, 100); // perspective(verticalAngel, aspectRatio, nearPlane, farPlane)   

    draw();

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

    setCursor(Qt::OpenHandCursor);
    QPoint clickPos = event->pos();
    float x = (float)clickPos.x();
    float y = (float)clickPos.y();
    QRect rect = this->geometry();
    if (rect.left() <= x && x < rect.right() && rect.top() <= y && y < rect.bottom()) {
        mouseLastX = x;
        mouseLastY = y;
        mouseDragging = true;
    }
}

void glWidget::mouseReleaseEvent(QMouseEvent* event)
{
    mouseDragging = false;
    setCursor(Qt::ArrowCursor);

}

void glWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPoint clickPos = event->pos();

    float x = (float)clickPos.x();
    float y = (float)clickPos.y();
    if (mouseDragging) {
        setCursor(Qt::ClosedHandCursor);
        float dx = (x - mouseLastX) / 10.0 * M_PI;
        float dy = (y - mouseLastY) / 10.0 * M_PI;

        angleX += dx; //yes, x for y, y for x, this is right
        angleY += dy;
    }
    mouseLastX = x;
    mouseLastY = y;

    draw();       

}

void glWidget::initVertexBufferForLaterUse()
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////The folloing three function is for creating vertex buffer, but link to shader to user later//////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    numVertices = 9;

    GLfloat  vertices[] = { //9 vertices (three triangles)
            0.0, 1.0, -2.0, //x, y, z of the 1st vertex of the 1st triangle
            -0.5, -1.0, -2.0,
            0.5, -1.0, -2.0,

            0.0, 1.0, -0.0,
            -0.5, -1.0, -0.0,
            0.5, -1.0, -0.0,

            0.0, 1.0, 2.0,
            -0.5, -1.0, 2.0,
            0.5, -1.0, 2.0,
    };

    GLfloat colors[] = {   //9 vertices (three triangles)'s color
           0.7, 0.0, 0.0, //r, g, b of the 1st vertex of the 1st triangle
           0.7, 0.0, 0.0,
           0.7, 0.0, 0.0,

           0.0, 0.7, 0.0,
           0.0, 0.7, 0.0,
           0.0, 0.7, 0.0,

           0.0, 0.0, 0.7,
           0.0, 0.0, 0.7,
           0.0, 0.0, 0.7,
    };

    // initArrayBufferForLaterUse
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

}

void glWidget::initAttributeVariable(GLuint arrtibute, GLint num, GLuint buffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(arrtibute, num, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(arrtibute);
}


void glWidget::draw()
{
    //call drawOneViewPort three times to draw the three views
    modelMatrix1.setToIdentity();
    modelMatrix1.rotate(-angleY, 1, 0, 0); // rotate(degree, axis)
    modelMatrix1.rotate(angleX, 0, 1, 0);
    modelMatrix1.translate(QVector3D(0.75f, 0.0f, 0.0f));

    //this only draw one set of triangles because we pass "QMatrix4x4()" for the last argument
    drawOneViewport(0, 0, this->width() / 2 , this->height() / 2 , //viewportX, viewportY, viewportWidth, viewportHeight,
        0, 0, 0, // bgColorR, bgColorG, bgColorB,
        pespProjMatrix, frontViewMatrix, modelMatrix1, QMatrix4x4());  // projMatrix, viewMatrix, modelMatrixTriangleSet1, modelMatrixTriangleSet2
}

void glWidget::drawOneViewport(float viewportX, float viewportY, float viewportWidth, float viewportHeight, 
                               float bgColorR, float bgColorG, float bgColorB, 
                               const QMatrix4x4 projMatrix, const QMatrix4x4 viewMatrix, const QMatrix4x4 modelMatrixTriangleSet1, const QMatrix4x4 modelMatrixTriangleSet2)
{
    glViewport(viewportX, viewportY, viewportWidth, viewportHeight);
    glScissor(viewportX, viewportY, viewportWidth, viewportHeight); 
    //scissor: make the background clear only apply to this region
     
    ////clear background color and depth buffer
    glClearColor(bgColorR, bgColorG, bgColorB, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    //draw a set of triangles
    transformMatrix = projMatrix * viewMatrix * modelMatrixTriangleSet1;
   
    initAttributeVariable(aPosition, 3, vertexBuffer);
    initAttributeVariable(aColor, 3, colorBuffer);
    glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, transformMatrix.data()); //pass current model-view-projection Matrix to shader
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
  
    if (modelMatrixTriangleSet2 != QMatrix4x4()) { //if we have the second modelMatrix
        //draw the second set of triangles
        transformMatrix = projMatrix * viewMatrix * modelMatrixTriangleSet2;

        initAttributeVariable(aPosition, 3, vertexBuffer);
        initAttributeVariable(aColor, 3, colorBuffer);
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, transformMatrix.data()); //pass current model-view-projection Matrix to shader
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
    }

    update();

}


