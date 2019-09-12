#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"
#include "../Tools/Header/FreetypeFont.h"
#include "../Tools/Header/Camera.h"

BaseShaderParam param;
ShaderMgr* shaderMgr;
UICamera uiCamera;


CharText charTex;
char text[] = "hello world";
GLfloat color[] ={1,0,0,0};

void Display(void)
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    param.SetMVPMatrix(uiCamera.GetModelviewprojectMatrix(charTex.modelStack));
    param.colorMap[0] = 0;
    shaderMgr->UseFont(param, color);
    charTex.Draw();

    glutSwapBuffers();
}

void OnStartUp()
{
    charTex.CreateText(text, sizeof(text) - 1, 0, 0, 0.2f);
    shaderMgr = ShaderMgr::GetInstance();
    uiCamera.OnInit(640, 480);
}

void OnShutUp()
{
    charTex.OnUnInit();
    shaderMgr->OnUnInit();
    uiCamera.OnUnInit();
}

void Idle(void) {glutPostRedisplay();}
void Resize(int w, int h) {uiCamera.Resize(w, h);}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("freetype font");

    glutReshapeFunc(Resize);
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
