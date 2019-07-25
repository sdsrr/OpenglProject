#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"
#include "../Tools/Header/FreetypeFont.h"
#include "../Tools/Header/GameObject.h"
#include "../Tools/Header/Camera.h"

enum ETexture
{
    ETGround,
    ETCubeMaintex,
    ETDepth,
};

enum EObject
{
    EOCube01,
    EOCube02,
    EOCube03,
    EOGround,
    EODepth,
    EOMax,
};

CharText charTex;
UICamera uiCamera;
UICamera shadowCamera;
NormalCamera normalCamera;
float shadowmapWidth = 512;
float shadowmapHeight = 512;
float shaderCameraViewSize = 50;

BaseShaderParam param;
GLShaderManager glShaderMgr;
ShaderMgr* shaderMgr;

GLuint frameBuf;
GLfloat angle;
BatchGObject triangles[(int)EOMax];
GLTriangleBatch cameraBatch;
GLuint textures[3];
GLfloat fontCol[] = {0,0,0,1};
bool nomlCameraView = true;//normalcamera/shadowcamera
char* paths[] =
{
    Util::GetFullPath("Chapter21 Shadowmap/pig.jpg"),
    Util::GetFullPath("Chapter21 Shadowmap/grass.png")
};

BaseCamera& GetMainCamera()
{
    if (nomlCameraView)
        return normalCamera;
    return shadowCamera;
}

void DrawGround(BatchGObject& object)
{
    BaseCamera& mainCamera = GetMainCamera();
    param.SetMVPMatrix(mainCamera.GetModelviewprojectMatrix(object.modelviewStack));
    param.SetDiffuseColor(ShaderMgr::white);
    param.colorMap[0] = 0;
    param.colorMap[1] = 1;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[(int)ETGround]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[(int)ETDepth]);
    shaderMgr->UseShadowmap(param, shadowCamera.GetModelviewprojectMatrix(object.modelviewStack));
    object.Draw();
}

void DrawGroundShadow(BatchGObject& object)
{
    param.SetMVPMatrix(shadowCamera.GetModelviewprojectMatrix(object.modelviewStack));
    shaderMgr->WriteToShadowmap(param);
    object.Draw();
}

void DrawCube(BatchGObject& object)
{
    BaseCamera& mainCamera = GetMainCamera();
    param.SetMVPMatrix(mainCamera.GetModelviewprojectMatrix(object.modelviewStack));
    param.colorMap[0] = 0;
    param.colorMap[1] = 1;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[(int)ETCubeMaintex]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[(int)ETDepth]);
    shaderMgr->UseShadowmap(param, shadowCamera.GetModelviewprojectMatrix(object.modelviewStack));
    object.Draw();
}

void DrawCubeShadow(BatchGObject& object)
{
    param.SetMVPMatrix(shadowCamera.GetModelviewprojectMatrix(object.modelviewStack));
    shaderMgr->WriteToShadowmap(param);
    object.Draw();
}

void DrawDepthTexture(BatchGObject& object)
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[(int)ETDepth]);
    param.SetMVPMatrix(uiCamera.GetModelviewprojectMatrix(object.modelviewStack));
    param.SetDiffuseColor(ShaderMgr::white);
    param.colorMap[0] = 1;
    shaderMgr->UseTexture2d(param);
    object.Draw();
}

void DrawCharText()
{
    param.colorMap[0] = 0;
    param.SetMVPMatrix(uiCamera.GetModelviewprojectMatrix(charTex.modelviewStack));
    shaderMgr->UseFont(param, fontCol);
    charTex.Draw();
}

void RoateCube(BatchGObject& object)
{
    object.modelviewStack.Rotate(2.0f, 0, 1, 0);
}

void DrawCameraBox()
{
    BaseCamera& mainCamera = GetMainCamera();
    GLMatrixStack& matrixStack = shadowCamera.GetModeviewStack();
    const M3DMatrix44f& mvMatrix = matrixStack.GetMatrix();
    const M3DMatrix44f& projectMatrix = mainCamera.GetProjectMatrix();
    const M3DMatrix33f& normalMatrix = mainCamera.GetNormalMatrix(matrixStack);
    shadowCamera.Draw(mvMatrix, projectMatrix, normalMatrix);
}

void Display(void)
{
    //roate cube
    RoateCube(triangles[(int)EOCube01]);
    RoateCube(triangles[(int)EOCube02]);

    //draw shadow
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,shadowmapWidth,shadowmapHeight);

    DrawCubeShadow(triangles[(int)EOCube01]);
    DrawCubeShadow(triangles[(int)EOCube02]);
    DrawGroundShadow(triangles[(int)EOGround]);

    //draw gameobject
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,640,480);

    DrawCube(triangles[(int)EOCube01]);
    DrawCube(triangles[(int)EOCube02]);
    DrawGround(triangles[(int)EOGround]);

    DrawCameraBox();
    DrawDepthTexture(triangles[(int)EODepth]);
    DrawCharText();
    glutSwapBuffers();
}

