#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"
#include "../Tools/Header/GameObject.h"
#include "../Tools/Header/Camera.h"

GLShaderManager glShaderMgr;
ShaderMgr* shaderMgr;
NormalCamera normalCamera;
BaseShaderParam param;

GLfloat angle = 10;
BatchGObject triangles[3];
GLfloat color[] = {1,0,0,1};
GLfloat lightDirection[] ={-1,0,0,0};

GLuint fbo;
GLuint rbo[2];
GLenum fboBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
GLenum windowBuffers[] = {GL_FRONT_LEFT};

GLuint texture;

void Display(void)
{
    //将triangle[0]绘制到rbo,texture
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glDrawBuffers(2, fboBuffers);
    glClearColor(0.3f, 0.3f, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLMatrixStack* modelStack = &triangles[0].modelStack;
    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(*modelStack));
    param.SetMVMatrix(modelStack->GetMatrix());
    param.SetNormalMatrix(normalCamera.GetNormalMatrix(*modelStack));
    param.SetLightDirection(lightDirection);
    param.SetEnvironmentColor(color);
    shaderMgr->DrawToFBO(param);
    triangles[0].Draw();

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDrawBuffers(1, windowBuffers);
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //绘制trinagle[1]到左边
    modelStack = &triangles[1].modelStack;
    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(*modelStack));
    param.SetMVMatrix(modelStack->GetMatrix());
    param.SetNormalMatrix(normalCamera.GetNormalMatrix(*modelStack));
    param.SetLightDirection(lightDirection);
    param.SetDiffuseColor(color);
    shaderMgr->UseDiffuse(param);
    triangles[1].Draw();

    //将多重采样rbo图像显示到屏幕右上
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    //设置范围与采样数Multisample有关???设置错误可能显示不了
    glBlitFramebuffer(0,0,320,240, 320,240,640,480, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    //将多重采样texture图像显示到屏幕右下(面片采样)
    modelStack = &triangles[2].modelStack;
    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(*modelStack));
    param.SetMVMatrix(modelStack->GetMatrix());
    param.SetNormalMatrix(normalCamera.GetNormalMatrix(*modelStack));
    param.colorMap[0] = 0;
    shaderMgr->UseMsaa(param);
    triangles[2].Draw();

    glutSwapBuffers();
}

void OnStartUp()
{
    //初始化多重采样渲染对象
    glGenFramebuffers(1, &fbo);
    glGenRenderbuffers(2, rbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo[0]);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, 640, 480);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, 640, 480);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo[0]);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo[1]);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT32, 640, 480);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, 640, 480);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo[1]);

    //初始化多重采样纹理
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, 640, 480, GL_TRUE);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, texture, 0);
    Util::CheckFBO();

    //init triangle
    for (int i = 0; i < 3; i++)
    {
        GLBatch& triangle = triangles[i].batch;
        triangle.Begin(GL_TRIANGLE_STRIP, 6, 1);
        triangle.MultiTexCoord2f(0,0,0);
        triangle.Normal3f(-1,0,0);
        triangle.Vertex3f(0,0,0);

        triangle.MultiTexCoord2f(0,1,0);
        triangle.Normal3f(-1,0,0);
        triangle.Vertex3f(1,0,0);

        triangle.MultiTexCoord2f(0,0,1);
        triangle.Normal3f(-1,0,0);
        triangle.Vertex3f(0,1,0);

        triangle.MultiTexCoord2f(0,1,1);
        triangle.Normal3f(-1,0,0);
        triangle.Vertex3f(1,1,0);
        triangle.End();
    }
    triangles[0].modelStack.Translate(-2, -1.5f, -5);
    triangles[0].modelStack.Rotate(angle+=2, 1, 1, 0);
    triangles[1].modelStack.Translate(-2, 0, -5);
    triangles[1].modelStack.Rotate(angle, 1, 1, 0);
    triangles[2].modelStack.Translate(0, -2.4f, -5);
    triangles[2].modelStack.Scale(4*0.78f, 3*0.78f, 1);

    //init shadermgr
    glShaderMgr.InitializeStockShaders();
    shaderMgr = ShaderMgr::GetInstance();
    normalCamera.OnInit(640, 480, 50, 1, 2);
}

void OnShutUp()
{
    glDeleteTextures(1, &texture);
    glDeleteFramebuffers(1, &fbo);
    glDeleteRenderbuffers(2, rbo);
    shaderMgr->OnUnInit();
    normalCamera.OnUnInit();
}

void Idle(void) {glutPostRedisplay();}
void MouseClick(int button, int action, int x, int y) {normalCamera.MouseClick(button, action, x, y);}
void MotionFunc(int mouse_x, int mouse_y) {normalCamera.MotionFunc(mouse_x, mouse_y);}
void Resize(int w, int h) {normalCamera.Resize(w, h);}
void KeyboardFn(unsigned char key, int x, int y){normalCamera.KeyboardFn(key, x, y);}

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
    glDisable(GL_CULL_FACE);

    //使用混合方式
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_POLYGON_SMOOTH);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    //使用多重采样(默认开启)
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
