#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"
#include "../Tools/Header/GameObject.h"
#include "../Tools/Header/Camera.h"

GLShaderManager glShaderMgr;
ShaderMgr* shaderMgr;
NormalCamera normalCamera;
GLfloat lightDirection[] = {-1,0,1};

GLfloat angle;
BatchGObject mirror;
BatchGObject mirrorFrames[4];
BatchGObject triangle[2];
BaseShaderParam param;

GLuint texture;
GLuint frameTex;
GLuint fbo;
GLuint depthRbo;

const GLenum windowBuffer[]={GL_BACK_LEFT};
const GLenum fboBuffer[]={GL_COLOR_ATTACHMENT0};
GLfloat cameraPos[3] = {-10,-10,-30};
GLfloat cameraRoate[4] = {180,0,1,0};

void Display(void)
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 绘制到fbo纹理
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glDrawBuffers(1, fboBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLMatrixStack* modelStack = &triangle[0].modelStack;
    param.SetDiffuseColor(ShaderMgr::ondine);
    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(*modelStack));
    param.SetMVMatrix(modelStack->GetMatrix());
    param.SetNormalMatrix(normalCamera.GetNormalMatrix(*modelStack));
    param.SetLightDirection(lightDirection);
    shaderMgr->UseDiffuse(param);
    triangle[0].Draw();

    //绘制到世界
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDrawBuffers(1, windowBuffer);

    modelStack = &triangle[1].modelStack;
    param.SetDiffuseColor(ShaderMgr::ondine);
    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(*modelStack));
    param.SetMVMatrix(modelStack->GetMatrix());
    param.SetNormalMatrix(normalCamera.GetNormalMatrix(*modelStack));
    param.SetLightDirection(lightDirection);
    shaderMgr->UseDiffuse(param);
    triangle[1].Draw();

    //绘制镜面，将fbo纹理作为主纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    param.SetDiffuseColor(ShaderMgr::white);
    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(mirror.modelStack));
    param.SetMVMatrix(mirror.modelStack.GetMatrix());
    param.SetNormalMatrix(normalCamera.GetNormalMatrix(mirror.modelStack));
    param.colorMap[0] = 0;
    shaderMgr->UseTexture2d(param);
    mirror.Draw();

    //绘制镜框
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, frameTex);
    param.colorMap[0] = 1;
    shaderMgr->UseTexture2d(param);
    for (int i = 0; i < 4; i++)
        mirrorFrames[i].Draw();

    glutSwapBuffers();
}

