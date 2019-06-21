#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"
#include "../Tools/Header/FreetypeFont.h"

enum ETexture
{
    ETGround,
    ETCubeMaintex,
    ETDepth,
};

enum EBatch
{
    EBCube,
    EBGround,
    EBDepth,
    EBMax,
};

CharText charTex;
UICamera uiCamera;
GLMatrixStack* uiStack;

UICamera shadowCamera;
GLMatrixStack* shadowStack;

BaseShaderParam param;
GLShaderManager glShaderMgr;
ShaderMgr shaderMgr;
NormalCamera normalCamera;
GLMatrixStack* modelviewStack;

GLuint frameBuf;
GLfloat angle;
GLBatch triangles[3];
GLTriangleBatch cameraBatch;
GLuint textures[3];
GLfloat fontCol[] = {0,0,0,1};
char* paths[] =
{
    Util::GetFullPath("Chapter21 Shadowmap/pig.jpg"),
    Util::GetFullPath("Chapter21 Shadowmap/grass.jpg")
};

void DrawGround()
{
    shadowStack->PushMatrix();
    shadowStack->Translate(-10,-4,-20);
    shadowStack->Rotate(90,1,0,0);

    modelviewStack->PushMatrix();
    modelviewStack->Translate(-10,-4,-20);
    modelviewStack->Rotate(90,1,0,0);

    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix());
    param.SetDiffuseColor(ShaderMgr::white);
    param.colorMap[0] = 0;
    param.colorMap[1] = 1;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[(int)ETGround]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[(int)ETDepth]);
    shaderMgr.UseShaodwmap(param, shadowCamera.GetModelviewprojectMatrix());
    triangles[(int)EBGround].Draw();

    modelviewStack->PopMatrix();
    shadowStack->PopMatrix();
}

void DrawGroundShadow()
{
    shadowStack->PushMatrix();
    shadowStack->Translate(-10,-4,-20);
    shadowStack->Rotate(90,1,0,0);
    param.SetMVPMatrix(shadowCamera.GetModelviewprojectMatrix());

    shaderMgr.WriteToShadowmap(param);
    triangles[(int)EBGround].Draw();

    shadowStack->PopMatrix();
}

void DrawCube()
{
    shadowStack->PushMatrix();
    shadowStack->Translate(0,0,-15);
    shadowStack->Rotate(angle+=2,0,1,0);
    shadowStack->Rotate(10,1,0,0);
    shadowStack->Rotate(10,0,1,0);

    modelviewStack->PushMatrix();
    modelviewStack->Translate(0,0,-15);
    modelviewStack->Rotate(angle+=2,0,1,0);
    modelviewStack->Rotate(10,1,0,0);
    modelviewStack->Rotate(10,0,1,0);

    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix());
    param.SetDiffuseColor(ShaderMgr::white);
    param.colorMap[0] = 0;
    param.colorMap[1] = 1;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[(int)ETCubeMaintex]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[(int)ETDepth]);
    shaderMgr.UseShaodwmap(param, shadowCamera.GetModelviewprojectMatrix());
    triangles[(int)EBCube].Draw();

    modelviewStack->PopMatrix();
    shadowStack->PopMatrix();
}

void DrawCubeShadow()
{
    shadowStack->PushMatrix();
    shadowStack->Translate(0,0,-15);
    shadowStack->Rotate(angle+=2,0,1,0);
    shadowStack->Rotate(10,1,0,0);
    shadowStack->Rotate(10,0,1,0);

    param.SetMVPMatrix(shadowCamera.GetModelviewprojectMatrix());
    param.SetDiffuseColor(ShaderMgr::white);

    shaderMgr.WriteToShadowmap(param);
    triangles[(int)EBCube].Draw();

    shadowStack->PopMatrix();
}

void DrawDepthTexture()
{
    uiStack->PushMatrix();
    uiStack->Translate(-310,-230,0);
    param.SetMVPMatrix(uiCamera.GetModelviewprojectMatrix());
    param.SetDiffuseColor(ShaderMgr::white);
    param.colorMap[0] = 1;

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[(int)ETDepth]);
    shaderMgr.UseTexture2d(param);
    triangles[(int)EBDepth].Draw();

    uiStack->PopMatrix();
}

void DrawCharText()
{
    param.SetMVPMatrix(uiCamera.GetModelviewprojectMatrix());
    shaderMgr.UseFont(param, fontCol);
    charTex.Show();
}

void Display(void)
{
    //draw shadow
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,64,64);

    shadowStack->PushMatrix();
    shadowStack->Translate(0,0,0);
    shadowStack->Rotate(80,1,0,0);
    DrawCubeShadow();
    DrawGroundShadow();

    //
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,640,480);
    DrawCube();
    DrawGround();

    shadowStack->PopMatrix();

    DrawDepthTexture();
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

    //init cube
    gltMakeCube(triangles[(int)EBCube], 2.0f);
    gltMakeCylinder(cameraBatch, 0.5, 0, 1.0f, 10, 10);

    {
        GLBatch& triangle = triangles[(int)EBGround];
        triangle.Begin(GL_TRIANGLE_STRIP, 6, 1);
        triangle.MultiTexCoord2f(0,0,0);
        triangle.Vertex3f(-50,-50,0);
        triangle.MultiTexCoord2f(0,1,0);
        triangle.Vertex3f(50,-50,0);
        triangle.MultiTexCoord2f(0,0,1);
        triangle.Vertex3f(-50,50,0);
        triangle.MultiTexCoord2f(0,1,1);
        triangle.Vertex3f(50,50,0);
        triangle.End();
    }
    {
        GLBatch& triangle = triangles[(int)EBDepth];
        triangle.Begin(GL_TRIANGLE_STRIP, 6, 1);
        triangle.MultiTexCoord2f(0,0,0);
        triangle.Vertex3f(0,0,0);
        triangle.MultiTexCoord2f(0,1,0);
        triangle.Vertex3f(100,0,0);
        triangle.MultiTexCoord2f(0,0,1);
        triangle.Vertex3f(0,100,0);
        triangle.MultiTexCoord2f(0,1,1);
        triangle.Vertex3f(100,100,0);
        triangle.End();
    }
    //init camera
    glShaderMgr.InitializeStockShaders();
    shaderMgr.OnInit();
    normalCamera.OnInit(640, 480, 50, 1, 2);
    modelviewStack = normalCamera.GetModelviewStack();

    uiCamera.OnInit(640, 480);
    uiStack = uiCamera.GetModelviewStack();

    shadowCamera.OnInit(64, 64);
    shadowStack = shadowCamera.GetModelviewStack();

    //init framebuffer
    glGenFramebuffers(1, &frameBuf);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[(int)ETDepth]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 64, 64, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[(int)ETDepth], 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //init text
    char ch[] = "frame count:";
    charTex.CreateText(ch, sizeof(ch)-1, -320,220,0.2f,20);
}

void OnShutUp()
{
    charTex.OnUnInit();
    uiCamera.OnUnInit();
    shadowCamera.OnUnInit();
    glDeleteTextures(3, textures);
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
    glutCreateWindow("shadow map");

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
