#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"
#include "../Tools/Header/UtilPrint.h"
#include "../Tools/Header/Camera.h"
#include "../Tools/Header/GameObject.h"
#include "../Tools/Header/Light.h"
#include "../Tools/Header/CommonStruct.h"
#include "../Tools/Header/UtilTimer.h"

bool useDeferredRender = false;
BaseShaderParam param;
GLShaderManager glShaderMgr;
ShaderMgr* shaderMgr;
NormalCamera normalCamera;
UICamera uiCamera;
FrameTimer frameTimer;

const int MaxLightNum = 500;
const int MaxSphereNum = 500;
TriangleGObject sphere[MaxSphereNum];
Light lights[MaxLightNum];
TriangleGObject lightObjs[MaxLightNum];
GLuint ubo_light;


void ShowCameraPosition()
{
    param.SetMVPMatrix(uiCamera.GetProjectMatrix());
    shaderMgr->UseFont(param, ShaderMgr::white);
    normalCamera.ShowInfo(-60,50);
}

void ShowFrameCount()
{
    param.SetMVPMatrix(uiCamera.GetProjectMatrix());
    shaderMgr->UseFont(param, ShaderMgr::white);
    frameTimer.Update();
}

void DrawLight()
{
    glDisable(GL_DEPTH_TEST);
    for (int i = 0 ; i < MaxLightNum; i++)
    {
        TriangleGObject& li = lightObjs[i];
        param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(li.modelStack));
        param.SetDiffuseColor(lights[i].color, 0.1f);
        shaderMgr->UseSolidColor(param);
        li.Draw();
    }
    glEnable(GL_DEPTH_TEST);
}

void DrawSphereNormalMode()
{
    for (int i = 0 ; i < MaxSphereNum; i++)
    {
        GLMatrixStack& modelStack = sphere[i].modelStack;
        param.SetMMatrix(modelStack.GetMatrix());
        param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(modelStack));
        shaderMgr->UseSphereLight(param);
        sphere[i].Draw();
    }
}

void DrawSphereDeferredMode()
{

}

void Display(void)
{
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (useDeferredRender)
        DrawSphereDeferredMode();
    else
        DrawSphereNormalMode();

    //DrawLight();
    ShowFrameCount();
    ShowCameraPosition();
    glutSwapBuffers();
}

void InitLight()
{
    //init light
    float space = 3.5f;
    int lineNum = 30;
    GLuint progress = shaderMgr->GetShaderId(STSphereLight);
    for (int i = 0; i < MaxLightNum; i++)
    {
        Light& li = lights[i];
        li.radius = random(10);
        li.position[0] = space * (i % lineNum);
        li.position[1] = random(16);
        li.position[2] = space * (i / lineNum);
        Util::RandomCol(li.color);

        TriangleGObject& obj = lightObjs[i];
        gltMakeSphere(obj.batch, li.radius, 30, 30);
        obj.modelStack.Translate(li.position[0], li.position[1], li.position[2]);
    }

    // init uniform buffer
    glGenBuffers(1, &ubo_light);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_light);
    glBufferData(GL_UNIFORM_BUFFER, 48*MaxLightNum, NULL, GL_DYNAMIC_DRAW);
    for (int i = 0 ; i < MaxLightNum; i++)
    {
        Light& li = lights[i];
        glBufferSubData(GL_UNIFORM_BUFFER, 0  + i * 48, 4, &li.radius);
        glBufferSubData(GL_UNIFORM_BUFFER, 16 + i * 48, 16, li.position);
        glBufferSubData(GL_UNIFORM_BUFFER, 32 + i * 48, 16, li.color);
    }
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_light);
}

void InitSphere()
{
    float space = 3;
    int lineNum = 30;
    for (int i = 0; i < MaxSphereNum; i++)
    {
        M3DVector3f position;
        Util::RandomVector3(position, 100, 10, 100);
        position[0] = space * (i & lineNum);
        position[1] = random(10);
        position[2] = space * (i / lineNum);

        gltMakeSphere(sphere[i].batch, 1, 20, 20);
        sphere[i].modelStack.Translate(position[0], position[1], position[2]);
    }
}

void InitCamera()
{
    //init camera
    glShaderMgr.InitializeStockShaders();
    shaderMgr = ShaderMgr::GetInstance();
    normalCamera.OnInit(640, 480, 50, 1, 2);
    uiCamera.OnInit(120,120);
    srand(int(time(0)));
}

void OnStartUp()
{
    InitCamera();
    InitSphere();
    InitLight();
}

void OnShutUp()
{
    shaderMgr->OnUnInit();
    normalCamera.OnUnInit();
    uiCamera.OnUnInit();
    glDeleteBuffers(1, &ubo_light);
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
    glEnable(GL_BLEND);

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
