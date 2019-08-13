#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"
#include "../Tools/Header/UtilPrint.h"
#include "../Tools/Header/Camera.h"
#include "../Tools/Header/GameObject.h"
#include "../Tools/Header/Light.h"
#include "../Tools/Header/CommonStruct.h"
#include "../Tools/Header/UtilTimer.h"

enum ETex
{
    Position,
    Normal,
    Max,
};

bool useDeferredRender = false;
BaseShaderParam param;
GLShaderManager glShaderMgr;
ShaderMgr* shaderMgr;
NormalCamera normalCamera;
UICamera uiCamera;
FrameTimer frameTimer;
BatchGObject ground;

GLuint rbo;
GLuint frameBuffer;
GLuint textures[(int)Max];
BatchGObject texObjects[(int)Max];
BatchGObject scenePlane;
const GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
const GLenum windowBuffer[]={GL_BACK_LEFT};

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

void NormalDrawGround()
{
    param.SetMMatrix(ground.modelStack.GetMatrix());
    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(ground.modelStack));
    shaderMgr->UseSphereLight(param);
    ground.Draw();
}

void NormalDrawSphere()
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

void NormalMode()
{
    NormalDrawSphere();
    NormalDrawGround();
}

void DrawTexture()
{
    for (int i = 0 ; i < (int)Max; i++)
    {
        BatchGObject& obj = texObjects[i];
        param.colorMap[0] = i;
        param.SetMVPMatrix(uiCamera.GetModelviewprojectMatrix(obj.modelStack));
        param.SetDiffuseColor(ShaderMgr::white);
        shaderMgr->UseTexture2d(param);
        obj.Draw();
    }
}

void DeferredDrawSphereWrite()
{
    for (int i = 0 ; i < MaxSphereNum; i++)
    {
        GLMatrixStack& modelStack = sphere[i].modelStack;
        param.SetMMatrix(modelStack.GetMatrix());
        param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(modelStack));
        shaderMgr->UseDeferredIn(param);
        sphere[i].Draw();
    }
}

void DeferredDrawGroundWrite()
{
    param.SetMMatrix(ground.modelStack.GetMatrix());
    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(ground.modelStack));
    shaderMgr->UseDeferredIn(param);
    ground.Draw();
}

void DeferredDraw()
{
    param.colorMap[0] = 0;
    param.colorMap[1] = 1;
    param.SetMVPMatrix(uiCamera.GetModelviewprojectMatrix(scenePlane.modelStack));
    shaderMgr->UseDeferredOut(param);
    scenePlane.Draw();
}

void DeferredMode()
{
    //写入物件数据到fbo(textures)
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
    glDrawBuffers(2, attachments);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DeferredDrawSphereWrite();
    DeferredDrawGroundWrite();

    //从fbo(texture)中取物件数据,进行复杂光照计算,渲染到屏幕
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDrawBuffers(1, windowBuffer);
    DeferredDraw();

    DrawTexture();
}

void Display(void)
{
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (useDeferredRender)
        DeferredMode();
    else
        NormalMode();

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
    for (int i = 0; i < MaxLightNum; i++)
    {
        Light& li = lights[i];
        li.radius = random(12);
        li.position[0] = space * (i % lineNum);
        li.position[1] = random(20);
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
    float minY = 0;
    for (int i = 0; i < MaxSphereNum; i++)
    {
        M3DVector3f position;
        Util::RandomVector3(position, 100, 10, 100);
        position[0] = space * (i & lineNum);
        position[1] = random(10);
        position[2] = space * (i / lineNum);
        minY = std::min(0.0f, position[1]);

        gltMakeSphere(sphere[i].batch, 1, 20, 20);
        sphere[i].modelStack.Translate(position[0], position[1], position[2]);
    }

    //init ground
    Util::MakePlaneBatch(ground.batch, 100, 100);
    ground.modelStack.Translate(-5, minY - 3, -15);

    //init texture plane
    for (int i = 0 ; i < (int)Max; i++)
    {
        BatchGObject& obj = texObjects[i];
        Util::MakePlaneBatch(obj.batch, 25, 19);
        obj.modelStack.Rotate(-90, 1, 0, 0);
        obj.modelStack.Translate(-59 + i * 30, 0, -59);
    }

    //init scene plane
    Util::MakePlaneBatch(scenePlane.batch, 120, 120);
    scenePlane.modelStack.Rotate(-90, 1, 0, 0);
    scenePlane.modelStack.Translate(-60, 10, -60);
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

void InitTexture(GLuint* id, float width, float height, GLenum attachment)
{
    glBindTexture(GL_TEXTURE_2D, *id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D, *id, 0);
}

void InitFrameBuffer(float width, float height)
{
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    glGenTextures((int)Max, textures);
    glActiveTexture(GL_TEXTURE0);
    InitTexture(&textures[(int)Position], width, height, GL_COLOR_ATTACHMENT0);
    glActiveTexture(GL_TEXTURE1);
    InitTexture(&textures[(int)Normal], width, height, GL_COLOR_ATTACHMENT1);
    Util::CheckFBO();

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void OnStartUp()
{
    InitCamera();
    InitSphere();
    InitLight();
    InitFrameBuffer(640, 480);
}

void OnShutUp()
{
    shaderMgr->OnUnInit();
    normalCamera.OnUnInit();
    uiCamera.OnUnInit();
    glDeleteBuffers(1, &ubo_light);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &frameBuffer);
    glDeleteTextures((int)Max, textures);
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