void OnStartUp()
{
    //init depth rbo
    glGenRenderbuffers(1, &depthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, 640, 480);

    //init texture
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 640, 480, 0, GL_RGBA, GL_FLOAT, NULL);

    //init fbo
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    //init mirror
    mirror.batch.Begin(GL_TRIANGLE_STRIP,6,1);
    mirror.batch.MultiTexCoord2f(0,0,0);
    mirror.batch.Vertex3f(0,0,0);
    mirror.batch.MultiTexCoord2f(0,1,0);
    mirror.batch.Vertex3f(20,0,0);
    mirror.batch.MultiTexCoord2f(0,0,1);
    mirror.batch.Vertex3f(0,20,0);
    mirror.batch.MultiTexCoord2f(0,1,1);
    mirror.batch.Vertex3f(20,20,0);
    mirror.batch.End();
    mirror.modelStack.Translate(-10,-10,-30);

    //init mirror frame
    float frame = 0.6f;
    int index = 0;
    BatchGObject* mirrorFrame = &mirrorFrames[index++];
    mirrorFrame->batch.Begin(GL_TRIANGLE_STRIP,6,1);
    mirrorFrame->batch.MultiTexCoord2f(0,0,0);
    mirrorFrame->batch.Vertex3f(0,-frame,0);
    mirrorFrame->batch.MultiTexCoord2f(0,1,0);
    mirrorFrame->batch.Vertex3f(20,-frame,0);
    mirrorFrame->batch.MultiTexCoord2f(0,0,1);
    mirrorFrame->batch.Vertex3f(0,0,0);
    mirrorFrame->batch.MultiTexCoord2f(0,1,1);
    mirrorFrame->batch.Vertex3f(20,0,0);
    mirrorFrame->batch.End();
    mirrorFrame->modelStack.Translate(-10,-10,-30);

    mirrorFrame = &mirrorFrames[index++];
    mirrorFrame->batch.Begin(GL_TRIANGLE_STRIP,6,1);
    mirrorFrame->batch.MultiTexCoord2f(0,0,0);
    mirrorFrame->batch.Vertex3f(0,20,0);
    mirrorFrame->batch.MultiTexCoord2f(0,1,0);
    mirrorFrame->batch.Vertex3f(20,20,0);
    mirrorFrame->batch.MultiTexCoord2f(0,0,1);
    mirrorFrame->batch.Vertex3f(0,20+frame,0);
    mirrorFrame->batch.MultiTexCoord2f(0,1,1);
    mirrorFrame->batch.Vertex3f(20,20+frame,0);
    mirrorFrame->batch.End();
    mirrorFrame->modelStack.Translate(-10,-10,-30);

    mirrorFrame = &mirrorFrames[index++];
    mirrorFrame->batch.Begin(GL_TRIANGLE_STRIP,6,1);
    mirrorFrame->batch.MultiTexCoord2f(0,0,0);
    mirrorFrame->batch.Vertex3f(-frame,-frame,0);
    mirrorFrame->batch.MultiTexCoord2f(0,1,0);
    mirrorFrame->batch.Vertex3f(0,-frame,0);
    mirrorFrame->batch.MultiTexCoord2f(0,0,1);
    mirrorFrame->batch.Vertex3f(-frame,20+frame,0);
    mirrorFrame->batch.MultiTexCoord2f(0,1,1);
    mirrorFrame->batch.Vertex3f(0,20+frame,0);
    mirrorFrame->batch.End();
    mirrorFrame->modelStack.Translate(-10,-10,-30);

    mirrorFrame = &mirrorFrames[index++];
    mirrorFrame->batch.Begin(GL_TRIANGLE_STRIP,6,1);
    mirrorFrame->batch.MultiTexCoord2f(0,0,0);
    mirrorFrame->batch.Vertex3f(20,-frame,0);
    mirrorFrame->batch.MultiTexCoord2f(0,1,0);
    mirrorFrame->batch.Vertex3f(20+frame,-frame,0);
    mirrorFrame->batch.MultiTexCoord2f(0,0,1);
    mirrorFrame->batch.Vertex3f(20,20+frame,0);
    mirrorFrame->batch.MultiTexCoord2f(0,1,1);
    mirrorFrame->batch.Vertex3f(20+frame,20+frame,0);
    mirrorFrame->batch.End();
    mirrorFrame->modelStack.Translate(-10,-10,-30);

    //init frame tex
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &frameTex);
    glBindTexture(GL_TEXTURE_2D, frameTex);
    Util::LoadJPGTexture(Util::GetFullPath("Chapter13 RenderTexWithFBO/Marble.tga"), GL_LINEAR, GL_CLAMP_TO_EDGE);

    //init sphere
    gltMakeCube(triangle[0].batch, 1.5f);
    triangle[0].modelStack.Translate(0,0,-15);
    triangle[0].modelStack.Rotate(angle+=1.5f, 0, 1, 0);

    gltMakeCube(triangle[1].batch, 1.5f);
    triangle[1].modelStack.Translate(0,0,-15);
    triangle[1].modelStack.Rotate(angle+=1.5f, 0, 1, 0);
    //init shadermgr,camera
    glShaderMgr.InitializeStockShaders();
    shaderMgr = ShaderMgr::GetInstance();
    normalCamera.OnInit(640, 480, 50, 1, 2);
}

void OnShutUp()
{
    glDeleteRenderbuffers(1, &depthRbo);
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &texture);
    glDeleteTextures(1, &frameTex);
    shaderMgr->OnUnInit();
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
    glutCreateWindow("render texture with fbo");

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
