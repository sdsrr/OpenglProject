#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"

GLShaderManager glShaderMgr;
ShaderMgr shaderMgr;
NormalCamera normalCamera;
GLMatrixStack* modelviewStack;
BaseShaderParam param;

GLfloat angle = 10;
GLBatch triangle;
GLfloat color[] = {1,1,0,1};

GLuint fbo,rbo;

void Display(void)
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelviewStack->PushMatrix();
    modelviewStack->Translate(0, 0, -5);
    modelviewStack->Rotate(angle, 1, 1, 0);
    param.SetDiffuseColor(color);
    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix());
    param.SetMVMatrix(normalCamera.GetModelviewMatrix());
    param.SetNormalMatrix(normalCamera.GetNormalMatrix());
    shaderMgr.UseDiffuse(param);
    triangle.Draw();
    modelviewStack->PopMatrix();

    glutSwapBuffers();
}

void OnStartUp()
{
    //init fbo,rbo
    glGenFramebuffers(1, &fbo);

    //init triangle
    triangle.Begin(GL_TRIANGLE_STRIP,6,1);
    triangle.MultiTexCoord2f(0,0,0);
    triangle.Vertex3f(0,0,0);
    triangle.MultiTexCoord2f(0,1,0);
    triangle.Vertex3f(1,0,0);
    triangle.MultiTexCoord2f(0,0,1);
    triangle.Vertex3f(0,1,0);
    triangle.MultiTexCoord2f(0,1,1);
    triangle.Vertex3f(1,1,0);
    triangle.End();

    //init shadermgr
    glShaderMgr.InitializeStockShaders();
    shaderMgr.OnInit();
    normalCamera.OnInit(640, 480, 50, 1, 2);
    modelviewStack = normalCamera.GetModelviewStack();
}

void OnShutUp()
{
    glDeleteFramebuffers(1, &fbo);
    shaderMgr.OnUnInit();
    normalCamera.OnUnInit();
}

void RoateGameobject(unsigned char key, int x, int y)
{
    if (key == 'z')
        angle += 10;
    else if (key == 'x')
        angle -= 10;
}

void Idle(void) {glutPostRedisplay();}
void MouseClick(int button, int action, int x, int y) {normalCamera.MouseClick(button, action, x, y);}
void MotionFunc(int mouse_x, int mouse_y) {normalCamera.MotionFunc(mouse_x, mouse_y);}
void Resize(int w, int h) {normalCamera.Resize(w, h);}
void KeyboardFn(unsigned char key, int x, int y)
{
    normalCamera.KeyboardFn(key, x, y);
    RoateGameobject(key, x, y);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutCreateWindow("msaa");

    glutKeyboardFunc(KeyboardFn);
    glutReshapeFunc(Resize);
    glutMotionFunc(MotionFunc);
    glutMouseFunc(MouseClick);
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);

    //使用混合方式
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_POLYGON_SMOOTH);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    //使用多重采样
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    if (glewInit() != GLEW_OK)
    {
        printf("glew init error...\n");
        return 0;
    }

    OnStartUp();
    glutMainLoop();
    OnShutUp();

    return EXIT_SUCCESS;
}
