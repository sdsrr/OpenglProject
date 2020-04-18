#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"
#include "../Tools/Header/Camera.h"
#include "../Tools/Header/GameObject.h"
#include "../Tools/Header/FreetypeFont.h"
#include "../Tools/Header/Light.h"
#include "../Tools/Header/CommonStruct.h"

enum ETex
{
    ETexPosition,//位置信息
    ETexNormal,  //法线信息
    ETexDepth,   //深度贴图
    ETexSSAO,    //遮蔽贴图
    ETexNoise,   //噪声贴图(绕法线旋转半球面)
    ETexMax,
};

enum EObj
{
    EObjPosTex,    //位置贴图
    EObjNormalTex, //法线贴图
    EObjDepth,     //深度贴图
    EObjSSAOTex,   //ssao贴图
    EOBNoise,      //噪声贴图
    EObjScenePlane,//表面
    EObjMax,
};

enum EFrameBuf
{
    EFBDeferred,//延迟收集数据(位置/法线/深度)
    EFBSSAO,    //计算ssao
    EFBMax,
};

const int MaxCount = 10;

GLuint uniformBuf;
BaseShaderParam param;
GLShaderManager glShaderMgr;
ShaderMgr* shaderMgr;
NormalCamera sceneCamera;
UICamera uiCamera;

//法线半球采样点
std::uniform_real_distribution<GLfloat> randomFloats(0.0f, 1.0f);//[0-1]随机
std::default_random_engine generator;
std::vector<vec3> ssaoNoise;

//texture
GLuint textures[(int)ETexMax];
//texture object
BatchGObject textureObjs[(int)EObjMax];
//cube object
BatchGObject objects[MaxCount];
//framebuffer
GLuint frameBuffers[(int)EFBMax];

const GLuint deferredAttachs[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
const GLuint ssaoAttachs[1] = {GL_COLOR_ATTACHMENT0};
const GLenum windowBuffer[]={GL_BACK_LEFT};

void DrawDeferred()
{
    //draw to deferred framebuffer
    GLuint framebuf = frameBuffers[(int)EFBDeferred];
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuf);
    glDrawBuffers(2, deferredAttachs);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0 ; i < MaxCount; i++)
    {
        BatchGObject& object = objects[i];
        param.SetMVMatrix(sceneCamera.GetModelviewMatrix(object.modelStack));
        param.SetMVPMatrix(sceneCamera.GetModelviewprojectMatrix(object.modelStack));
        shaderMgr->UseSSAODeferred(param, 0.1f, 1000);
        object.Draw();
    }
}

void DrawSSAO()
{
    //draw ssoa to framebuffer
    GLuint framebuf = frameBuffers[(int)EFBSSAO];
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuf);
    glDrawBuffers(1, ssaoAttachs);
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    BatchGObject& scenePlane = textureObjs[(int)EObjScenePlane];
    param.SetMVPMatrix(uiCamera.GetModelviewprojectMatrix(scenePlane.modelStack));
    param.SetProjectMatrix(sceneCamera.GetProjectMatrix());
    param.colorMap[0] = (int)EObjPosTex;
    param.colorMap[1] = (int)ETexNormal;
    param.colorMap[2] = (int)ETexDepth;
    param.colorMap[3] = (int)ETexNoise;
    shaderMgr->UseCalcSSAO(param, 640 ,480);
    scenePlane.Draw();
}

void DrawScene()
{
    //draw scene plane
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDrawBuffers(1, windowBuffer);
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    BatchGObject& scenePlane = textureObjs[(int)EObjScenePlane];
    param.SetMVPMatrix(uiCamera.GetModelviewprojectMatrix(scenePlane.modelStack));
    param.SetViewMatrix(sceneCamera.GetViewMatrix());
    param.SetLightDirection(0,0,1);
    param.SetDiffuseColor(ShaderMgr::white);
    param.colorMap[0] = (int)ETexPosition;
    param.colorMap[1] = (int)ETexNormal;
    param.colorMap[2] = (int)ETexSSAO;
    shaderMgr->UseSSAO(param);
    scenePlane.Draw();
}

void DrawTexture()
{
    //draw texture
    for (int i = 0 ; i <= (int)ETexNoise; i++)
    {
        BatchGObject& object = textureObjs[i];
        param.SetMVPMatrix(uiCamera.GetModelviewprojectMatrix(object.modelStack));
        param.SetDiffuseColor(ShaderMgr::white);
        param.colorMap[0] = i;
        shaderMgr->UseTexture2d(param);
        object.Draw();
    }
}

void Display(void)
{
    //收集法线/顶点/深度数据
    DrawDeferred();
    //根据上阶段收集数据计算ssao
    DrawSSAO();
    //计算最终光照
    DrawScene();
    //绘制各阶段texture
    DrawTexture();

    glutSwapBuffers();
}

void CreateCubeObject()
{
    for (int i = 0; i < MaxCount; i++)
    {
        BatchGObject& object = objects[i];
        M3DVector3f vector;
        Util::RandomVector3(vector, 30, 30 ,30);
        object.CreateCube(vector[0], vector[1], vector[2], 1);

        Util::RandomVector3(vector, 30, 30 ,30);
        object.modelStack.LoadIdentity();
        object.modelStack.Translate(vector[0],vector[1],-90 + vector[2]);
        Util::RandomVector3(vector, 180, 180 ,180);
        object.modelStack.Rotate(60, vector[0], vector[1], vector[2]);
    }
}


