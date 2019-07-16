#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"
#include "../Tools/Header/GameObject.h"

GLShaderManager glShaderMgr;
ShaderMgr shaderMgr;
NormalCamera normalCamera;
BaseShaderParam param;

GLint tickCount = 0;
GLfloat angle;
BatchGObject triangle;
GLuint textures[3];
const char* filepath[]=
{
    "Chapter14 HDRTexture/Cannon.exr",
    "Chapter14 HDRTexture/Ocean.exr",
    "Chapter14 HDRTexture/Tree.exr"
};
int exposure[] = {2,5,10};

void Display(void)
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLMatrixStack& modelviewStack = triangle.modelviewStack;
    param.SetNormalMatrix(normalCamera.GetNormalMatrix(modelviewStack));
    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(modelviewStack));
    param.SetMVMatrix(modelviewStack.GetMatrix());
    param.colorMap[0] = 0;

    int index = tickCount++/100%3;
    glBindTexture(GL_TEXTURE_2D, textures[index]);
    shaderMgr.UseHDR(param, exposure[index]);
    triangle.Draw();

    glutSwapBuffers();
}

void OnStartUp()
{
    //init cube
    gltMakeCube(triangle.batch, 1);
    triangle.modelviewStack.Translate(0, 0, -2.5);
    triangle.modelviewStack.Rotate(angle, 0, 1, 0);

    //init hdr texture
    int width,height=0;
    glGenTextures(3, textures);
    glActiveTexture(GL_TEXTURE0);
    for (int i = 0; i < 3; i++)
    {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        Util::LoadOpenEXR(Util::GetFullPath(filepath[i]), GL_LINEAR, GL_REPEAT, width, height);
    }

    //init camera,shadermgr
    glShaderMgr.InitializeStockShaders();
    shaderMgr.OnInit(1<<STHDR | 1<<STTexture2d);
    normalCamera.OnInit(640, 480, 50, 1, 2);
}

void OnShutUp()
{
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
    glutCreateWindow("hdr texture");


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