void OnStartUp()
{
    //load texture
    glGenTextures(3, textures);
    glBindTexture(GL_TEXTURE_2D, textures[(int)ETCubeMaintex]);
    Util::LoadJPGTexture(paths[0], GL_LINEAR, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, textures[(int)ETGround]);
    Util::LoadJPGTexture(paths[1], GL_LINEAR, GL_CLAMP);

    //init triangles
    BatchGObject& gameobj01 = triangles[(int)EOCube01];
    gltMakeCube(gameobj01.batch, 2.0f);
    gameobj01.modelviewStack.Translate(0, 0, -15);

    BatchGObject& gameobj02 = triangles[(int)EOCube02];
    gltMakeCube(gameobj02.batch, 1.0f);
    gameobj02.modelviewStack.Translate(-10, 5, -15);
    gameobj02.modelviewStack.Scale(5,1,1);

    BatchGObject& ground = triangles[(int)EOGround];
    ground.modelviewStack.Translate(-10, -4, -20);
    ground.modelviewStack.Rotate(90,1,0,0);
    ground.batch.Begin(GL_TRIANGLE_STRIP, 6, 1);
    ground.batch.MultiTexCoord2f(0,0,0);
    ground.batch.Vertex3f(-50,-50,0);
    ground.batch.MultiTexCoord2f(0,1,0);
    ground.batch.Vertex3f(50,-50,0);
    ground.batch.MultiTexCoord2f(0,1,1);
    ground.batch.Vertex3f(50,50,0);

    ground.batch.MultiTexCoord2f(0,1,1);
    ground.batch.Vertex3f(50,50,0);
    ground.batch.MultiTexCoord2f(0,0,1);
    ground.batch.Vertex3f(-50,50,0);
    ground.batch.MultiTexCoord2f(0,0,0);
    ground.batch.Vertex3f(-50,-50,0);
    ground.batch.End();

    BatchGObject& depth = triangles[(int)EODepth];
    depth.batch.Begin(GL_TRIANGLES, 6, 1);
    depth.batch.MultiTexCoord2f(0,0,0);
    depth.batch.Vertex3f(0,0,0);
    depth.batch.MultiTexCoord2f(0,1,0);
    depth.batch.Vertex3f(1,0,0);
    depth.batch.MultiTexCoord2f(0,1,1);
    depth.batch.Vertex3f(1,1,0);

    depth.batch.MultiTexCoord2f(0,0,0);
    depth.batch.Vertex3f(0,0,0);
    depth.batch.MultiTexCoord2f(0,1,1);
    depth.batch.Vertex3f(1,1,0);
    depth.batch.MultiTexCoord2f(0,0,1);
    depth.batch.Vertex3f(0,1,0);
    depth.batch.End();
    depth.modelviewStack.Scale(60,60,1);
    depth.modelviewStack.Translate(-2.7,-2,0);
    //init camera
    glShaderMgr.InitializeStockShaders();
    shaderMgr = ShaderMgr::GetInstance();

    normalCamera.OnInit(640, 480, 50, 1, 2);
    uiCamera.OnInit(320, 240);
    shadowCamera.OnInit(shaderCameraViewSize, shaderCameraViewSize);
    shadowCamera.Rotate(90,1,0,0);

    //init framebuffer
    glGenFramebuffers(1, &frameBuf);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[(int)ETDepth]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowmapWidth, shadowmapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[(int)ETDepth], 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //init chartext
    char ch[] = "frame count:";
    charTex.CreateText(ch, sizeof(ch)-1, -160, 100, 0.2f, 10);
}

void OnShutUp()
{
    charTex.OnUnInit();
    uiCamera.OnUnInit();
    shadowCamera.OnUnInit();
    glDeleteTextures(3, textures);
    shaderMgr->OnUnInit();
    normalCamera.OnUnInit();
}

void Idle(void) {glutPostRedisplay();}
void KeyboardFn(unsigned char key, int x, int y) {shadowCamera.KeyboardFn(key, x, y);}
void MouseClick(int button, int action, int x, int y) {shadowCamera.MouseClick(button, action, x, y);}
void MotionFunc(int mouse_x, int mouse_y) {shadowCamera.MotionFunc(mouse_x, mouse_y);}
void Resize(int w, int h) {normalCamera.Resize(w, h); normalCamera.Translate(0,-10,-50);}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("shadow map");

    glutKeyboardFunc(KeyboardFn);
    glutReshapeFunc(Resize);
    glutMotionFunc(MotionFunc);
    glutMouseFunc(MouseClick);
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glFrontFace(GL_CCW);

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
