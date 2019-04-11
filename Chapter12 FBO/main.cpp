#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"

BaseShaderParam param;
GLShaderManager glShaderMgr;
ShaderMgr shaderMgr;
NormalCamera normalCamera;
GLMatrixStack* modelviewStack;

GLfloat angle;
GLTriangleBatch triangle;

GLfloat lightDirection[] = {0,100,0};

GLuint fbo;
GLuint rbo[4];
GLenum fboBuffers[]={GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
GLenum windowsBuffers[]={GL_FRONT_LEFT};

GLuint texture2d;
const char* filepath = "Chapter12 FBO/MoonLike.tga";

void Display(void)
{
    //绘制到fbo
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //设置gl_fragdata[]与缓冲区映射关系
    glDrawBuffers(3, fboBuffers);

    modelviewStack->PushMatrix();
    modelviewStack->Translate(0,0,-10);
    modelviewStack->Rotate(angle+=2, 0,1,0);
        glBindTexture(GL_TEXTURE_2D, texture2d);
        param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix());
        param.SetMVMatrix(normalCamera.GetModelviewMatrix());
        param.SetLightDirection(lightDirection);
        param.SetNormalMatrix(normalCamera.GetNormalMatrix());
        param.SetEnvironmentColor(ShaderMgr::ondine);
        param.colorMap[0] = 0;
        //shaderMgr.UseDiffuse(param);
        shaderMgr.DrawToFBO(param);
        triangle.Draw();
    modelviewStack->PopMatrix();

    //还原默认fbo
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDrawBuffers(1, windowsBuffers);
    //glViewport(0, 0, 640, 480);

    //将fbo中数据绘制到窗口
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(0, 0, 640, 480, 0, 0, 320, 240, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glReadBuffer(GL_COLOR_ATTACHMENT1);
    glBlitFramebuffer(0, 0, 640, 480, 320, 240, 640, 480, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glReadBuffer(GL_COLOR_ATTACHMENT2);
    glBlitFramebuffer(0, 0, 640, 480, 320, 0, 640, 240, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glutSwapBuffers();
}

void OnStartUp()
{
    //int fbo,rbo
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

    //将rbo设置为颜色缓冲区绑到fbo上
    glGenRenderbuffers(4, rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo[3]);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, 640, 480);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo[3]);

    for (int i = 0 ; i < 3; i++)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, rbo[i]);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, 640, 480);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_RENDERBUFFER, rbo[i]);
    }

    Util::CheckFBO();
    gltMakeCylinder(triangle, 1, 2, 3, 30, 30);

    // init camera
    glShaderMgr.InitializeStockShaders();
    shaderMgr.OnInit(1<<STFBO | 1<<STTexture2d);
    normalCamera.OnInit(640, 480, 50, 1, 2);
    modelviewStack = normalCamera.GetModelviewStack();

    //load texture
    glGenTextures(1, &texture2d);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture2d);
    Util::LoadJPGTexture(Util::GetFullPath(filepath), GL_LINEAR, GL_CLAMP);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OnShutUp()
{
    glDeleteTextures(1, &texture2d);
    glDeleteFramebuffers(1, &fbo);
    glDeleteRenderbuffers(4, rbo);
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
    glutCreateWindow("fbo");

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
