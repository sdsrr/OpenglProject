#include "../Tools/Header/Tools.h"
#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tinyobjloader.h"
#include "../Tools/Header/Trackball.h"

const char* modelName = Util::GetFullPath("Tools/Models/OBJ/spider.obj");
Tinyobjloader modelObj;
GLShaderManager glShaderMgr;
ShaderMgr shaderMgr;
M3DVector4f green = {0,1,0,0};
NormalCamera normalCamera;
GLMatrixStack* modelviewStack;
GLfloat lightPosition[] = {-100, 100, -100,1};
BaseShaderParam shaderParam;

static void OnStartup()
{
    //load model data
    glActiveTexture(GL_TEXTURE0);
    modelObj.LoadObjAndConvert(modelName);

    normalCamera.OnInit(640, 480, 50, 2, 1);
    modelviewStack = normalCamera.GetModelviewStack();

    shaderMgr.OnInit();
    glShaderMgr.InitializeStockShaders();
}

static void OnShutup()
{
    shaderMgr.OnUnInit();
}

static void Display(void)
{
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelviewStack->PushMatrix();
        modelviewStack->Translate(0,0,-100);
        shaderParam.SetDiffuseColor(green);
        shaderParam.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix());
        shaderParam.SetMVMatrix(normalCamera.GetModeviewMatrix());
        shaderParam.SetLightPostion(lightPosition);
        shaderParam.SetNormalMatrix(normalCamera.GetNormalMatrix());
        shaderParam.colorMap[0] = 0;
        shaderMgr.UseTexture2d(shaderParam);
        modelObj.DefaultDrawGraph();
    modelviewStack->PopMatrix();
    glutSwapBuffers();
}

static void Idle(void)
{
    glutPostRedisplay();
}

void KeyboardFn(unsigned char key, int x, int y){normalCamera.KeyboardFn(key, x, y);}
void MouseClick(int button, int action, int x, int y){normalCamera.MouseClick(button, action, x, y);}
void MotionFunc(int mouse_x, int mouse_y){normalCamera.MotionFunc(mouse_x, mouse_y);}
void Resize(int w, int h){normalCamera.Resize(w, h);}

int main(int argc, char** argv)
{
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("model tinyobjloade");

    glutKeyboardFunc(KeyboardFn);
    glutReshapeFunc(Resize);
    glutMotionFunc(MotionFunc);
    glutMouseFunc(MouseClick);
    glutIdleFunc(Idle);
    glutDisplayFunc(Display);
    glEnable(GL_DEPTH_TEST);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW." << std::endl;
        return -1;
    }

    OnStartup();
    glutMainLoop();
    OnShutup();
}
