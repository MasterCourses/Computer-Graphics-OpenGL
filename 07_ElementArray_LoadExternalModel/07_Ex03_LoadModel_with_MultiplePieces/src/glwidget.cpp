#include "glwidget.h"

const char* vertexShaderSource =
"attribute vec4 aPosition;                                                                                              \n"
"attribute vec4 aNormal;                                                                                                \n"
"uniform mat4 uMVPMatrix;                                                                                               \n"
"uniform mat4 uModelMatrix;                                                                                             \n"
"uniform mat4 uNormalMatrix;                                                                                            \n" // correct normal matrix
"varying vec3 vNormal;                                                                                                  \n" // pass
"varying vec3 vPositionInWorld;                                                                                         \n" // pass
"void main(){                                                                                                           \n"
"    gl_Position = uMVPMatrix * aPosition;                                                                              \n"
"    vPositionInWorld = (uModelMatrix * aPosition).xyz;                                                                 \n"
"    vNormal = normalize((uNormalMatrix * aNormal).xyz);                                                                \n"  // N : surface normal vector (use correct normal matrix)
"}                                                                                                                      \n";

const char* fragmentShaderSource =
"uniform vec3 uLightPosition;                                                                                           \n" // for diffuse
"uniform vec3 uViewPosition;                                                                                            \n" // for specular
"uniform float uKa;                                                                                                     \n" // ambient reflection coefficient  (Ka)
"uniform float uKd;                                                                                                     \n" // diffuse reflection coefficient  (Kd)
"uniform float uKs;                                                                                                     \n" // specular reflection coefficient (Ks)
"uniform float uShininess;                                                                                              \n" // specular shininess
"uniform vec3 uColor;                                                                                                   \n" // pass
"varying vec3 vNormal;                                                                                                  \n" // pass
"varying vec3 vPositionInWorld;                                                                                         \n" // pass
"void main()                                                                                                            \n"
"{                                                                                                                      \n"
"    // let ambient and diffuse color are vColor                                                                        \n"
"    // (you can also input them from ouside and make it different)                                                     \n"
"    vec3 ambientLightColor = uColor;                                                                                   \n" // Ia : ambient light intensity
"    vec3 diffuseLightColor = uColor;                                                                                   \n" // Id : diffuse light intensity
"    // assume white specular light (you can also input it from ouside)                                                 \n"
"    vec3 specularLightColor = vec3(1.0, 1.0, 1.0);                                                                     \n" // Is : specular light intensity
"                                                                                                                       \n"
"    vec3 ambient = ambientLightColor * uKa;                                                                            \n" // Ambient = 𝑰𝒂 * 𝑲𝒂
"                                                                                                                       \n"
"                                                                                                                       \n" // normalize the v_Normal before using it,
"    vec3 normal = normalize(vNormal);                                                                                  \n" // before it comes from normal vectors interpolation
"                                                                                                                       \n"
"    vec3 lightDirection = normalize(uLightPosition - vPositionInWorld);                                                \n"  // L : for diffuse light calculation
"    float nDotL = max(dot(lightDirection, normal), 0.0);                                                               \n"  // cos(θ) = L • N
"    vec3 diffuse = diffuseLightColor * uKd * nDotL;                                                                    \n"  // 𝐷𝑖𝑓𝑓𝑢𝑠𝑒 = 𝐾𝑑 ∗ 𝐼𝑑 ∗ cos(𝜃)  
"                                                                                                                       \n"
"                                                                                                                       \n"
"    vec3 specular = vec3(0.0, 0.0, 0.0);                                                                               \n"
"    if(nDotL > 0.0) {                                                                                                  \n"
"       vec3 R = reflect(-lightDirection, normal);                                                                      \n"
"       // V : the vector, point to viewer                                                                              \n"
"       vec3 V = normalize(uViewPosition - vPositionInWorld);                                                           \n"
"       float specAngle = clamp(dot(R, V), 0.0, 1.0);                                                                   \n"  // cos𝜙 = 𝑅∙𝑉 : angle between V and R
"       specular = specularLightColor * uKs * pow(specAngle, uShininess);                                               \n"  // 𝑆𝑝𝑒𝑐𝑢𝑙𝑎𝑟 = 𝐾𝑠 ∗ 𝐼𝑠 ∗ cos𝑛(𝜙)
"    }                                                                                                                  \n"
"                                                                                                                       \n"
"                                                                                                                       \n"
"	gl_FragColor = vec4( ambient + diffuse + specular, 1.0);                                                            \n"
"}                                                                                                                      \n";

