#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"
#include "../Tools/Header/Camera.h"
#include "../Tools/Header/GameObject.h"
#include "../Tools/Header/FreetypeFont.h"
#include "../Tools/Header/Light.h"
#include "../Tools/Header/CommonStruct.h"

BaseShaderParam param;
GLShaderManager glShaderMgr;
ShaderMgr* shaderMgr;
NormalCamera normalCamera;
UICamera uiCamera;
CharText charTex;

const int MaxLightnum = 3;
const int MaxSpherenum = 10;
TriangleGObject sphere[MaxSpherenum];
Light light[MaxLightnum];

void ShowCameraPosition()
{
    param.SetMVPMatrix(uiCamera.GetProjectMatrix());
    shaderMgr->UseFont(param, ShaderMgr::black);
    normalCamera.ShowInfo(-60,50);
}

void ShowFrameCount()
{
    char* str = "frame count";
    param.SetMVPMatrix(uiCamera.GetProjectMatrix());
    shaderMgr->UseFont(param, ShaderMgr::black);
    charTex.CreateText(str, strlen(str), -60, 40, 0.3f, 20, 0.5f);
    charTex.Draw();
}

void Display(void)
{
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int m = 0 ; m < 3; m++)
    {
        Light li = light[m];
        M3DVector4f lightPosition = {0,0,0,1};
        m3dTransformVector4(lightPosition, lightPosition, li.modelStack.GetMatrix());
        for (int i = 0 ; i < MaxSpherenum; i++)
        {
            GLMatrixStack& modeStack = sphere[i].modelviewStack;
            param.SetMMatrix(modeStack.GetMatrix());
            param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(modeStack));
            shaderMgr->UseSphereLight(param, lightPosition, li.color, li.radius);
            sphere[i].Draw();
        }

        //Util::PrintVector3f(lightPosition);
    }

    ShowFrameCount();
    ShowCameraPosition();
    glutSwapBuffers();
}

void OnStartUp()
{
    for (int i = 0; i < MaxSpherenum; i++)
    {
        M3DVector3f position;
        Util::RandomVector3(position, 10, 5, 10);
        //printf("%f %f %f \n", x,y,z);
        gltMakeSphere(sphere[i].batch, 1, 20, 20);
        sphere[i].modelviewStack.Translate(position[0], position[1], position[2]);
    }

    //init light
    for (int i = 0; i < MaxLightnum; i++)
    {
        M3DVector3f position;
        Util::RandomVector3(position, 10, 5, 10);
        Light& li = light[i];
        li.radius = random(10);
        li.modelStack.Translate(position[0], position[1], position[2]);
        Util::RandomCol(li.color);
    }

    //init camera
    glShaderMgr.InitializeStockShaders();
    shaderMgr = ShaderMgr::GetInstance();
    normalCamera.OnInit(640, 480, 50, 1, 2);
    uiCamera.OnInit(120,120);
    srand(int(time(0)));
}

void OnShutUp()
{
    shaderMgr->OnUnInit();
    normalCamera.OnUnInit();
    uiCamera.OnUnInit();
}

void Idle(void) {glutPostRedisplay();}
void KeyboardFn(unsigned char key, int x, int y) {normalCamera.KeyboardFn(key, x, y);}
void MouseClick(int button, int action, int x, int y) {normalCamera.MouseClick(button, action, x, y);}
void MotionFunc(int mouse_x, int mouse_y) {normalCamera.MotionFunc(mouse_x, mouse_y);}
void Resize(int w, int h) {normalCamera.Resize(w, h);normalCamera.Translate(-7,-9,-34);}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("deferred render");


    glutKeyboardFunc(KeyboardFn);
    glutReshapeFunc(Resize);
    glutMotionFunc(MotionFunc);
    glutMouseFunc(MouseClick);
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
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
