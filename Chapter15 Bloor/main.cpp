#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"

GLShaderManager glShaderMgr;
ShaderMgr shaderMgr;
NormalCamera normalCamera;
GLMatrixStack* modelviewStack;
BaseShaderParam param;

GLint tickCount = 0;
GLfloat angle;
GLBatch triangles[3];

GLuint textures[1];
GLuint normalFbo;
GLuint normalDepthRbo;
GLuint normalTexture;

GLuint brightFbo;
GLuint brightDepthRbo;
GLuint brightTexture;

GLuint blurFbo;
GLuint blurDepthRbo;
GLuint blurTexture;
GLfloat exposure = 5;
GLfloat brightLimit = 0.5f;

static const GLenum windowBuff[] = {GL_BACK_LEFT};
static const GLenum fboBuffs[] = {GL_COLOR_ATTACHMENT0};

void Display(void)
{
    modelviewStack->PushMatrix();
    modelviewStack->Translate(-5, -5, -10);
    modelviewStack->Rotate(angle, 0, 1, 0);
    param.SetNormalMatrix(normalCamera.GetNormalMatrix());
    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix());
    param.SetMVMatrix(normalCamera.GetModelviewMatrix());

    //正常渲染到texture[0]
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, normalFbo);
    glDrawBuffers(1, fboBuffs);
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    param.colorMap[0] = 0;
    shaderMgr.UseBloorBase(param);
    triangles[0].Draw();


    //对texture[0]曝光获得高亮部分,输出到texture[1]
    param.colorMap[0] = 1;
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, brightFbo);
    glDrawBuffers(1, fboBuffs);
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderMgr.UseBloorBright(param, brightLimit);
    triangles[1].Draw();

/*
    //对texture[1]多次模糊输出到texture[2]
    param.colorMap[0] = 2;
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, blurFbo);
    glDrawBuffers(1, fboBuffs);
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderMgr.UseBloorBlur(param, 0.6f);
    triangle.Draw();
*/
    //将三张texture混合输出到屏幕
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDrawBuffers(1, windowBuff);
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    param.colorMap[0] = 1;
    param.colorMap[1] = 2;
    param.colorMap[2] = 3;
    shaderMgr.UseBloorMix(param, exposure);
    triangles[2].Draw();
    modelviewStack->PopMatrix();

    glutSwapBuffers();
}

void CreateFBOTexture(GLuint* fbo, GLuint* rbo, GLuint* texture, GLuint textureUnit, float width, float height)
{
    //init fbo，texture
    glGenFramebuffers(1, fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, *fbo);
    glActiveTexture(textureUnit);
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //use float texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    //bind fbo with texture
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *texture, 0);

    //init depth render buffer
    glGenRenderbuffers(1, rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, *rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
    glFramebufferRenderbuffer(GL_DRAW_BUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *rbo);
}

void OnStartUp()
{
    //init triangle
    for (int i = 0 ; i < 3; i++)
    {
        GLBatch triangle = triangles[i];
        triangle.Begin(GL_TRIANGLE_STRIP, 6, 1);
        triangle.MultiTexCoord2f(0,0,0);
        triangle.Vertex3f(0,0,0);
        triangle.MultiTexCoord2f(0,1,0);
        triangle.Vertex3f(10,0,0);
        triangle.MultiTexCoord2f(0,1,1);
        triangle.Vertex3f(10,10,0);
        triangle.MultiTexCoord2f(0,0,1);
        triangle.Vertex3f(0,10,0);
        triangle.End();
    }

    //init hdr texture
    int width,height=0;
    glGenTextures(1, textures);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    Util::LoadOpenEXR(Util::GetFullPath("Chapter15 Bloor/Ocean.exr"), GL_LINEAR, GL_REPEAT, width, height);

    //init camera,shadermgr
    glShaderMgr.InitializeStockShaders();
    shaderMgr.OnInit(1<<STHDR | 1<<STTexture2d | 1<<STBLOOR);
    normalCamera.OnInit(640, 480, 50, 1, 2);
    modelviewStack = normalCamera.GetModelviewStack();

    //init fbo,texture
    CreateFBOTexture(&normalFbo, &normalDepthRbo, &normalTexture, GL_TEXTURE1, 640, 480);
    CreateFBOTexture(&brightFbo, &brightDepthRbo, &brightTexture, GL_TEXTURE2, 640, 480);
    CreateFBOTexture(&blurFbo, &blurDepthRbo, &blurTexture, GL_TEXTURE3, 640, 480);
}

void OnShutUp()
{
    //del texture
    glDeleteTextures(1, &normalTexture);
    glDeleteTextures(1, &brightTexture);
    glDeleteTextures(1, &blurTexture);
    //del render buffer
    glDeleteBuffers(1, &normalDepthRbo);
    glDeleteBuffers(1, &brightDepthRbo);
    glDeleteBuffers(1, &blurDepthRbo);
    //del frame buffer
    glDeleteFramebuffers(1, &normalFbo);
    glDeleteFramebuffers(1, &brightFbo);
    glDeleteFramebuffers(1, &blurFbo);

    glDeleteTextures(1, textures);
    shaderMgr.OnUnInit();
    normalCamera.OnUnInit();
}

void ChangeExposure(unsigned char key)
{
    if (key == 'z')
        exposure += 1;
    else if (key == 'x')
        exposure -= 1;
}

void KeyboardFn(unsigned char key, int x, int y)
{
    ChangeExposure(key);
    normalCamera.KeyboardFn(key, x, y);
}
void Idle(void) {glutPostRedisplay();}
void MouseClick(int button, int action, int x, int y) {normalCamera.MouseClick(button, action, x, y);}
void MotionFunc(int mouse_x, int mouse_y) {normalCamera.MotionFunc(mouse_x, mouse_y);}
void Resize(int w, int h) {normalCamera.Resize(w, h);}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("hdr bloor");

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
