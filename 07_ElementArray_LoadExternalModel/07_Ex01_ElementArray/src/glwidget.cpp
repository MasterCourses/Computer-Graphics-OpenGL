#include "glwidget.h"

const char* vertexShaderSource =
"attribute vec4 aPosition;                                              \n"
"attribute vec4 aColor;                                                 \n"
"uniform mat4 uMVPMatrix;                                               \n"
"varying vec4 vColor;                                                   \n"
"void main()                                                            \n"
"{                                                                      \n"
"    gl_Position = uMVPMatrix * aPosition;                              \n"
"    vColor = aColor;                                                   \n"
"}                                                                      \n";

const char* fragmentShaderSource =
"varying vec4 vColor;                                                   \n"
"void main()                                                            \n"
"{                                                                      \n"
"	gl_FragColor = vColor;                                              \n"
"}                                                                      \n";

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
    uMVPMatrix = glGetUniformLocation(program->programId(), "uMVPMatrix"); // set the Uniform attribute for MVP Matrix 

    /////create vertex buffer of triangle, color for later use
    initVertexBuffer();

    glEnable(GL_DEPTH_TEST);

    modelMatrix = QMatrix4x4();
    mvpMatrix = QMatrix4x4();    
    
    draw();

    // Release shader program
    program->release();
      
}

void glWidget::resizeGL(int w, int h)
{
    glViewport(-w / 2, w / 2, h / 2, -h / 2);   
}

QSize glWidget::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize glWidget::sizeHint() const
{
    return QSize(800, 800);
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

void glWidget::initVertexBuffer()
{
    num = 6 * 6; // indices length

    GLfloat  vertices[] = { // cube
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            -1.0, 1.0, 1.0, 1.0, 0.0, 1.0,
            -1.0, -1.0, 1.0, 1.0, 0.0, 0.0,
            1.0, -1.0, 1.0, 1.0, 1.0, 0.0,
            1.0, -1.0, -1.0, 0.0, 1.0, 0.0,
            1.0, 1.0, -1.0, 0.0, 1.0, 1.0,
            -1.0, 1.0, -1.0, 0.0, 0.0, 1.0,
            -1.0, -1.0, -1.0, 0.0, 0.0, 0.0
    };

    GLubyte indices[] = {   //cube's indices
            0, 1, 2, 0, 2, 3,  //front
            0, 3, 4, 0, 4, 5, //right
            0, 5, 6, 0, 6, 1,  //up
            1, 6, 7, 1, 7, 2,  //left
            7, 4, 3, 7, 3, 2,  //bottom
            4, 7, 6, 4, 6, 5   //back
    };


    // initArrayBuffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)0);
    glEnableVertexAttribArray(aPosition);

    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(aColor, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(aColor);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ARRAY_BUFFER, NULL);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

}


void glWidget::draw()
{   
    //model Matrix (part of the mvp matrix)
    modelMatrix.setToIdentity();
    modelMatrix.rotate(-angleY, 1, 0, 0); //for mouse rotation
    modelMatrix.rotate(angleX, 0, 1, 0); //for mouse rotation
    //mvp: projection * view * model matrix  
    mvpMatrix.setToIdentity(); //identity matrix
    mvpMatrix.perspective(30, 1, 1, 100); //Projection matrix
    mvpMatrix.lookAt(QVector3D(3, 3, 7), QVector3D(0, 0, 0), QVector3D(0, 1, 0)); //Projection matrix * View matrix
    mvpMatrix *= modelMatrix;

    glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, mvpMatrix.data()); //pass current model-view-projection Matrix to shader
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);    
    glDrawElements(GL_TRIANGLES, num, GL_UNSIGNED_BYTE, 0);
    update();
}