void CreateRendererTexture(GLuint* texture, GLenum attach, int length, int height)
{
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, length, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attach, GL_TEXTURE_2D, *texture, 0);
}

void CreateDepthTexture(GLuint* texture, int length, int height)
{
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, length, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *texture, 0);
}

void CreateDefferedFrameBuffer(int length, int height)
{
    GLuint& frameBuffer = frameBuffers[(int)EFBDeferred];
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);

    //create and bind position info texture
    glActiveTexture(GL_TEXTURE0 + (int)ETexPosition);
    CreateRendererTexture(&textures[(int)ETexPosition], GL_COLOR_ATTACHMENT0, length, height);

    //create and bind normal info texture
    glActiveTexture(GL_TEXTURE0 + (int)ETexNormal);
    CreateRendererTexture(&textures[(int)ETexNormal], GL_COLOR_ATTACHMENT1, length, height);

    //create and bind depth render texture
    glActiveTexture(GL_TEXTURE0 + (int)ETexDepth);
    CreateDepthTexture(&textures[(int)ETexDepth], length, height);

    Util::CheckFBO();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void CreateSSAOFramebuffer(int length, int height)
{
    GLuint& frameBuffer = frameBuffers[(int)EFBSSAO];
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);

    //因为只绘制一个表面，故可以没有深度缓冲区
    //create and bind ssao info texture
    glActiveTexture(GL_TEXTURE0 + (int)ETexSSAO);
    CreateRendererTexture(&textures[(int)ETexSSAO], GL_COLOR_ATTACHMENT0, length, height);

    Util::CheckFBO();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void CreateTexturePlane()
{
    for (int i = 0 ; i < (int)EObjMax - 1; i++)
    {
        BatchGObject& object = textureObjs[i];
        object.CreateTriangle(20, 20, 1);
        object.modelStack.Translate(-60 + i * 22, -60, -10);
    }
}

void CreateScenePlane()
{
    BatchGObject& object = textureObjs[(int)EObjScenePlane];
    object.CreateTriangle(120, 120, 1);
    object.modelStack.Translate(-60,-60,-20);
}

void CreateSSAOKernel()
{
    std::vector<vec3> ssaoKernel;
    for (int i = 0; i < 64; i++)
    {
        vec3 vec;
        vec.v[0] = randomFloats(generator) * 2 - 1;//[-1,1]
        vec.v[1] = randomFloats(generator) * 2 - 1;
        vec.v[2] = randomFloats(generator);//[0,1]z轴为法线方向，半球范围
        Util::NormalizeVector(vec);
        //限制采样点更接近像素点
        GLfloat scale = i / 64.0f;
        scale = Util::Lerp(0.1f, 1, scale * scale);
        float d = randomFloats(generator);
        vec.v[0] *= scale * d;
        vec.v[1] *= scale * d;
        vec.v[2] *= scale * d;
        ssaoKernel.push_back(vec);
    }

    glGenBuffers(1, &uniformBuf);
    glBindBuffer(GL_UNIFORM_BUFFER, uniformBuf);
    glBufferData(GL_UNIFORM_BUFFER, 64*16, NULL, GL_DYNAMIC_DRAW);
    for (int i = 0 ; i < 64; i++)
    {
        vec3 vec = ssaoKernel[i];
        glBufferSubData(GL_UNIFORM_BUFFER, 16 * i, 16, vec.v);
    }
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniformBuf);

}

void CreateNoiseTexture()
{
    //构造4*4随机点,再利用纹理repeat环绕模式,构造全屏大小的随机纹理
    for (int i = 0; i < 16; i++)
    {
        vec3 vec;
        vec.v[0] = randomFloats(generator) * 2 - 1;
        vec.v[1] = randomFloats(generator) * 2 - 1;
        vec.v[2] = 0;
        ssaoNoise.push_back(vec);
    }

    GLuint& texture = textures[(int)ETexNoise];
    glActiveTexture(GL_TEXTURE0 + (int)ETexNoise);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void OnStartUp()
{
    //init framebuffer
    CreateDefferedFrameBuffer(640, 480);
    CreateSSAOFramebuffer(640, 480);
    //init object
    CreateCubeObject();
    //init texture object
    CreateTexturePlane();
    CreateScenePlane();
    //create ssao kernel
    CreateSSAOKernel();
    //create noise texture
    CreateNoiseTexture();
    //init camera
    glShaderMgr.InitializeStockShaders();
    shaderMgr = ShaderMgr::GetInstance();
    sceneCamera.OnInit(640, 480, 50, 1, 2);
    uiCamera.OnInit(120, 120);
}

void OnShutUp()
{
    shaderMgr->OnUnInit();
    sceneCamera.OnUnInit();
    glDeleteTextures((int)ETexMax, textures);
    glDeleteFramebuffers(2, frameBuffers);
}

void Idle(void) {glutPostRedisplay();}
void KeyboardFn(unsigned char key, int x, int y) {sceneCamera.KeyboardFn(key, x, y);}
void MouseClick(int button, int action, int x, int y) {sceneCamera.MouseClick(button, action, x, y);}
void MotionFunc(int mouse_x, int mouse_y) {sceneCamera.MotionFunc(mouse_x, mouse_y);}
void Resize(int w, int h) {sceneCamera.Resize(w, h);}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("ssao");


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
