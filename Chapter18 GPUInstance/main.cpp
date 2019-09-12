#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"
#include "../Tools/Header/UtilTimer.h"
#include "../Tools/Header/GameObject.h"
#include "../Tools/Header/Camera.h"

BaseShaderParam param;
GLShaderManager glShaderMgr;
ShaderMgr* shaderMgr;
NormalCamera normalCamera;
UICamera uiCamera;

TriangleStripGObject triangle[2];
GLuint textures[3];
GLfloat angle;
GLboolean bUseGpuInstance = false;
GLuint maxCount = 100000;
FrameTimer timer;
int tickCount = 0;
bool add = true;

void Display(void)
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw ground
    GLMatrixStack* modelStack = &triangle[0].modelStack;
    param.SetMVMatrix(modelStack->GetMatrix());
    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(*modelStack));
    param.SetProjectMatrix(normalCamera.GetProjectMatrix());
    param.SetNormalMatrix(normalCamera.GetNormalMatrix(*modelStack));
    param.SetDiffuseColor(ShaderMgr::white);
    param.colorMap[0] = 2;
    shaderMgr->UseTexture2d(param);
    triangle[0].Draw();

    // draw grass
    modelStack = &triangle[1].modelStack;
    param.SetMVMatrix(modelStack->GetMatrix());
    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(*modelStack));
    param.SetProjectMatrix(normalCamera.GetProjectMatrix());
    param.SetNormalMatrix(normalCamera.GetNormalMatrix(*modelStack));
    param.SetDiffuseColor(ShaderMgr::white);
    param.colorMap[0] = 0;
    param.colorMap[1] = 1;
    if (bUseGpuInstance)
    {
        shaderMgr->DrawGrass(param, 0, tickCount/240.0f);
        glBindVertexArray(triangle[1].vao);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, maxCount);
    }
    else
    {
        for (int i = 0; i < maxCount; i++)
        {
            shaderMgr->DrawGrass(param, i, tickCount/240.0f);
            glBindVertexArray(triangle[1].vao);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }

    param.SetMVPMatrix(uiCamera.GetProjectMatrix());
    shaderMgr->UseFont(param, ShaderMgr::black);
    timer.Update();
    if (add)
        tickCount++;
    else
        tickCount--;
    if (tickCount >= 20)
        add = false;
    if (tickCount <= -20)
        add = true;
    glutSwapBuffers();
}

void OnStartUp()
{
    //init textures
    glGenTextures(3, textures);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    Util::LoadJPGTexture(Util::GetFullPath("Chapter18 GPUInstance/grass.png"), GL_LINEAR, GL_CLAMP);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    Util::LoadJPGTexture(Util::GetFullPath("Chapter18 GPUInstance/noise.png"), GL_LINEAR, GL_CLAMP);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    Util::LoadJPGTexture(Util::GetFullPath("Chapter18 GPUInstance/marble.tga"), GL_LINEAR, GL_CLAMP);

    // init triangle
    float vertices[] = {0,0,0,1, 0,0,1, 1,1,
                        1,0,0,1, 0,0,1, 0,1,
                        0,1,0,1, 0,0,1, 1,0,
                        1,1,0,1, 0,0,1, 0,0};

    for (int i = 0 ; i < 2; i++)
    {
        TriangleStripGObject& batch = triangle[i];
        glGenVertexArrays(1, &batch.vao);
        glGenBuffers(1, &batch.vbo);
        glBindVertexArray(batch.vao);
        glBindBuffer(GL_ARRAY_BUFFER, batch.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(4*sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7*sizeof(float)));
        glEnableVertexAttribArray(3);
    }
    triangle[0].modelStack.Rotate(90,1,0,0);
    triangle[0].modelStack.Scale(500,200,1);
    triangle[1].modelStack.Translate(0,0,-5);

    // init camera
    param.deltatime = 10;
    glShaderMgr.InitializeStockShaders();
    shaderMgr = ShaderMgr::GetInstance();
    normalCamera.OnInit(640, 480, 50, 1, 2);
    uiCamera.OnInit(120,120);
}

void OnShutUp()
{
    glDeleteTextures(2, textures);
    shaderMgr->OnUnInit();
    normalCamera.OnUnInit();
    uiCamera.OnUnInit();
}

void Idle(void) {glutPostRedisplay();}
void KeyboardFn(unsigned char key, int x, int y) {normalCamera.KeyboardFn(key, x, y);}
void MouseClick(int button, int action, int x, int y) {normalCamera.MouseClick(button, action, x, y);}
void MotionFunc(int mouse_x, int mouse_y) {normalCamera.MotionFunc(mouse_x, mouse_y);}
void Resize(int w, int h)
{
    normalCamera.Resize(w, h);
    normalCamera.Translate(-30, 5, -50);
    normalCamera.Rotate(10, 1, 0, 0);
}

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
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
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
