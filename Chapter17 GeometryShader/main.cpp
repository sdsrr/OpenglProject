#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"

BaseShaderParam param;
GLShaderManager glShaderMgr;
ShaderMgr shaderMgr;
NormalCamera normalCamera;
GLMatrixStack* modelviewStack;

GLfloat angle = 0;
GLuint texture;
GLBatch triangle;
GLfloat diffuseColor[] = {0,1,1,0};

void Display(void)
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelviewStack->PushMatrix();
    modelviewStack->Translate(0, 0, -10);
    modelviewStack->Rotate(angle+=2, 1, 1, 0);
        param.SetMVMatrix(normalCamera.GetModelviewMatrix());
        param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix());
        param.SetNormalMatrix(normalCamera.GetNormalMatrix());
        param.SetDiffuseColor(diffuseColor);
        param.colorMap[0] = 0;
        shaderMgr.UseTexture2d(param);
        triangle.Draw();
        shaderMgr.DrawNormal(param, 1.0f);
        triangle.Draw();
    modelviewStack->PopMatrix();

    glutSwapBuffers();
}

void OnStartUp()
{
    //init texture
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    Util::LoadTGATexture("Chapter17 GeometryShader/Marble.tga", GL_LINEAR, GL_CLAMP);

    // init cube
    gltMakeCube(triangle, 1);
    // init camera
    glShaderMgr.InitializeStockShaders();
    shaderMgr.OnInit();
    normalCamera.OnInit(640, 480, 50, 1, 2);
    modelviewStack = normalCamera.GetModelviewStack();
}

void OnShutUp()
{
    glDeleteTextures(1, &texture);
    shaderMgr.OnUnInit();
    normalCamera.OnUnInit();
}

void Idle(void) {glutPostRedisplay();}
void KeyboardFn(unsigned char key, int x, int y) {normalCamera.KeyboardFn(key, x, y);}
void MouseClick(int button, int action, int x, int y) {normalCamera.MouseClick(button, action, x, y);}
void MotionFunc(int mouse_x, int mouse_y) {normalCamera.MotionFunc(mouse_x, mouse_y);}
void Resize(int w, int h) {normalCamera.Resize(w, h);}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("geometry shader");


    glutKeyboardFunc(KeyboardFn);
    glutReshapeFunc(Resize);
    glutMotionFunc(MotionFunc);
    glutMouseFunc(MouseClick);
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);

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
