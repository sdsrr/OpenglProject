#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"
#include "../Tools/Header/UtilTimer.h"

BaseShaderParam param;
GLShaderManager glShaderMgr;
ShaderMgr shaderMgr;
NormalCamera normalCamera;
GLMatrixStack* modelviewStack;

GLuint textures[2];
GLfloat angle;
GLBatch grassBatch;
GLboolean bUseGpuInstance = false;
GLuint maxCount = 10000;
FrameTimer timer;

void Display(void)
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (bUseGpuInstance)
    {

    }
    else
    {
        modelviewStack->PushMatrix();
        modelviewStack->Translate(0, 0, -5);
        param.SetMVMatrix(normalCamera.GetModelviewMatrix());
        param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix());
        param.SetProjectMatrix(normalCamera.GetProjectMatrix());
        param.SetNormalMatrix(normalCamera.GetNormalMatrix());
        param.SetDiffuseColor(ShaderMgr::white);
        param.colorMap[0] = 0;
        param.colorMap[1] = 1;
        for (int i = 0; i < maxCount; i++)
        {
            shaderMgr.DrawGrass(param, i);
            grassBatch.Draw();
        }
        modelviewStack->PopMatrix();
    }

    timer.Update();
    glutSwapBuffers();
}

void OnStartUp()
{
    //init textures
    glGenTextures(2, textures);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    //Util::LoadTGATexture("Chapter18 GPUInstance/grass.png", GL_LINEAR, GL_CLAMP);
    Util::LoadJPGTexture(Util::GetFullPath("Chapter18 GPUInstance/grass.png"), GL_LINEAR, GL_CLAMP);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    Util::LoadJPGTexture(Util::GetFullPath("Chapter18 GPUInstance/noise.png"), GL_LINEAR, GL_CLAMP);

    // init triangle
    grassBatch.Begin(GL_TRIANGLE_STRIP, 6, 1);
    grassBatch.MultiTexCoord2f(0,1,1);
    grassBatch.Normal3f(0,0,1);
    grassBatch.Vertex3f(0,0,0);

    grassBatch.MultiTexCoord2f(0,0,1);
    grassBatch.Normal3f(0,0,1);
    grassBatch.Vertex3f(1,0,0);

    grassBatch.MultiTexCoord2f(0,1,0);
    grassBatch.Normal3f(0,0,1);
    grassBatch.Vertex3f(0,1,0);

    grassBatch.MultiTexCoord2f(0,0,0);
    grassBatch.Normal3f(0,0,1);
    grassBatch.Vertex3f(1,1,0);
    grassBatch.End();
    // init camera
    glShaderMgr.InitializeStockShaders();
    shaderMgr.OnInit();
    normalCamera.OnInit(640, 480, 50, 1, 2);
    modelviewStack = normalCamera.GetModelviewStack();
}

void OnShutUp()
{
    glDeleteTextures(2, textures);
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
    glutCreateWindow("gpu instance");

    glutKeyboardFunc(KeyboardFn);
    glutReshapeFunc(Resize);
    glutMotionFunc(MotionFunc);
    glutMouseFunc(MouseClick);
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