glWidget::glWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setMouseTracking(true);
}

glWidget::~glWidget()
{

}

void glWidget::initsize(QString filename)
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

    // Obj Loader
    OBJModel obj("E:\\models\\obj\\sonic.obj"); // given absolute path on Windows
    modelObj = obj.ToIndexedModel();
}

void glWidget::paintGL()
{
    // Activate shader program
    program->bind();

    /////prepare attribute reference of the shader
    aPosition = glGetAttribLocation(program->programId(), "aPosition");
    aNormal = glGetAttribLocation(program->programId(), "aNormal");

    uMVPMatrix = glGetUniformLocation(program->programId(), "uMVPMatrix"); // set the Uniform attribute for MVP Matrix 
    uModelMatrix = glGetUniformLocation(program->programId(), "uModelMatrix"); // set the Uniform attribute for Model Matrix 
    uNormalMatrix = glGetUniformLocation(program->programId(), "uNormalMatrix"); // set the Uniform attribute for Normal Matrix 
    uLightPosition = glGetUniformLocation(program->programId(), "uLightPosition"); // set the Uniform attribute for Light Position
    uViewPosition = glGetUniformLocation(program->programId(), "uViewPosition"); // set the Uniform attribute for View Position (for specular use)

    uKa = glGetUniformLocation(program->programId(), "uKa"); // set the Uniform attribute for ambient reflection coefficient Ka (0 <= Ka <= 1) 
    uKd = glGetUniformLocation(program->programId(), "uKd"); // set the Uniform attribute for diffuse reflection coefficient Kd (0 <= Kd <= 1) 
    uKs = glGetUniformLocation(program->programId(), "uKs"); // set the Uniform attribute for specular reflection coefficient Kd (0 <= Ks <= 1) 
    uShininess = glGetUniformLocation(program->programId(), "uShininess"); // set the Uniform attribute for specular shininess 
    uColor = glGetUniformLocation(program->programId(), "uColor"); // set the Uniform attribute for color

   
    /////create vertex buffer of triangle, color for later use
    numIndices = initVertexBuffer(modelObj);

    modelMatrix = QMatrix4x4();
    mvpMatrix = QMatrix4x4();  
    normalMatrix = QMatrix4x4();

    glEnable(GL_DEPTH_TEST);

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

GLuint glWidget::initVertexBuffer(const IndexedModel& model)
{   
    
    // initArrayBuffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);
    initArrayBuffer(aPosition, 3, vertexBuffer);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
    initArrayBuffer(aNormal, 3, normalBuffer);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

    return model.indices.size();
}

void glWidget::initArrayBuffer(GLuint arrtibute, GLint num, GLuint buffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(arrtibute, num, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(arrtibute);
}


void glWidget::draw()
{   
    //model Matrix (part of the mvp matrix)
    modelMatrix.setToIdentity();
    modelMatrix.rotate(-angleY, 1, 0, 0); //for mouse rotation
    modelMatrix.rotate(angleX, 0, 1, 0); //for mouse rotation
    modelMatrix.scale(objScale, objScale, objScale);
    modelMatrix.translate(0.0, -15.0, 0.0);
    //mvp: projection * view * model matrix  
    mvpMatrix.setToIdentity(); //identity matrix
    mvpMatrix.perspective(30, 1, 1, 100); //Projection matrix
    mvpMatrix.lookAt(QVector3D(cameraX, cameraY, cameraZ), QVector3D(0, 0, 0), QVector3D(0, 1, 0)); //Projection matrix * View matrix
    mvpMatrix *= modelMatrix;

    // normal matrix
    normalMatrix = modelMatrix.inverted();
    normalMatrix.transposed();

    glUniform3f(uLightPosition, 0.0, 5.0, 3.0);
    glUniform3f(uViewPosition, cameraX, cameraY, cameraZ);

    glUniform1f(uKa, 0.2f);
    glUniform1f(uKd, 0.7f);
    glUniform1f(uKs, 1.0f);
    glUniform1f(uShininess, 3.0f);
    glUniform3f(uColor, 1.0f, 0.4f, 0.4f);

    glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, mvpMatrix.data()); //pass current model-view-projection Matrix to shader
    glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, modelMatrix.data()); //pass current model-view-projection Matrix to shader
    glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, normalMatrix.data()); //pass current model-view-projection Matrix to shader
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);    

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

    update();
}


